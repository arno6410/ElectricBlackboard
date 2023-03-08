/*
    Controlling two stepper with the AccelStepper library

     by Dejan, https://howtomechatronics.com
*/

#include <AccelStepper.h>
#include <MultiStepper.h>
#include "Sled.h"
#include <math.h>

// use 1/16 steps! 200 steps per revolution -> 3200 steps per revolution
// Origin in upper left corner

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 2, 5); // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, 3, 6);
MultiStepper steppers_control;  // Create instance of MultiStepper
long go_to_position[2]; // An array to store the target position (x and y coordinates)
unsigned long t1;
unsigned long t2;
unsigned long myTime;
int rotation_size = 3200;
float radius = 0.015; //[m] 
float x_board = 0.8; //[m]
float distance_stepper_edge = 0.008 + 0.021; //AFSTAND TUSSEN STEPPER AS EN BORD, 8mm rand + 21 mm naar as van stepper
float y_board = 1; //[m]
Sled krijtje(x_board + 2*distance_stepper_edge, y_board, radius, rotation_size); //Create sled object

//for the serial comm
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
float float_PC_1 = 0.0;
float float_PC_2 = 0.0;


boolean new_data = false;


void setup() {
  stepper1.setMaxSpeed(3000); // Set maximum speed value for the stepper
  stepper1.setAcceleration(100000); // Set acceleration value for the stepper
  stepper1.setCurrentPosition(0); // Set the current position to 0 steps

  stepper2.setMaxSpeed(3000); // Set maximum speed value for the stepper
  stepper2.setAcceleration(100000); // Set acceleration value for the stepper
  stepper2.setCurrentPosition(0); // Set the current position to 0 steps

  steppers_control.addStepper(stepper1);
  steppers_control.addStepper(stepper2);

  Serial.begin(9600); //9600 baud rate
}

void loop() {
  if(Serial.available() > 0) {

  //=====This works=======
  // int value = Serial.parseInt();  
  //   delay(1000);
    
  //   if(value==1){ //horizontaal
  //     MoveStraight(0.34+0.029,0.1,krijtje,steppers_control);
  //     delay(2000);
  //     MoveStraight(0.14+0.029,0.1,krijtje,steppers_control);
  //   }
  //   else if(value==2){ //verticaal
  //     MoveStraight(0.1+0.029,0.23,krijtje,steppers_control);
  //     delay(2000);
  //     MoveStraight(0.1+0.029,0.13,krijtje,steppers_control);
  //   }
  //   else if(value==3){ //diagonaal
  //     MoveStraight(0.3+0.029,0.2,krijtje,steppers_control);
  //     delay(2000);
  //     MoveStraight(0.1+0.029,0.1,krijtje,steppers_control);
  //   }
  //   else if(value==4){ //diagonaal nieuwe functie
  //     StraightLine(0.3+0.029,0.2,krijtje,steppers_control,4);
  //     delay(2000);
  //     StraightLine(0.1+0.029,0.1,krijtje,steppers_control,4);
  //   }
  //   else if(value==5){ //diagonaal relative
  //     StraightRelative(0.2,0.1,krijtje,steppers_control,4);
  //     delay(2000);
  //     StraightRelative(-0.2,-0.1,krijtje,steppers_control,4);
  //   }
    
  // }  

  // while (Serial.available() == 0) {}     //wait for data available
  // String input = Serial.readString();  //read until timeout
  // input.trim();                        // remove any \r \n whitespace at the end of the String
  // if (input == "right") {
  //   StraightRelative(0.1, 0, krijtje, steppers_control, 1);)
  // } else if (input == "up") {
    
  // } else if (input == "left") {

  // } else if (input == "down") {

  // } else if (input == "move") {
    
  // }

  ReceiveData();
    if (new_data == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in ParseData() replaces the commas with \0
        ParseData();
        ShowParsedData();
        new_data = false;
    }
}

test


// ====================Functions==================== 
void MoveStraight(float x_destination, float y_destination, Sled krijtje, MultiStepper steppers_control) {
  long go_to_position[2];

  // Calculate number of steps
  krijtje.Update(x_destination, y_destination);
  long steps1 = krijtje.CalculateSteps(1);
  Serial.print("steps1: ");
  Serial.println(steps1);
  long steps2 = krijtje.CalculateSteps(2);
  Serial.print("steps2: ");
  Serial.println(steps2);
  int size = 100;

  //--------------
  go_to_position[0] = steps1;
  go_to_position[1] = steps2;

  steppers_control.moveTo(go_to_position);  
  steppers_control.runSpeedToPosition();
  //--------------

  krijtje.SetPosition(x_destination, y_destination);
}

//Move in a straight line using several intermediate points
void StraightLine(float x_destination, float y_destination, Sled krijtje, MultiStepper steppers_control, int n_points) {
  // Calculate intermediate points(delta)
  float total_x = x_destination - krijtje.GetXPosition();
  float total_y = y_destination - krijtje.GetYPosition();
  float delta_x = total_x / n_points;
  float delta_y = total_y / n_points;
  
  float current_destination[2] = {krijtje.GetXPosition() + delta_x, krijtje.GetYPosition() + delta_y};

  // Move to each point
  for (int i = 0; i < n_points; i++) {
    MoveStraight(current_destination[0],current_destination[1],krijtje,steppers_control);
    current_destination[0] += delta_x;
    current_destination[1] += delta_y;    
  } 
}

//Doesn't work yet
void StraightRelative(float rel_x_destination, float rel_y_destination, Sled krijtje, MultiStepper steppers_control, int n_points) {
  StraightLine(rel_x_destination+krijtje.GetXPosition(), rel_y_destination+krijtje.GetYPosition(), krijtje, steppers_control, n_points);
}

//Not finished yet
// void Arc(float x_destination, float y_destination, float r_center, Sled krijtje, MultiStepper steppers_control, int n_points) {
//   // Calculate angle between source and destination points
//   float x_midpoint = (x_destination + krijtje.GetXPosition())/2;
//   float y_midpoint = (y_destination + krijtje.GetYPosition())/2;
//   // 
// }



//Serial communication functions
void ReceiveData () {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char start_marker = '<';
  char end_marker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
        if (rc != end_marker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            recvInProgress = false;
            ndx = 0;
            newData = true;
        }
    }

    else if (rc == start_marker) {
        recvInProgress = true;
    }
  }
}

//============

void ParseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");
  float_PC_1 = atof(strtokIndx);     // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  float_PC_2 = atof(strtokIndx);     // convert this part to a float

}

void ShowParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Float 1 ");
    Serial.println(float_PC_1);
    Serial.print("Float 2 ");
    Serial.println(float_PC_2);
}
