#include <motorWrappers.h>

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 250

#define MOTOR_MIN_SPEED -100
#define MOTOR_MAX_SPEED 100

void setMotorSpeed(polcore* motor, int speed) {
	if (speed<MOTOR_MIN_SPEED) {
		speed = MOTOR_MIN_SPEED;
	} else if (speed>MOTOR_MAX_SPEED) {
		speed = MOTOR_MAX_SPEED;
	}

	pconGenMotorGo( motor, pcon_buffer_to_dev, abs(speed),
					speed>0 ? PCON_FORWARD : PCON_BACKWARD);
	pconSendCommandBuffer( motor);
}

void setServoAngle(PololuMSC* pololu, uint8_t pololuId, uint8_t angle) {
	if (angle<SERVO_MIN_ANGLE) {
		angle = SERVO_MIN_ANGLE;
	} else if (angle>SERVO_MAX_ANGLE) {
		angle = SERVO_MAX_ANGLE;
	}

	pololu->setPosition(pololuId, angle);
}
