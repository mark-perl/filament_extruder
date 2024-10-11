#pragma once
#include <Arduino.h>

// ###### Left Side ######
const int SW_Limit = 13;
// V3.3
// AREF
const int Encoder_Dial_A = A0;
const int Encoder_Dial_B = A1;
const int Encoder_Spooling_A = A2;
const int Encoder_Spooling_B = A3;
const int I2C_Data = SDA;  // A4
const int I2C_Clock = SCL; // A5
const int SW_Select = A6;
const int SW_Enter = A7;
// 5V
// GND
// RESET
// VIN

// ###### Right Side ######
const int SW_Manual = 12;
const int SW_Auto = 11;
const int Fan_1 = 10;
const int Fan_2 = 9;   
const int Fan_3 = 8;     
const int M3_Dir = 7;  // spooler
const int M2_Dir = 6;  // feeder
const int M1_Dir = 5;  // tensioner
const int M3_Step = 4; // spooler
const int M2_Step = 3; // feeder
const int M1_Step = 2; // tensioner
// GND
// RESET