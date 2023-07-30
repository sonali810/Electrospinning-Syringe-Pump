// Include the AccelStepper library:
#include <AccelStepper.h>
#include <math.h>

// Define number of steps per revolution:
const int stepsPerRevolution = 200;

// Give the motor control pins names:
#define pwmA 3
#define pwmB 11
#define brakeA 9
#define brakeB 8
#define dirA 12
#define dirB 13

// Define the AccelStepper interface type:
#define MotorInterfaceType 2

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(MotorInterfaceType, dirA, dirB);

// Define variable relationships
float vflow = 0; // mL/hr
float Area = M_PI * pow((0.01489 / 2), 2); // m^2
float velocity = 0; // m
float pitch = 5*pow(10,(-3)); // m
float rpm = 0;
float volume = 0;
char receivedChar;
boolean newData = false;

void setup() {
  // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);
  pinMode(6,OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);
  digitalWrite(6,LOW);

  // Set the maximum steps per second:
  stepper.setMaxSpeed(600);
  Serial.begin(9600);
  
  Serial.println("Enter the volume that needs to be dispensed in mL: ");
  while (volume == 0){
    if(Serial.available()==0) {
      volume = Serial.parseFloat();
    }
  }
  Serial.println(volume);
  
  Serial.println("Enter the volumetric flow rate in mL/hr. Press 'Enter' to start the process: ");
  while (vflow ==0){
    if(Serial.available()> 0) {
      vflow = Serial.parseFloat();
    }
  }
  Serial.println(vflow);
  
  rpm = Calculations(vflow);
  //Serial.println(rpm);
 
  Serial.println("Once the process is finished, type 'n' to stop.");
  }


void loop() {
  recvOneChar();
  stepper.setSpeed(rpm);
  stepper.runSpeed();

  if(rpm !=0){
    digitalWrite(6,HIGH);
  }

  
  if (receivedChar == 'n' && newData == true) {
    digitalWrite(6,LOW);
    rpm = 0;
    stepper.setSpeed(rpm);
    stepper.runSpeed();
    stepper.stop();
    
  }

  showNewData();
 }

float Calculations(float vflow){
  vflow = vflow * (pow(10,-3)) / ((1000*pow(60,2))); // unit conversion to m^3/s
  //Serial.println(vflow);
  velocity = vflow / Area; // m/s
  //Serial.println(velocity);
  rpm = (velocity * (1/pitch) * 60)*1.63; // rpm
  //Serial.println(rpm);
  return rpm;
}

void recvOneChar() {
    if (Serial.available() > 0) 
    {
        receivedChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) 
    {
      Serial.println(receivedChar);
      newData = false;
    }
}
