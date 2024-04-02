// Include Guard
#ifndef MOTOR_H
#define MOTOR_H

// Declaration
extern float motorNow[4];
extern short int pStep;
extern short int M1;
extern short int M2;
extern short int M3;
extern short int M4;

// Function
void motorSetup(int, short int, short int, short int, short int);
void motorRaw(float, float, float, float, int);
void motor(float, float, float, float, int);
void motorP(int, int, int, int, int);

#endif