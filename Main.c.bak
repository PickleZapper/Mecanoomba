//The Mecanoomba

//motors
Motor_3_Wire(port 2) frontRightMotor
Motor_3_Wire(port 3) frontLeftMotor
Motor_3_Wire(port 4) backRightMotor
Motor_3_Wire(port 5) backLeftMotor

//digital sensors
Ultrasonic_Sensor_CM(port 1, port 2) frontUltraSonic
Quad_Encoder(port 3, port 4) frontRightQuad

//analog sensors
//potentially potentiometer



/*
	vaccumnWidth is on the coordinate plane where leftmost = 0 and rightmost = vaccumnWidth
	0-------------------(vaccumnWidth)
*/

task control_listener(){
	while(true){
		if(vexRT[Btn8U == 1)
			mode = 0;
		if(vexRT[Btn8R] == 1)
			mode = 1;
		wait1Msec(10);
	}
}
void crabDrive(boolean driveRight){
	if(driveRight){
		startMotor(frontRightMotor, 127)
		startMotor(backRightMotor, -127)
		startMotor(frontLeftMotor, 127)
		startMotor(backLeftMotor, -127)
	} else{
		startMotor(frontRightMotor, -127)
		startMotor(backRightMotor, 127)
		startMotor(frontLeftMotor, -127)
		startMotor(backLeftMotor, 127)
	}
}

void driveForward(int speed){ //says forward but really will accept any standard(-127 to 127) input
	startMotor(frontRightMotor, speed)
	startMotor(backRightMotor, speed)
	startMotor(frontLeftMotor, speed)
	startMotor(backLeftMotor, speed)
}

task main(){
	
	int controlMode = 0; //modes:: driver control = 0; vaccumning = 1
	int vaccumnWidth; //horizontal(left, right) distance to vaccumn
	int vaccumnDistance = 45; //vertical(forward) distance to vaccumn, PLACEHOLDER VALUE
	boolean vaccumningRight; //indicates the direction to vaccumn sweep
	StartTask(control_listener);
	
	while(true){
		if(controlMode == 0){ //driver control
			if(vexRT[Btn6U] == 1){ //crab-drive right
				crabDrive(true);
			} else if(vexRT[Btn5U] == 1){ //crab-drive left
				crabDrive(false);
			} else { //tank driving
				startMotor(frontRightMotor, vexRT[Ch2])
				startMotor(backRightMotor, vexRT[Ch2])
				startMotor(frontLeftMotor, vexRT[Ch3])
				startMotor(backLeftMotor, vexRT[Ch3])
			}
			wait1Msec(1);
		}
		if(controlMode == 1){
			vaccumnWidth = 0; //clear history
			SensorValue[frontRightQuad] = 0; //clear encoder
			vaccumningRight = false; //reset direction
			
			while(vexRT[Btn6U] == 0) { //wait for button press
				wait1Msec(1);
			}
			crabDrive(true);//start crab-driving right
			stopTask(control_listener); //disable mode switching
			
			while(vexRT[Btn6U] == 1){ //wait for button release
				wait1Msec(1);
			}
			vaccumnWidth = SensorValue[frontRightQuad]; //set crab-driving distance to encoder value
			while(SensorValue[frontUltrasonic] > 5){ //while 5 cm away from front obstacle
			
				int distanceSave = SensorValue[frontRightQuad]; //save encoder position to account for inaccurate driving
				driveForward(50); //drive forward
				while(SensorValue[frontRightQuad] - distanceSave < vaccumningDistance){ //wait until distance has been driven
					wait1Msec(1); }
				driveForward(0); //stop driving forward
				SensorValue[frontRightQuad] = distanceSave; //reset encoder to last value
				
				if(vaccumingRight){ //crab drive right
					crabDrive(true);
					while(SensorValue[frontRightQuad] > 0){ //wait until at 0 position
						wait1Msec(1); }
					driveForward(0); //stop driving
				} else{ //crab drive left
					crabDrive(false);
					while(SensorValue[frontRightQuad] < vaccumnWidth){ //wait until at (vaccumnWidth) position
						wait1Msec(1); }
						driveForward(0); //stop driving
				}
				vaccumningRight = !vaccumningRight; //switch directions for next time
			}
			controlMode = 0; 
			startTask(control_listener);
		}
	}
	
}


