#include "Motor.h"


MotorController::MotorController(PinName pwm,PinName CH1,PinName CH2,float p,float i,float d,char whichMotor,PinName en,int pls):motorPwm(pwm),dutyCycle(0.5f),encoder(CH1,CH2,NC,pls),P(p),I(i),D(d),name_(whichMotor),enable(en)
{
		targetSpeed = 0;
		target_=0;
		feed_=0;
		currentDiff_=0;
		lastDiff_=0;
		Pout=0;
		Iout=0;
		Dout=0;
		PIDout=0;
		IoutMax=99999999;
		PIDoutMax=10000;

		motorPwm.period_us(50);

		motorPIDTicker.attach(callback(this,&MotorController::TickerFunct),0.0008f);
	
	
		enable.write(1);
}

void MotorController::resIout(void)
{
	Iout = 0;
}
float MotorController::MotorPIDFun(float target,float feed)
{
		target_ = target;
		feed_ = feed;
		currentDiff_ = target_ - feed_;
	

		Pout = P * currentDiff_;

		Iout += I * currentDiff_;
		Iout = Iout > IoutMax ? IoutMax : Iout;
		Iout = Iout < -IoutMax ? -IoutMax : Iout;
	
		Dout = D * (currentDiff_-lastDiff_);

		lastDiff_ = currentDiff_;
	
		PIDout = (Pout + Iout + Dout);

		PIDout = PIDout > PIDoutMax ? PIDoutMax : PIDout;
		PIDout = PIDout < -PIDoutMax ? -PIDoutMax : PIDout;
		return (float)PIDout/(float)PIDoutMax/2.0f;
}

void MotorController::TickerFunct(void)
{
		
	dutyCycle +=  MotorPIDFun(targetSpeed,encoder.getRotationSpeed());
		
	
		dutyCycle  =dutyCycle>LIMIT?LIMIT:dutyCycle;
	dutyCycle=dutyCycle<(1-LIMIT)?(1-LIMIT):dutyCycle;
		motorPwm.write(dutyCycle);

		

}
void MotorController::SetTargetSpeed(float x)
{
		targetSpeed = x;
}
void MotorController::encoderReset(void)
{
	encoder.reset();
}

float MotorController::getRotationSpeed(void)
{
	return encoder.getRotationSpeed();
}
char MotorController::getName(void)
{
	return name_;
}
float MotorController::getDuty(void)
{
	return dutyCycle;
}
