#pragma config(Sensor, dgtl1,  frontUltrasonic, sensorSONAR_cm)
#pragma config(Sensor, dgtl3,  frontRightQuad, sensorQuadEncoder)
#pragma config(Motor,  port2,           frontRightMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           frontLeftMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           backRightMotor, tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port5,           backLeftMotor, tmotorServoContinuousRotation, openLoop, reversed)

//potentially potentiometer



/*
vacuumWidth is on the coordinate plane where leftmost = 0 and rightmost = vacuumWidth
0-------------------(vacuumWidth)
*/

int controlMode = 0; //modes:: driver control = 0; vacuuming = 1

task control_listener(){
	while(true){
		if(vexRT[Btn8U] == 1)
			controlMode = 0;
		if(vexRT[Btn8R] == 1)
			controlMode = 1;
		wait1Msec(10);
	}
}
void crabDrive(bool driveRight){
	if(driveRight){
		startMotor(frontRightMotor, 127);
		startMotor(backRightMotor, -127);
		startMotor(frontLeftMotor, 127);
		startMotor(backLeftMotor, -127);
		} else{
		startMotor(frontRightMotor, -127);
		startMotor(backRightMotor, 127);
		startMotor(frontLeftMotor, -127);
		startMotor(backLeftMotor, 127);
	}
}

void driveForward(int speed){ //says forward but really will accept any standard(-127 to 127) input
	startMotor(frontRightMotor, speed);
	startMotor(backRightMotor, speed);
	startMotor(frontLeftMotor, speed);
	startMotor(backLeftMotor, speed);
}

task main(){

	
	int vacuumWidth; //horizontal(left, right) distance to vacuum
	int vacuumDistance = 45; //vertical(forward) distance to vacuum, PLACEHOLDER VALUE
	bool vacuumingRight; //indicates the direction to vacuum sweep
	startTask(control_listener);

	while(true){
		if(controlMode == 0){ //driver control
			if(vexRT[Btn6U] == 1){ //crab-drive right
				crabDrive(true);
				} else if(vexRT[Btn5U] == 1){ //crab-drive left
				crabDrive(false);
				} else { //tank driving
				startMotor(frontRightMotor, vexRT[Ch2]);
				startMotor(backRightMotor, vexRT[Ch2]);
				startMotor(frontLeftMotor, vexRT[Ch3]);
				startMotor(backLeftMotor, vexRT[Ch3]);
			}
			wait1Msec(1);
		}
		if(controlMode == 1){
			vacuumWidth = 0; //clear history
			SensorValue[frontRightQuad] = 0; //clear encoder
			vacuumingRight = false; //reset direction

			while(vexRT[Btn6U] == 0) { //wait for button press
				wait1Msec(1);
			}
			crabDrive(true);//start crab-driving right
			stopTask(control_listener); //disable mode switching

			while(vexRT[Btn6U] == 1){ //wait for button release
				wait1Msec(1);
			}
			vacuumWidth = SensorValue[frontRightQuad]; //set crab-driving distance to encoder value
			while(SensorValue[frontUltrasonic] > 5){ //while 5 cm away from front obstacle

				int distanceSave = SensorValue[frontRightQuad]; //save encoder position to account for inaccurate driving
				driveForward(50); //drive forward
				while(SensorValue[frontRightQuad] - distanceSave < vacuumDistance){ //wait until distance has been driven
					wait1Msec(1); }
				driveForward(0); //stop driving forward
				SensorValue[frontRightQuad] = distanceSave; //reset encoder to last value

				if(vacuumingRight){ //crab drive right
					crabDrive(true);
					while(SensorValue[frontRightQuad] > 0){ //wait until at 0 position
						wait1Msec(1); }
					driveForward(0); //stop driving
					} else{ //crab drive left
					crabDrive(false);
					while(SensorValue[frontRightQuad] < vacuumWidth){ //wait until at (vacuumWidth) position
						wait1Msec(1); }
					driveForward(0); //stop driving
				}
				vacuumingRight = !vacuumingRight; //switch directions for next time
			}
			controlMode = 0;
			startTask(control_listener);
		}
	}

}
