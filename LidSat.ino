
// For second UART
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

// bandwith in KHz ( 0= 12.5KHz or 1= 25KHz )
int bw = 1;

// squelch level  ( 0 - 8 ); 0 = "open" 
int squ = 1;

// rx frequency in MHz
float uplink = 145.7250;

// tx frequency in MHz
float downlink = 434.0000;

// 67.0Hz PL
String plTone = "0001";

// Routinely configure modules
unsigned long previousMillis = 0;
const long interval = 500;

// Reconfigure the SERCOM for a second serial port for the UHF module
Uart Serial2 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);

// Setup interrupts
void SERCOM1_Handler()
{
    Serial2.IrqHandler();
}

void setup()
{
    Serial1.begin(9600);
    Serial2.begin(9600);

    // Assign pins 10 & 11 SERCOM functionality
    pinPeripheral(10, PIO_SERCOM);
    pinPeripheral(11, PIO_SERCOM);

    // Setup PTT control pins
    pinMode(5, INPUT);
    pinMode(6, OUTPUT);

    // PTT inputs are inverted logic, put in RX
    digitalWrite(6, HIGH);

    delay(100);
    configureModules();
}

void loop()
{
  // Key on carrier detect
  digitalWrite(6, digitalRead(5));

  // Configure modules
  if (millis() - previousMillis >= interval)
  {
    previousMillis = millis();
    //configureModules();
  }
}

void configureModules()
{
  // Uplink
    delay(10);
    Serial1.print("AT+DMOSETGROUP=");
    Serial1.print(bw,1);
    Serial1.print(",");
    Serial1.print(uplink,4); 
    Serial1.print(",");
    Serial1.print(uplink,4);
    Serial1.print(",");
    Serial1.print(plTone);
    Serial1.print(",");
    Serial1.print(squ);
    Serial1.print(",");
    Serial1.println(plTone);

    Serial1.println("AT+DMOSETVOLUME=4");

    // Downlink
    delay(10);
    Serial2.print("AT+DMOSETGROUP=");
    Serial2.print(bw,1);
    Serial2.print(",");
    Serial2.print(downlink,4); 
    Serial2.print(",");
    Serial2.print(downlink,4);
    Serial2.print(",");
    Serial2.print("0000");
    Serial2.print(",");
    Serial2.print(squ);
    Serial2.print(",");
    Serial2.println("0000");
}

