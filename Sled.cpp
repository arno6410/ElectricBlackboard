// Sled.cpp
#include "Sled.h"
#include <math.h>

Sled::Sled(float x, float y, float r, int rsize){
  x_position = 0.14+0.029;
  y_position = 0.1;
  x_board = x;
  y_board = y;
  radius = r;
  rotation_size = rsize;
  d1A = 0;
  d1B = 0;
  d2A = 0;
  d2B = 0;
}

// void Sled::MoveSteppers(){

// }

float Sled::CalculateRopeLength(float point_x, float point_y, int stepper_number){ //Calculate length of rope from spool to one point
  float stepper_x;
  if(stepper_number == 1){
    stepper_x = 0;
  }
  else{
    stepper_x = x_board;
  }
  float stepper_y = 0;

  float stepper_distance_straight = sqrt(pow((point_x-stepper_x),2)+pow((point_y),2));
  float a = asin((point_y)/stepper_distance_straight);
  float b = asin((radius/stepper_distance_straight));
  float rope_angle = a + b;

  float rope_around_spool = rope_angle * radius;
  float rope_straight = sqrt(pow(stepper_distance_straight,2) - pow(radius,2));
  float rope_length = rope_around_spool + rope_straight;

  return rope_length;
}

void Sled::Update(float x_destination, float y_destination){
  d1A = Sled::CalculateRopeLength(x_position, y_position, 1);
  // Serial.print("d1A: ");
  // Serial.println(d1A);
  d1B = Sled::CalculateRopeLength(x_destination, y_destination, 1);
  // Serial.print("d1B: ");
  // Serial.println(d1B);
  d2A = Sled::CalculateRopeLength(x_position, y_position, 2);
  // Serial.print("d2A: ");
  // Serial.println(d2A);
  d2B = Sled::CalculateRopeLength(x_destination, y_destination, 2);
  // Serial.print("d2B: ");
  // Serial.println(d2B);
}

long Sled::CalculateSteps(int stepper_number){
  float steps_float;
  if(stepper_number == 1){
    steps_float = (d1A-d1B)*rotation_size/(2*M_PI*radius); //Omgedraaid omdat ze niet in de juiste richting draaiden
  }
  else{
    steps_float = (d2B-d2A)*rotation_size/(2*M_PI*radius);
  }
  
  long steps = (long) steps_float;
  return steps;
}

void Sled::SetPosition(float x, float y){
  x_position = x;
  y_position = y;
}

float Sled::GetXPosition(){
  return x_position;
}

float Sled::GetYPosition(){
  return y_position;
}