// Include Guard
#ifndef MOTOR_H
#define MOTOR_H

// Declaration
extern const float pi;
extern float motorNow[4];
extern int pStep;
extern bool motorSerial;

// Function
// void move(float, int);
// void turn(bool, int);
void motorSetup(int, int, int, int, int);
void motor(float, float, float, float, int);
void motorP(int, int, int, int, int);

#endif