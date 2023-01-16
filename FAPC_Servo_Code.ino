// Autonomous Parking Car
//Adding a library
#include <AFMotor.h>
#include <Servo.h>
#include <Ultrasonic.h>

// definitions of motors
AF_DCMotor Left_Front_Motor(4);
AF_DCMotor Right_Front_Motor(3);
AF_DCMotor Left_Back_Motor(1);
AF_DCMotor Right_Back_Motor(2);


// definitions of servo motors
Servo engine1;
Servo engine2;
Servo engine3;
Servo engine4;
// definitions of ultrasonic sensors
Ultrasonic ultrasonic_back(40,41),ultrasonic_left_back(38,39),ultrasonic_left_front(36,37),ultrasonic_front(34,35);

 
#define Left 0 //left direction command
#define Right 1 //right direction command
#define Forward 2 //forward command
#define Back 3 //reverse command
#define minimum_limit 15 //width of the car (cm)
#define minimum_limit1 28 //length of the car (cm)
byte park_state = 0;
int signalpin = 21;
volatile int val;
int painValue=90;
 
int counter = 0;
int current_state = 0;
int previous_state = 0;
 
void say(int saydir)
{
for (int i = 0 ; i <=saydir; i+1)
{
val = digitalRead(signalpin);
if (val == LOW) {
 
current_state = 0;
}
else {
 
current_state = 1;
}
 
if(current_state != previous_state)
{
if(current_state == 1)
{
counter = counter + 1;
Serial.println(counter);
i = i+1;
}
else
{
i = i ;
}
 
previous_state = current_state;
 
}
if (i == say)
{
 
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
 
}
 
}
 
}
 
void motor_pinSetup()
{
 
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
}
 
// Motion functions
void Robot_Move(byte engine, byte spd)
{
if (engine == Forward)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(FORWARD);
Right_Front_Motor.run(FORWARD);
Left_Back_Motor.run(FORWARD);
Right_Back_Motor.run(FORWARD);
 
}
if (engine == Back)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(BACKWARD);
Right_Front_Motor.run(BACKWARD);
Left_Back_Motor.run(BACKWARD);
Right_Back_Motor.run(BACKWARD);
 
}
if (engine == Left)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(BACKWARD);
Right_Front_Motor.run(FORWARD);
Left_Back_Motor.run(BACKWARD);
Right_Back_Motor.run(FORWARD);
 
}
 
if (engine == Right)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(FORWARD);
Right_Front_Motor.run(BACKWARD);
Left_Back_Motor.run(FORWARD);
Right_Back_Motor.run(BACKWARD);
 
}
 
}
 
void Robot_Stop()
{
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
}
 
// Search for parking space
bool Parking_Space_Control()
{
 
long front_Sensor = ultrasonic_front.Ranging(CM);
long left_Sensor = ultrasonic_left_front.Ranging(CM);
long left_back_Sensor =ultrasonic_left_back.Ranging(CM);
 
if( (left_Sensor <= minimum_limit)&&(left_back_Sensor <= minimum_limit)&&(park_state == 0))
{
Robot_Move(Forward, 100);
park_state = 1; Serial.println(park_state);
}
 
if((left_Sensor > minimum_limit)&&(left_Sensor < minimum_limit1)&&(left_back_Sensor > minimum_limit)&&(left_back_Sensor < minimum_limit1)&&(park_state == 1))
{
Robot_Move(Forward, 100);
park_state = 2;Serial.println(park_state);
}
 
if((left_Sensor >= minimum_limit1)&&(left_back_Sensor >= minimum_limit1)&&(park_state == 1))
{
/* Vertical Parking Decision */
Robot_Stop() ;
delay(500);
engine1.write(0);
engine2.write(0);
engine3.write(180);
engine4.write(180);
park_state = 3;Serial.println(park_state);
}
 
if((left_Sensor <= minimum_limit)&&(left_back_Sensor <= minimum_limit)&&(park_state == 2))
{
/* Parallel Parking Decision */
park_state = 3; Serial.println(park_state);
}
 
return park_state;
}
 
void Park_find()
{
Parking_Space_Control();
if(park_state == 3 )
{
Robot_Stop();Serial.println(park_state);
delay(400);
park_state = 4;
}
if(park_state == 4 )
{
 
Robot_Move(Back,120);
say(18);
Robot_Stop();Serial.println(park_state);
delay(500);
Robot_Move(Right,150);
say(9);
Robot_Stop();
delay(500);
park_state = 5;
}
if(park_state == 5)
{
 
Robot_Move(Back,120);
long back_Sensor = ultrasonic_back.Ranging(CM);Serial.println(back_Sensor);
 
if(back_Sensor>0 && back_Sensor <= 13)
{
Robot_Stop();
delay(400);
park_state = 6;
}
return ultrasonic_back;
}
 
if(park_state == 6)
{
Robot_Move(Left,150);
long left_Sensor = ultrasonic_left_front.Ranging(CM); Serial.println(left_Sensor);
long left_Back_Sensor = ultrasonic_left_back.Ranging(CM); Serial.println(left_Back_Sensor);
 
if(left_Sensor == left_Back_Sensor)
{
Robot_Stop();
park_state = 7;
}
 
return left_Sensor,left_Back_Sensor;
}
if(park_state == 7)
{
long front_Sensor = ultrasonic_front.Ranging(CM);
 
if(front_Sensor<=6)
{
Robot_Stop();
park_state = 8;
}
else
{
Robot_Move(Forward,100);
}
return ultrasonic_front;
}
if (park_state ==10)
{
 
Robot_Move(Left,180);
say(14);
Robot_Stop();
delay(500);
park_state = 7;
 
}
 
}
 
void setup()
{
Serial.begin(9600);
attachInterrupt(5, say, CHANGE);
pinMode (signalpin, INPUT) ;
engine1.attach(26);
engine2.attach(28);
engine3.attach(30);
engine4.attach(32);
motor_pinSetup();
}
 
void loop()
{
Park_find();
}
