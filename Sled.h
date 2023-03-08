// Sled.h
#ifndef SLED_H
#define SLED_H

#include <Arduino.h>

class Sled {
  public:
    Sled(float x, float y, float r, int rsize);
    //void MoveSteppers();
    float CalculateRopeLength(float point_x, float point_y, int stepper_number);
    void Update(float destination_x, float destination_y);
    long CalculateSteps(int stepper_number);
    void SetPosition(float x, float y);
    float GetXPosition();
    float GetYPosition();
  private:
    float x_position;
    float y_position;
    float x_board;
    float y_board;
    float radius;
    int rotation_size;
    float d1A;
    float d1B;
    float d2A;
    float d2B;
};

#endif