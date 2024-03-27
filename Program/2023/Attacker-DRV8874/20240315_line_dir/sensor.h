// Include Guard
#ifndef SENSOR_H
#define SENSOR_H

// Varibales & Constants
extern float dir;
extern float dirPrev;
extern float dirOpponent;
extern bool dirCalibration;
extern unsigned short int line[22];
extern unsigned short int lineBlock[5];
extern unsigned short int ball[16];
extern float ballX;
extern float ballY;
extern float ballAngle;
extern int ballSum;
extern short int ballBig;

// Function
void dirSetup(void);
void refreshDirRaw(void);
void refreshDir(void);

void refreshLine(void);
void refreshLinePrint(void);

void refreshBall(void);
void refreshBallPrint(void);

#endif