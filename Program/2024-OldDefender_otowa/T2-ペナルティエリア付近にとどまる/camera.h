// Include Guard
#ifndef CAMERA_H
#define CAMERA_H

#include <Wire.h>
#include <Pixy2I2C.h>

// Declaration
extern const short int camXMin[3];
extern const short int camXMax[3];

extern short int blockNum;
extern short int blockX;
extern short int blockY;
extern short int blockHeight;
extern short int blockWidth;
extern short int blockSignature;
extern short int blockPosition;

// Function
void cameraSetup(Pixy2I2C, short int);
int refreshCameraPosX(int);
void refreshCamera(Pixy2I2C);

#endif