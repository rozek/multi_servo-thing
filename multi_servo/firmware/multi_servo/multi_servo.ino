#include <osap.h>
#include <Servo.h>

#define PIN_LED_R 18
#define PIN_LED_G 19
#define PIN_LED_B 20

  int AnalogIn[4]  = { 26,27,28,29 };
  int PIN_Servo[8] = { 0,1,2,3,4,5,6,7 };

  OSAP_Runtime osap;
  OSAP_Gateway_USBSerial serLink(&Serial);
  OSAP_Port_DeviceNames namePort("multi_servo");

/**** RGB Control (RGB LED on Tiny2040 is "active low"!) ****/

  void _setRGB (uint8_t* Data, size_t Length) {
    analogWrite(PIN_LED_R, 65535-(Length < 2 ? 0 : Data[0] + Data[1]*255));
    analogWrite(PIN_LED_G, 65535-(Length < 4 ? 0 : Data[2] + Data[3]*255));
    analogWrite(PIN_LED_B, 65535-(Length < 6 ? 0 : Data[4] + Data[5]*255));
  }
  OSAP_Port_Named setRGB("setRGB",_setRGB);

/**** Analog Input ****/

  size_t _getAnalog (uint8_t* Data, size_t Length, uint8_t* Response) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 3)) {
        uint16_t Value = analogRead(AnalogIn[Port]);
          Response[0] = Value & 0xFF;
          Response[1] = Value >> 8 & 0xFF;
        return 2;
      }
    }
    return 0;
  }
  OSAP_Port_Named getAnalog("getAnalog",_getAnalog);

/**** Servo Control ****/

  Servo ServoList[8];
  void _setServo (uint8_t* Data, size_t Length) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 7)) {
        ServoList[Port].write(Length == 1 ? 0 : Data[1]);
      }
    }
  }
  OSAP_Port_Named setServo("setServo",_setServo);

/**** Startup ****/

  void setup() {
    osap.begin();

    analogWriteResolution(16);             // according to RP2040 specifications
    analogReadResolution(12);                                            // dto.

    pinMode(PIN_LED_R,OUTPUT);
    pinMode(PIN_LED_G,OUTPUT);
    pinMode(PIN_LED_B,OUTPUT);

    analogWrite(PIN_LED_R,65535);              // initially switches the LED off
    analogWrite(PIN_LED_G,65535);                                        // dto.
    analogWrite(PIN_LED_B,65535);                                        // dto.

    for (int Port = 0; Port < 4; Port++) {
      analogRead(AnalogIn[Port]);
    }

    for (int Port = 0; Port < 8; Port++) {
      ServoList[Port].attach(PIN_Servo[Port],700,2700); // for SG90 micro servos
    }
  }

/**** Operation ****/

  void loop() {
    osap.loop();
  }