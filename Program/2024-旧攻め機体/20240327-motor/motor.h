// Include Guard
#ifndef MOTOR_H
#define MOTOR_H

// Declaration
extern float mpGoal[4];
extern float mpGoalPrev[4];
extern float mpOutput[4];
extern short int M1;
extern short int M2;
extern short int M3;
extern short int M4;

// Function
void motorSetup(short int, short int, short int, short int, short int);
void move(void);

void mpCalc(float, float, float, float);

#endif