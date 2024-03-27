// Arduino Include
#include <Arduino.h>

// Header Include
#include "camera.h"

#include <Wire.h>
#include <PixyI2C.h>

#define CAMERA_WAIT_MS 5

// Constants
const short int camXMin[3] = {0, 106, 213};
const short int camXMax[3] = {106, 213, 320};
// const int camYMin[3] = { 0, 66, 133 };
// const int camYMax[3] = { 66, 133, 200 };

// Variables
short int blockNum;
short int blockX;
short int blockY;
short int blockHeight;
short int blockWidth;
short int blockSignature;
short int blockPosition;

// Function
void cameraSetup(PixyI2C pixy, short int brightness)
{
  Serial.println("Pixy setup completed!");
  pixy.init();
  Serial.println("Pixy setup completed!");
}

int refreshCameraPosX(int camX)
{
  for (int x = 0; x < 3; x++)
  {
    if (camX < camXMin[x])
      continue;
    if (camXMax[x] < camX)
      continue;
    return (x + 1);
  }
}

void refreshCamera(PixyI2C pixy)
{
  uint16_t blocks;

  blocks = pixy.getBlocks();
  blockNum = blocks;

  if (blockNum == 0)
  {
    blockX = 0;
    blockY = 0;
    blockHeight = 0;
    blockWidth = 0;
    blockSignature = 0;
    blockPosition = 0;
    return;
  }

  // pixy.blocks[0].print();
  blockX = pixy.blocks[0].x;
  blockY = pixy.blocks[0].y;
  blockHeight = pixy.blocks[0].height;
  blockWidth = pixy.blocks[0].width;
  blockSignature = pixy.blocks[0].signature;
  blockPosition = refreshCameraPosX(blockX);

  return;
}