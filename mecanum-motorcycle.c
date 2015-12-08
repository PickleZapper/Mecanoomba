//motors


int drivemode = 0; //0 for tank, 1 for arcade
float magnitude;
float direction;

task drivemode_toggle_listener(){
	while(true){
		if(vexRT[Btn8D] == 1){
			drivemode = abs(drivemode - 1);
		}
		while(vexRT[Btn8D] == 1){
			wait1Msec(50);
		}
		wait1Msec(50);
	}
}

task main(){
	while(true){
		if(drivemode == 0){
			motor[frontWheel] = vexRT[Ch2];
			motor[backWheel] = vexRT[Ch3];
		}
		if(drivemode == 1){
			direction = arctan(vexRT[Ch2] / vexRT[Ch3]);
			magnitude = sqrt(pow(vexRT[Ch1] * cos(direction), 2) + pow (vexRT[Ch2] * sin(direction), 2));
			motor[frontMotor] = magnitude * cos(direction - PI/4);
			motor[backMotor] = magnitude * sin(direction - PI/4);
		}
		wait1Msec(10);
	}
}