#include "hexaServo.h"
#include "PS2X_lib.h"


// Define pins used to connect the PS2 controller
#define PS2_DAT     14    
#define PS2_CMD     16
#define PS2_SEL     7
#define PS2_CLK     15

// SDA pin = D2 or pin 2 on a spark pro micro
// SCL pin = D3 or pin 3 on a spark pro micro


// int g_hexaAngle;
// int g_hexaSpeed;

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

PS2X ps2x;        // create PS2 Controller Class



void setup(){

  Serial.begin(57600);
  delay(4000);
  Serial.println();
  Serial.println( "Serial connexion established" );

  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);

  if (error == 1){
    Serial.println();
    Serial.println( "No PS2 gamepad found" );
  }else{
    Serial.println("PS2 gamepad found");
  }

//  g_hexaAngle = 0;
//  g_hexaSpeed = 0;
  hexaServoInit();
  hexaHoming();
  delay(1000);
//  hexaMove( 3, 0, 1, 0 );
  Serial.println();
  Serial.println( "Home position set" );

}

void loop() {

  ps2x.read_gamepad();

// Un pas en arriere à pleine vitesse  
if(ps2x.Button(PSB_PAD_DOWN)){
  hexaMove( -1, 0, 1, height);
  ps2x.read_gamepad();
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



else if( ps2x.Analog(PSS_LY) >=150 || ps2x.Analog(PSS_LY) <=110 ||ps2x.Analog(PSS_LX) >= 150 ||ps2x.Analog(PSS_LX) <= 110 ){
//  Serial.println("\nLeft analog stick ");
//  Serial.print(ps2x.Analog(PSS_LX), DEC);
//  Serial.print(",");
//  Serial.print(ps2x.Analog(PSS_LY), DEC); 
// Serial.print(ps2x.Analog(PSS_LX), DEC);
// Lx =  (Lx << 255) | ps2x.Analog(PSS_LX);
// Serial.print(ps2x.Analog(PSS_LY), DEC);
// Ly =  (Ly << 255) | ps2x.Analog(PSS_LY);
//  double stepSize = ((double)Ly-127)/127.0;
//  double curve = ((double)Lx-127)/127.0;


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
