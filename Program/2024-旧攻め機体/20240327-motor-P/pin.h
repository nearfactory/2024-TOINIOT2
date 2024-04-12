// Include Guard
#ifndef PIN_H
#define PIN_H

// Declaration
extern const unsigned short int linePin[22];
extern const unsigned short int ballPin[16];
extern const unsigned short int motorPin[4][2];
extern const unsigned short int ledPin[3];
extern const unsigned short int btnPin[4];
extern const unsigned short int bzPin;
extern const unsigned short int dirPin[2];

// Function
void pinSetup(void);
void waitBtn(int);

#endif