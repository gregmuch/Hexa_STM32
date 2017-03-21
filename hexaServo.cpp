#include "hexaServo.h"

Adafruit_PWMServoDriver g_pwm = Adafruit_PWMServoDriver();
int g_hexaPos[12];

int hexaServoMap( int index )
{
  switch( index )
  {
    case 0:
      return _SERVO0_;
    case 1:
	    return _SERVO1_;
    case 2:
	    return _SERVO2_;
    case 3:
	    return _SERVO3_;
    case 4:
	    return _SERVO4_;
    case 5:
	    return _SERVO5_;
    case 6:
	    return _SERVO6_;
    case 7:
	    return _SERVO7_;
    case 8:
	    return _SERVO8_;
    case 9:
	    return _SERVO9_;
    case 10:
      return _SERVO10_;
    case 11:
	    return _SERVO11_;
  }
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
  g_pwm.begin();
  g_pwm.setPWMFreq( _PWMFREQ_ );
  hexaHoming();
}

void hexaHoming()
{
  int i;

  for( i = 0; i < 12; i++ )
  {
	  g_pwm.setPWM( hexaServoMap( i ), 0, hexaServoHome( i ) );
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
    g_pwm.setPWM( hexaServoMap( index ), 0, hexaServoHome( index ) + pos );
    g_hexaPos[ index ] = hexaServoHome( index ) + pos;
  }
  else
  {
    g_pwm.setPWM( hexaServoMap( index ), 0, hexaServoHome( index ) - pos );
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

