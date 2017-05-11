#include "hexaServo.h"

  // create servo object to control a servo
Servo myservo0;
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8;
Servo myservo9;
Servo myservo10;
Servo myservo11;

Servo myservo[12];


int g_hexaPos[12];

void setting_pin_servo( int pin_S0, int pin_S1, int pin_S2, int pin_S3, int pin_S4, int pin_S5, int pin_S6, int pin_S7, int pin_S8, int pin_S9, int pin_S10, int pin_S11){

  // attach the servo on the pin number
myservo0.attach(pin_S0);
myservo1.attach(pin_S1);
myservo2.attach(pin_S2);
myservo3.attach(pin_S3);
myservo4.attach(pin_S4);
myservo5.attach(pin_S5);
myservo6.attach(pin_S6);
myservo7.attach(pin_S7);
myservo8.attach(pin_S8);
myservo9.attach(pin_S9);
myservo10.attach(pin_S10);
myservo11.attach(pin_S11);


Servo myservo_attached[12]={myservo0,myservo1,myservo2,myservo3,myservo4,myservo5,myservo6,myservo7,myservo8,myservo9,myservo10,myservo11};
  for( int i = 0; i < 12; i++ )
  {
myservo[i] = myservo_attached[i];
}
}


int hexaDCtoUS(int DC){
  // From 0 (duty cycle = 0%) to 4096 (duty cycle = 100%)
  // For used servo pulse duration is from 500us to 2500us
  // Frequency : 50Hz
  int US;
  US = map(DC, 0, 4096, 0, 20000);
  return US;
}



int hexaServoHome( int index )
{
  switch( index )
  {
    case 0:
      return _SERVOHOME0_;
    case 1:
	    return _SERVOHOME1_;
    case 2:
	    return _SERVOHOME2_;
    case 3:
	    return _SERVOHOME3_;
    case 4:
	    return _SERVOHOME4_;
    case 5:
	    return _SERVOHOME5_;
    case 6:
	    return _SERVOHOME6_;
    case 7:
	    return _SERVOHOME7_;
    case 8:
	    return _SERVOHOME8_;
    case 9:
	    return _SERVOHOME9_;
    case 10:
	    return _SERVOHOME10_;
    case 11:
	    return _SERVOHOME11_;
  }
}

void hexaServoInit()
{
  hexaHoming();
}

void hexaHoming()
{
  int i;
  for( i = 0; i < 12; i++ )
  {
//  g_pwm.setPWM( hexaServoMap( i ), 0, hexaServoHome( i ) );
Serial.println( (hexaDCtoUS(hexaServoHome( i ))));
    myservo[i].writeMicroseconds(hexaDCtoUS(hexaServoHome( i )));
    g_hexaPos[ i ] = hexaServoHome( i );
  }
}

void hexaShuttingDown()
{
  hexaHoming();
  /* to be done */
  delay(1000);
}

void hexaSetPos( int index, int pos )
{
  if( 6 > index )
  {
//  g_pwm.setPWM( hexaServoMap( index ), 0, hexaServoHome( index ) + pos );
    myservo[index].writeMicroseconds(hexaDCtoUS(hexaServoHome(index)  + pos));
    g_hexaPos[ index ] = hexaServoHome( index ) + pos;
  }
  else
  {
//  g_pwm.setPWM( hexaServoMap( index ), 0, hexaServoHome( index ) - pos );
    myservo[index].writeMicroseconds(hexaDCtoUS(hexaServoHome(index)  - pos));
    g_hexaPos[ index ] = hexaServoHome( index ) - pos;
  }
  delay( _STDBREAK_ );
}

void hexaMove( double lenght, double curve, double stepSize, int height )
{
  int i, j, hexaFor, hexaBack, corrHeight, highLeg;
  double d, distance, corrStep, curveLeft, curveRight;

  /* Going forward or backward */
  if( 0 > lenght )
  {
    distance = -lenght;
    hexaFor = _PWMHMIN_;
    hexaBack = _PWMHMAX_;
  }
  else
  {
    distance = lenght;
    hexaFor = _PWMHMAX_;
    hexaBack = _PWMHMIN_;
  }

  /* Determine the side which is modified to control the exentricity of the movement */
  if( -1 > curve )
  {
    curveLeft = 0;
    curveRight = 1;
  }
  else if( 0 > curve )
  {
    curveLeft = 0;
    curveRight = -curve;
  }
  else if( 1 > curve )
  {
    curveLeft = curve;
    curveRight = 0;
  }
  else
  {
    curveLeft = 1;
    curveRight = 0;
  }

  /* Corection on step size to reduce speed */
  if( 0 > stepSize )
  {
    corrStep = 0;
  }
  else if( 1 < stepSize )
  {
    corrStep = 1;
  }
  else
  {
    corrStep = stepSize;
  }

  /* Setting height value */
  if( _PWMVMIN_ + _STDHEIGHT_ > height )
  {
    corrHeight = _PWMVMIN_ + _STDHEIGHT_;
  }
  else if( _PWMVMAX_ < height )
  {
    corrHeight = _PWMVMAX_;
  }
  else
  {
    corrHeight = height;
  }

  /* Determine which leg is high and cofirm home position */
  if( g_hexaPos[0] > g_hexaPos[2] )
  {
    highLeg = 1;
    for( i = 0; i < 3; i++)
    {
      hexaSetPos( 4 * i, corrHeight );
      hexaSetPos( 4 * i + 1, 0 );
      hexaSetPos( 4 * i + 2, corrHeight - _STDHEIGHT_ );
      hexaSetPos( 4 * i + 3, 0 );
    }
  }
  else
  {
    highLeg = 0;
    for( i = 0; i < 3; i++ )
    {
      hexaSetPos( 4 * i, corrHeight - _STDHEIGHT_ );
      hexaSetPos( 4 * i + 1, 0 );
      hexaSetPos( 4 * i + 2, corrHeight );
      hexaSetPos( 4 * i + 3, 0 );
    }
  }

  while( _ERRLENGHT_ * _STEPLENGHT_ < distance )
  {
    if( _STEPLENGHT_ * corrStep > distance )
    {
      corrStep = distance / _STEPLENGHT_;
    }
    distance -= _STEPLENGHT_ * corrStep;

    if( 0 != highLeg )
    {
      for( i = 0; i < _NBSTEPS_; i++ )
      {
        d = i * 1.0 /_NBSTEPS_;
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j + 2, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 3, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 5, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 7, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 9, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 11, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
      }
      for( i = _NBSTEPS_; i > 0; i-- )
      {
        d = i * 1.0 /_NBSTEPS_;
     
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 3, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 5, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 7, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 9, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 11, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
      }
    }
    else
    {
      for( i = 0; i < _NBSTEPS_; i++ )
      {
        d = i * 1.0 /_NBSTEPS_;
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 3, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 5, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 7, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 9, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 11, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
      }
      for( i = _NBSTEPS_; i > 0; i-- )
      {
        d = i * 1.0 /_NBSTEPS_;
     
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j + 2, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 3, int( d * hexaBack * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 5, int( d * hexaFor * corrStep * ( 1 - curveLeft ) ) );
        hexaSetPos( 7, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 9, int( d * hexaFor * corrStep * ( 1 - curveRight ) ) );
        hexaSetPos( 11, int( d * hexaBack * corrStep * ( 1 - curveRight ) ) );
      }
    }
    highLeg = 1 - highLeg;
  }
}

void hexaRotate( double angle, double stepSize, int height )
{
  int i, j, hexaFor, hexaBack, corrHeight, highLeg;
  double d, absAngle, corrStep;

  if( 0 > angle )
  {
    absAngle = -angle;
    hexaFor = _PWMHMIN_;
    hexaBack = _PWMHMAX_;
  }
  else
  {
    absAngle = angle;
    hexaFor = _PWMHMAX_;
    hexaBack = _PWMHMIN_;
  }

  if( 0 > stepSize )
  {
    corrStep = 0;
  }
  else if( 1 < stepSize )
  {
    corrStep = 1;
  }
  else
  {
    corrStep = stepSize;
  }

  if( _PWMVMIN_ + _STDHEIGHT_ > height )
  {
    corrHeight = _PWMVMIN_ + _STDHEIGHT_;
  }
  else if( _PWMVMAX_ < height )
  {
    corrHeight = _PWMVMAX_;
  }
  else
  {
    corrHeight = height;
  }

  /* Determine which leg is high and cofirm home position */
  if( g_hexaPos[0] > g_hexaPos[2] )
  {
    highLeg = 1;
    for( i = 0; i < 3; i++)
    {
      hexaSetPos( 4 * i, corrHeight );
      hexaSetPos( 4 * i + 1, 0 );
      hexaSetPos( 4 * i + 2, corrHeight - _STDHEIGHT_ );
      hexaSetPos( 4 * i + 3, 0 );
    }
  }
  else
  {
    highLeg = 0;
    for( i = 0; i < 3; i++ )
    {
      hexaSetPos( 4 * i, corrHeight - _STDHEIGHT_ );
      hexaSetPos( 4 * i + 1, 0 );
      hexaSetPos( 4 * i + 2, corrHeight );
      hexaSetPos( 4 * i + 3, 0 );
    }
  }

  while( _ERRANGLE_ * _STEPANGLE_ < absAngle )
  {
    if( _STEPANGLE_ * corrStep > absAngle )
    {
      corrStep = absAngle / _STEPANGLE_;
    }
    absAngle -= _STEPANGLE_ * corrStep;
    
    if( 0 != highLeg )
    {
      for( i = 0; i < _NBSTEPS_; i++ )
      {
        d = i * 1.0 /_NBSTEPS_;
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j + 2, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaFor * corrStep ) );
        hexaSetPos( 3, int( d * hexaBack * corrStep ) );
        hexaSetPos( 5, int( d * hexaFor * corrStep ) );
        hexaSetPos( 7, int( d * hexaFor * corrStep ) );
        hexaSetPos( 9, int( d * hexaBack * corrStep ) );
        hexaSetPos( 11, int( d * hexaFor * corrStep ) );
      }
      for( i = _NBSTEPS_; i > 0; i-- )
      {
        d = i * 1.0 /_NBSTEPS_;
     
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaFor * corrStep ) );
        hexaSetPos( 3, int( d * hexaBack * corrStep ) );
        hexaSetPos( 5, int( d * hexaFor * corrStep ) );
        hexaSetPos( 7, int( d * hexaFor * corrStep ) );
        hexaSetPos( 9, int( d * hexaBack * corrStep ) );
        hexaSetPos( 11, int( d * hexaFor * corrStep ) );
      }
    }
    else
    {
      for( i = 0; i < _NBSTEPS_; i++ )
      {
        d = i * 1.0 /_NBSTEPS_;
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaBack * corrStep ) );
        hexaSetPos( 3, int( d * hexaFor * corrStep ) );
        hexaSetPos( 5, int( d * hexaBack * corrStep ) );
        hexaSetPos( 7, int( d * hexaBack * corrStep ) );
        hexaSetPos( 9, int( d * hexaFor * corrStep ) );
        hexaSetPos( 11, int( d * hexaBack * corrStep ) );
      }
      for( i = _NBSTEPS_; i > 0; i-- )
      {
        d = i * 1.0 /_NBSTEPS_;
     
        for( j = 0; j < 3; j++ )
        {
          hexaSetPos( 4 * j + 2, corrHeight - int( ( 1 - d ) * _STDHEIGHT_ ) );
        }
        hexaSetPos( 1, int( d * hexaBack * corrStep ) );
        hexaSetPos( 3, int( d * hexaFor * corrStep ) );
        hexaSetPos( 5, int( d * hexaBack * corrStep ) );
        hexaSetPos( 7, int( d * hexaBack * corrStep ) );
        hexaSetPos( 9, int( d * hexaFor * corrStep ) );
        hexaSetPos( 11, int( d * hexaBack * corrStep ) );
      }
    }
    highLeg = 1 - highLeg;
  }
}

