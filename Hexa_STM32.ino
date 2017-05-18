#include "hexaServo.h"
#include "PS2X_lib.h"
#include <SPI.h>



int SPI_freq = 200000;    // 500kHz SPI frequency for PS2 controller
SPIClass spi(2);          // SPI 2 used in this project
PS2X ps2x(spi, SPI_freq); // create PS2 Controller Class



//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
byte vibrate = 0;


int error = 0; // returned status of PS2 controller connection.
// error = 0 -> everything fine, controller found and well configured
// error = 1 -> No cotroller found
// error = 2 -> controller found but not configured (rumble, analog ...)
// error = 3 -> controller not in pressure mode (may not support it)

int Lx = 127; 
int Ly = 127;
int Rx = 127;
int Ry = 127;
// Values of analog joysticks. 127 is the default position (min=0 & max=255)

double lenght = 0;
double curve = 0.0;
double stepSize = 0.0;
int height = 0;
double angle =0;

int pin_servo0 = 3;
int pin_servo1 = 4;
int pin_servo2 = 5;
int pin_servo3 = 8;
int pin_servo4 = 9;
int pin_servo5 = 10;
int pin_servo6 = 11;
int pin_servo7 = 15;
int pin_servo8 = 16;
int pin_servo9 = 25;
int pin_servo10 = 26;
int pin_servo11 = 27;

void setup(){
  delay(2500);
/* attaches the servo on the pin number
/* 
 *  Servo mapping
 *  0/1 _____ 7/6
 *     |     |
 *  2/3|  ^  |9/8
 *     |_____|
 *  4/5       11/10
 */

setting_pin_servo(pin_servo0,pin_servo1,pin_servo2,pin_servo3,pin_servo4,pin_servo5,pin_servo6,pin_servo7,pin_servo8,pin_servo9,pin_servo10,pin_servo11); 


//  Serial.begin(57600);
//  Serial.println();
//  Serial.println( "Serial connexion established" );

   error = ps2x.config_gamepad(31);   //SPI number (SPI1 or SP2) is already defined when SPIClass is created. Here it is just to define the pin for attention
 
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
   //Serial.print(ps2x.Analog(1), HEX);
// ps2x.enableRumble();              //enable rumble vibration motors

 //ps2x.enablePressures();           //enable reading the pressure values from the buttons.  

  hexaServoInit();

 hexaHoming();


/*
while (error != 4){
error = ps2x.config_gamepad(31);
Serial.println("waiting for analog mode");
 delay(1000);
}
*/

}

void loop() {

  ps2x.read_gamepad(false, vibrate);  

// delay(500);


// Un pas en arriere à pleine vitesse  
if(ps2x.Button(PSB_PAD_DOWN)){
  hexaMove( -1, 0, 1, height);
  ps2x.read_gamepad();
//    Serial.println( "PSB_PAD_DOWN" );
  }
  
// Un pas en avant à pleine vitesse 
else if(ps2x.Button(PSB_PAD_UP)){
  hexaMove( 1, 0, 1, height);
  ps2x.read_gamepad();
  }

// Rotation sur place vers la gauche
else if(ps2x.Button(PSB_PAD_LEFT)){
  hexaRotate( 1, 1, height );
  ps2x.read_gamepad();
  }

// Rotation sur place vers la droite
else if(ps2x.Button(PSB_PAD_RIGHT)){
  hexaRotate( -1, 1, height );
  ps2x.read_gamepad();
  }  

// Rotation sur place vers la droite
else if(ps2x.Button(PSB_TRIANGLE)){
  hexaHoming();
  height = 0;
  ps2x.read_gamepad();
  }  

else if(ps2x.Button(PSB_L1)){
  height = height -15 ;
  hexaMove( -0.1, 0, 1, height);
  ps2x.read_gamepad();
  }

else if(ps2x.Button(PSB_R1)){
  height = height +15 ;
  hexaMove( -0.1, 0, 1, height);
  ps2x.read_gamepad();
  }

// Danse de la vistoire
else if(ps2x.Button(PSB_CIRCLE)){
  hexaRotate( -1, 1, height );
  hexaRotate( -1, 1, height );
  hexaRotate( -1, 1, height );
  hexaRotate( 1, 1, height );
  hexaMove( -1, 0, 1, height );
  hexaMove( -1, 0, 1, height );
  hexaMove( 1, 0, 1, height );
  hexaMove( 1, 0, 1, height );
  hexaRotate( 1, 1, height );
  hexaRotate( 1, 1, height );
  hexaRotate( 1, 1, height );
  hexaRotate( -1, 1, height );
  ps2x.read_gamepad();
  }  
// Danse de la vistoire
else if(ps2x.Button(PSB_CROSS)){
  
  ps2x.read_gamepad();
  }  

else if((ps2x.Analog(PSS_LY) >=150 || ps2x.Analog(PSS_LY) <=110 ||ps2x.Analog(PSS_LX) >= 150 ||ps2x.Analog(PSS_LX) <= 110 )){
    Serial.println( "analog move" );
              Serial.println(ps2x.Analog(PSS_RX));
        Serial.println(ps2x.Analog(PSS_LY));
if (ps2x.Analog(PSS_LY) <= 40){ stepSize= 1;
}else if (ps2x.Analog(PSS_LY) <= 70){ stepSize= 0.75;
}else if (ps2x.Analog(PSS_LY) <= 100){ stepSize= 0.5;
}else if (ps2x.Analog(PSS_LY) <= 120){ stepSize= 0.25;
}else if (ps2x.Analog(PSS_LY) <= 140){ stepSize= -0.1;
}else if (ps2x.Analog(PSS_LY) <= 160){ stepSize= -0.25;
}else if (ps2x.Analog(PSS_LY) <= 190){ stepSize= -0.5;
}else if (ps2x.Analog(PSS_LY) <= 220){ stepSize= -0.75;
}else if (ps2x.Analog(PSS_LY) <= 256){ stepSize= -1;
}else stepSize= 0;

if (ps2x.Analog(PSS_RX) <= 40){ curve= 1;
}else if (ps2x.Analog(PSS_RX) <= 70){ curve= 0.75;
}else if (ps2x.Analog(PSS_RX) <= 100){ curve= 0.5;
}else if (ps2x.Analog(PSS_RX) <= 120){ curve= 0.25;
}else if (ps2x.Analog(PSS_RX) <= 140){ curve= 0.0;
}else if (ps2x.Analog(PSS_RX) <= 160){ curve= -0.25;
}else if (ps2x.Analog(PSS_RX) <= 190){ curve= -0.5;
}else if (ps2x.Analog(PSS_RX) <= 220){ curve= -0.75;
}else if (ps2x.Analog(PSS_RX) <= 256){ curve= -1;
}else curve= 0.0;




         
  hexaMove( stepSize, curve, 1, height );
  ps2x.read_gamepad();
}


  delay(20);  

}
