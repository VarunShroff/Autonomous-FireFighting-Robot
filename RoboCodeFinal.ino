/*
  Author : Varun Shroff
  Assignment : Autonomous Robot Code
  Date : 06-09-2019
  Description : Robot Code That Allows Our Robot To Extingish A Flame
*/

/*
                                    Algorithm Description
  Robot Will Move Forward Until Ultrasonic Sensor Detects An Obstacle Within 30cm.
  When Obstacle Is Detected, Robot Will Turn 90 Degrees Right And Collect The Distance of The Right Side,
  Robot Will Then Turn 90 Degrees Left And Collect The Distance of The Left Side.
  Robot Will Turn And Navigate In The Direction That Has A Greater Average Distance, It Will Than Process The Cycle
  Over And Over Again Untill It Detects The Flame.
  When The Flame Sensor Detects The Flame, It Will Activate The Fan To Extinguish The Flame.
*/

// Including Packages & Libraries
#include <Servo.h>

// Component Initialization
Servo RightWheel;
Servo LeftWheel;
Servo SensorServo;

// Global Variables
int pos = 0;    
int sensorValue = 0; // Flame Sensor Value
int posRight = 180;
int posLeft = 0;

// Sensor Digital Pins
const int UltraSonicPIN = 13;
const int EchoPIN = 8;
const int FanPin = 4;

// Distance Calculating Variables
double RightWallDistance[10];
double LeftWallDistance [10];
double AverageLeft;
double AverageRight;
double DistanceRight;
double DistanceLeft;

// Measurement Variables
long duration, cm;


void setup() // Setup Function
{
  Serial.begin(9600);
  pinMode(UltraSonicPIN, OUTPUT); // Output Initialised
  pinMode(EchoPIN, INPUT); // Input Initialised
  pinMode(FanPin, OUTPUT); // Fan Output Initialised
  RightWheel.attach(9); // Right Servo Pin
  LeftWheel.attach(7); // Left Servo Pin
  SensorServo.attach(2); // 180 Servo Pin

}

void loop() // Main Function
{

  // Moving Forward
  RightWheel.write(posRight); 
  LeftWheel.write(posLeft);

  FireDetection(); // Checks For Fire In The Background
  ReadDistance(); // Reads Distance In The Background

 // Conditional Statements Which Control Steering Of Robot
 
  if (cm < 30 ) { // If Robot Find Obstacle Within 30 cm, & No Flame Detected, It Will Re-Route
  
    TurnRight90();
    StopServo();
    ReadRightWall();
    StartServo();
    TurnLeft90();
    TurnLeft90();
    StopServo();
    ReadLeftWall();
    StartServo();
    TurnRight90();
    StopServo();

    if (AverageRight > AverageLeft) { // If Right Wall Distance > Left Wall Distance, Robot Will Go Right
      StartServo();
      TurnRight90();
      RightWheel.write(posRight);
      LeftWheel.write(posLeft);


    }

    else if (AverageLeft > AverageRight) { // If Left Wall Distance > Right Wall Distance, Robot Will Go Left
      StartServo();
      TurnLeft90();
      RightWheel.write(posRight);
      LeftWheel.write(posLeft);

    }

  }

}

long microsecondsToCentimeters(long microseconds) // Conversion Function
{
   /*
  The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
   object we take half of the distance travelled.
   */
  return microseconds / 29 / 2;
}

void ReadDistance() { // Read Distance Using Ultrasonic Sensor

  digitalWrite(UltraSonicPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(UltraSonicPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(UltraSonicPIN, LOW);

  duration = pulseIn(EchoPIN, HIGH); // Recieves Distance Between Obstacle

  cm = microsecondsToCentimeters(duration); // Converts Microseconds To Centimeters

if (cm > 1500) {
 cm = 0; // Used to neglect huge spikes in sensor
}
  Serial.print(cm); // Prints Out Cm
  Serial.print("cm");
  Serial.println();
  delay(10);


}

void TurnLeft90() { // Turns Robot 90 Degrees Left

  RightWheel.write(0);
  LeftWheel.write(0);
  delay(700);

}

void TurnRight90() { // Turns Robot 90 Degrees Right

  RightWheel.write(180);
  LeftWheel.write(180);
  delay(800);

}

void StopServo() { // Stops Servo (Time)

  RightWheel.detach(); // De-attaches Servos
  LeftWheel.detach();
  delay(600);

}
void StartServo() { // Re-attaches Servo & Starts Servos

  delay(600);
  RightWheel.attach(9);
  LeftWheel.attach(7);


}


int ReadRightWall() {

  // Reads Distance And Calculates Average Distance (Right Wall)
  ReadDistance();
  RightWallDistance[0] = cm;
  ReadDistance();
  RightWallDistance[1] = cm;
  ReadDistance();
  RightWallDistance[2] = cm;
  ReadDistance();
  RightWallDistance[3] = cm;
  ReadDistance();
  RightWallDistance[4] = cm;
  ReadDistance();
  RightWallDistance[5] = cm;
  ReadDistance();
  RightWallDistance[6] = cm;
  ReadDistance();
  RightWallDistance[7] = cm;
  ReadDistance();
  RightWallDistance[8] = cm;
  ReadDistance();
  RightWallDistance[9] = cm;

  for (int i = 0; i < 10; i++) { // Calculates Average Right Distance

    AverageRight += RightWallDistance[i];
    AverageRight = AverageRight / 10;

  }
  return AverageRight; // Returns AverageRight Distance

}

int ReadLeftWall() {

  // Reads Distance And Calculates Average Distance (Left Wall)
  ReadDistance();
  LeftWallDistance[0] = cm;
  ReadDistance();
  LeftWallDistance[1] = cm;
  ReadDistance();
  LeftWallDistance[2] = cm;
  ReadDistance();
  LeftWallDistance[3] = cm;
  ReadDistance();
  LeftWallDistance[4] = cm;
  ReadDistance();
  LeftWallDistance[5] = cm;
  ReadDistance();
  LeftWallDistance[6] = cm;
  ReadDistance();
  LeftWallDistance[7] = cm;
  ReadDistance();
  LeftWallDistance[8] = cm;
  ReadDistance();
  LeftWallDistance[9] = cm;

  for (int i = 0; i < 10; i++) { // Calculates Average Left Distance

    AverageLeft += LeftWallDistance[i];
    AverageLeft = AverageLeft / 10;

  }
  return AverageLeft; // Returns AverageLeft Distance

}

int FireDetection() { // Continously Tries To Find Fire


  for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
  {
    SensorServo.write(pos);
    sensorValue = analogRead(A2);
    Serial.println(sensorValue);
    delay(1); // Decreases Speed of Servo By Delaying Time

    if (sensorValue > 250) { // If Flame Is Detected, Extingusih Function is Called

    Extinguish(); // Calling Extinguish Function

  }
  else {
    RightWheel.write(posRight); 
    LeftWheel.write(posLeft);
    
  }
  }
  
  for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    SensorServo.write(pos);
    sensorValue = analogRead(A2);
    Serial.println(sensorValue);
    delay(1); // Decreases Speed Of Servo By Delaying Time

      if (sensorValue > 250) { // If Flame Is Detected, Extingusih Function is Called

    Extinguish(); // Calling Extinguish Function

  }
  else {
  RightWheel.write(posRight); 
  LeftWheel.write(posLeft);
    
  }


  }

}

void Extinguish() {   // Extinguishing Function Will Activate The Fan & Spin The Robot In 360 Degrees To Ensure The Flame Is Taken Out


  digitalWrite(FanPin, HIGH);
  RightWheel.write(180);
  LeftWheel.write(180);
  delay(6000);
  digitalWrite(FanPin, LOW);


}

