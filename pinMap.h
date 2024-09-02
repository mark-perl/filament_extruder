#include <Arduino.h>

// TODO: Check pinouts correct with uno unit
// TODO: What pins are required for calipers

// ###### Left Side ######
// IOREF
// RESET
// V3.3
// V5
// GND
// GND
// VIN

const int Pot_Dial_1 = A0;  // Tensioner Speed
const int Pot_Dial_2 = A1;  // 
const int Pot_Dial_3 = A2;
const int UNUSED = A3;
const int Display_Clock = A4;
const int Display_Data = A5;

// ###### Right Side ######
const int UNUSED = SCL; //A5
const int UNUSED = SDA; //A4
// AREF
// GND
const int Tranistor_1 = 13; // Fan power switch
const int UNUSED = 12;
const int UNUSED = 11;
const int UNUSED = 10;
const int Switch_1 = 9;     // (Feeder limit switch)
const int Switch_2 = 8;     // Manual/Auto control toggle

const int Z_Dir = 7;
const int Y_Dir = 6;
const int X_Dir = 5;
const int Z_Step = 4;
const int Y_Step = 3;
const int X_Step = 2;
const int Encoder_1 = 1;    // (Spool encoder)
const int Encoder_2 = 0;    // (Spool encoder)