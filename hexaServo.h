#ifndef _HEXASERVO_H_
#define _HEXASERVO_H_

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


/* 
 *  Servo index remapping
 *  0/1 _____ 7/6
 *     |     |
 *  2/3|  ^  |9/8
 *     |_____|
 *  4/5       11/10
 */
#define _SERVO0_ 0
#define _SERVO1_ 2
#define _SERVO2_ 5
#define _SERVO3_ 6
#define _SERVO4_ 14
#define _SERVO5_ 12
#define _SERVO6_ 1
#define _SERVO7_ 3
#define _SERVO8_ 4
#define _SERVO9_ 7
#define _SERVO10_ 15
#define _SERVO11_ 13

/* PWM frequency (take care of change in home position to avoid damage) */
#define _PWMFREQ_ 50

/* Servo home position 
 * From 0 (duty cycle = 0%) to 4096 (duty cycle = 100%)
 * For used servo pulse duration is from 500us to 2500us
 * Theorical home position is: 4096*(TMAX-TMIN)/TCLK = 4096*(2500-500)*50 = 307
 */
#define _SERVOHOME0_ 238    
#define _SERVOHOME1_ 288
#define _SERVOHOME2_ 232
#define _SERVOHOME3_ 270 //283
#define _SERVOHOME4_ 340
#define _SERVOHOME5_ 275  //288
#define _SERVOHOME6_ 365
#define _SERVOHOME7_ 285  //295
#define _SERVOHOME8_ 352
#define _SERVOHOME9_ 276
#define _SERVOHOME10_ 344   
#define _SERVOHOME11_ 285 


/* Horizontal and vertical limit in term of duty cycle from home position */
#define _PWMHMIN_ -52 
#define _PWMHMAX_ 52 
#define _PWMVMIN_ -110
#define _PWMVMAX_ 120 

/* Break time between instruction for servo rotation */
#define _STDBREAK_ 1

/* Number of steps per instructions */
#define _NBSTEPS_ 15

/* Lenght or angle of one step */
#define _STEPLENGHT_ 1
#define _STEPANGLE_ 1

/* Standard elevation of hexa legs */
#define _STDHEIGHT_ 70

/* Accuracy of the movement */
#define _ERRLENGHT_ 0.01
#define _ERRANGLE_ 0.01

/* Abstraction functions */
int hexaServoMap( int index );
int hexaServoHome( int index );
/* Initiate I2C connexion */
void hexaServoInit();
/* Set hexa legs in standard position */
void hexaHoming();
/* Set hexa legs close to tidied position */
void hexaShuttingDown();
/* Abstratcion function:
 * Go front if positive for horizontal mouvement (odd index)
 * Go deep if positive for vertical mouvement (even index)
 */
void hexaSetPos( int index, int pos );
/* Displacement function:
 * lenght, angle can be positive or negative 
 * lenght, angle are absolute. Trigger _STEPLENGHT_ and _STEPANGLE_ for a correct conversion
 * angle is counterclockwise
 * stepSize reduces speed, a [0,1] value is expected (0 -> no move, 1 -> max step)
 * height is given in term of duty cycle as an offset from home position
 */
void hexaToward( double lenght, double stepSize, int height );
void hexaRotate( double angle, double stepSize, int height );
/* curve allows slanted paths, a [-1,1] value is expected (-1 -> clockwise, 0 -> straight, 1 -> counterclockwise)
 * lenght represents the distance crossed by legs.
 * It may be very different from actual distance crossed by hexa in case of sharp curve (closed to -1 or 1)
 */
void hexaMove( double lenght, double curve, double stepSize, int height );

#endif
