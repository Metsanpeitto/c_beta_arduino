


/*Attach the Zero cross pin of the module to Arduino External Interrupt pin
Select the correct Interrupt # from the below table 
(the Pin numbers are digital pins, NOT physical pins: 
digital pin 2 [INT0]=physical pin 4 and digital pin 3 [INT1]= physical pin 5)
check: http://arduino.cc/en/Reference/attachInterrupt

Pin    |  Interrrupt # | Arduino Platform
-------------------------s--------------
2      |  0            |  All -But it is INT1 on the Leonardo
3      |  1            |  All -But it is INT0 on the Leonardo
18     |  5            |  Arduino Mega Only
19     |  4            |  Arduino Mega Only
20     |  3            |  Arduino Mega Only
21     |  2            |  Arduino Mega Only
0      |  0            |  eonardo
1      |  3            |  Leonardo
7      |  4            |  Leonardo
The Arduino Due haas no standard interrupt pins as an interrupt can be attached to almosty any pin. 
//_________________
--------pin 3  = 1;
--------pin 19 = 4;
--------pin 18 = 5;
In the program pin 2,3,18 are chosen
*/
int AC_LOAD0 = 47;    // Output to Opto Triac pin
int dimming0 = 0;  // Dimming level (0-128)  0 = ON, 128 = OFF
int AC_LOAD1 = 45;    // Output to Opto Triac pin
int dimming1 = 1;  // Dimming level (0-128)  0 = ON, 128 = OFF
int AC_LOAD2 = 43;    // Output to Opto Triac pin
int dimming2 = 8;  // Dimming level (0-128)  0 = ON, 128 = OFF

int AC_LOADArray[3] = {AC_LOAD0,AC_LOAD1,AC_LOAD2};
int dimmingArray[3] = {dimming0,dimming1,dimming2};

// PH and pumps -----------------------------------------

#define Offset 0.00            //deviation compensate
#define LED 13
const int pin4=45;
#define SensorPin 3            //pH meter Analog output to Arduino Analog Input 2
const int phUp = 46;
const int phDown = 48;
const int  micro = 50;
const int bloom = 52;

 int postDelay = 10;
 int minuteRef ;
 int hourRef;
 int lastPost ;
 int lastPostHour;
 int len;
 char inChar;
 char str[200]= "";
 char* arr = "";
 String inputString ="";
 boolean stringComplete;
 
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int phArray[ArrayLenth];   //Store the average value of the sensor feedback
int phArrayIndex=0;  
const float minph = 5.5;
const float maxph = 7.0; 
static float phValue,voltage;
float phValueOnCheck ; 
char* line = "";                                                  // This part receives the response line fromesp8266
//char message[] = "This is some long message that will end up scrolling";
int previous = 0;
int pos = 0;

#include <string.h>
#include <SPI.h>
#include <SD.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <OneWire.h>
#include <DS18B20.h>  
#include <SoftwareSerial.h>
//#include <Ethernet.h>
#include <Wire.h>                       // For some strange reasons, Wire.h must be included here
#include <DS1307new.h>
//#define SensorPin A8            //pH meter Analog output to Arduino Analog Input 2
#define DEBUG true
     
  SoftwareSerial esp8266(10,11); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                                 // This means that you need to connect the RX line from the esp to the Arduino's pin 2
                                 // and the TX line from the esp to the Arduino's pin 3


// # Product name: Analog EC Meter
// # Product SKU : DFR0300
// # Version     : 1.0

// # Description:
// # Sample code for testing the EC meter and get the data feedback from the Arduino Serial Monitor.

// # Connection:
// #        EC meter output     -> Analog pin 1
// #        DS18B20 digital pin -> Digital pin 2
// #

#include "OneWire.h"

#define StartConvert 0
#define ReadTemperature 1

const byte numReadings = 20;     //the number of sample times
byte ECsensorPin = A8;  //EC Meter analog output,pin on analog 1
byte DS18B20_Pin = 8; //DS18B20 signal, pin on digital 2
unsigned int AnalogSampleInterval=25,printintervalEcEc=700,tempSampleInterval=850;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
byte index = 0;                  // the index of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime,tempSampleTime;
float temperature,ECcurrent; 
 
//Temperature chip i/o
OneWire ds(DS18B20_Pin);  // on digital pin 2

// VARIABLES
// *********************************************
uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again

String outPutStringTest = "11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44";
/*                                                                // Option if not wifi present
//char ssid[] = "Muchanyanya"; //  your network SSID (name)
//char pass[] = "12112006";    // your network passw6ord (use for WPA, or use as key for WEP)
//int keyIndex = 0;            // your network key Index number (needed only for WEP)


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 177);
byte server[] = {31,170,160,82};    // name address for Google (using DNS)
//String builder and string to send
 String dataToSend;
 String sT[] = "";
 String sH []= "";
 String getStringPost0Post ;
 String getStringPart0 [] = {"Get /addf.php?temp=","Get /addm.php?temp=","Get /addv.php?temp=","Get /addc.php?temp="};
 String getStringPart1 ="&moi=";
 String getStringPart2 =" HTTP/1.1";
 String getStringHost  ="Host: arduinoStation.site50.net\r\n";
 String getStringClose ="Connection: close";
 
 char buf[37];  //  char to save the built string to use with get mode
 int len= 37;    //  length of the buffer
 int t;
 int h;
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetClient client;
*/
LiquidCrystal lcd(33,31,29,27,25,23);
 byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};


// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 53;

//Pin connected to ST_CP of 74HC595 These help with the relays byte
#define latchPin 37
//Pin connected to SH_CP of 74HC595
#define clockPin 35
////Pin connected to DS of 74HC595
#define dataPin 39

const int pin  = 4;                                                  //// Temperature and humidity sensors
const int pin1 = 5;
const int pin2 = 6;
const int pin3 = 7;

DHT11 dht[]={pin,pin1,pin2,pin3,/*pin4*/};

// Variable to store temperature and humidity readings
float temp_C;
float temp_CS;
float temp_CM;
float temp_CC;
float temp_CD;
float humidity;
float humidityS;
float humidityM;
float humidityC;
float humidityD;

int tempF,tempS,tempM,tempC,
    humiF,humiS,humiM,humiC ;

float temp[]={ temp_C,temp_CM,temp_CC,temp_CS,temp_CD};
float humi[]={humidity,humidityM,humidityC,humidityS,humidityD};

// Read the values from the sensors .  The variable to store max and mins ,should be set in a string once per hour ,named as the id of 
  //   the int (sensor + hour())and send it to the array LogDaymaxmin [i]
   float temp_Cmax = temp_C;                         // ,where ''i'' will be the value of the hour .Ex : the max and min string about the twellve hour when is one o'clock
   float temp_Cmin = temp_C;                           // will be packed in rht [2] ..to 
   float temp_CMmax = temp_CM;
   float temp_CMmin = temp_CM;                // initalize them in the setup with same valour as the current value of each sensor,should change with the loop
   float temp_CCmax = temp_CC;                
   float temp_CCmin = temp_CC;
   float temp_CSmax = temp_CS;
   float temp_CSmin = temp_CS;
   float temp_CDmax = temp_CD;
   float temp_CDmin = temp_CD;
   float humi_max   = humidity;
   float humi_min  = humidity;
   float humi_Mmax = humidityM;
   float humi_Mmin = humidityM;
   float humi_Cmax = humidityC;
   float humi_Cmin = humidityC;
   float humi_Smax = humidityS;
   float humi_Smin = humidityS;
   float humi_Dmax = humidityD;
   float humi_Dmin = humidityD;   
   
   const int ldr0 = A1;                                  /// light sensor pins
   const int ldr1 = A2;
   const int ldr2 = A3;
   const int ldr3 = A4;
   //const int ldr4 = A5;
   const char ldr[]= {ldr0,ldr1,ldr2,ldr3,/*ldr4*/};

   int ldrValue0;
   int ldrValue1;
   int ldrValue2;
   int ldrValue3;
   int ldrValue4;

   int ldrValue[] = {ldrValue0,ldrValue1,ldrValue2,ldrValue3,ldrValue4};
 
   int temp_CmaxArray[] = {temp_Cmax,temp_CMmax,temp_CCmax,temp_CSmax,temp_CDmax};
   int temp_CminArray[] = {temp_Cmin,temp_CMmin,temp_CCmin,temp_CSmin,temp_CDmin};
   int humi_maxArray[]  = {humi_max,humi_Mmax,humi_Cmax,humi_Smax,humi_Dmax};
   int humi_minArray[]  = {humi_min,humi_Mmin,humi_Cmin,humi_Smin,humi_Dmin};
   // make a string for assembling the data to log:
   String dataString = "";
   
   int arraysArray[5][5]={temp_CmaxArray[5],temp_CminArray[5],humi_maxArray[5],humi_minArray[5],ldrValue[5] };

const int CO2Sensor = A3;
   int thisSecond;
   int thisMinute;
   int thisHour;      // Initialize them with the current value 
   int thisDay;       
   int thisMonth;                            
   int thisYear; 

String ldrString[]= {"Too much light!",
                     "Enough light"
                     "Not enough light"
                     "No light at all"};
int CO2Value = 0;

//      ----if control pad is needed uncomment and reedit this---
/*const int bUp = 35;
const int bDown = 27;
const int bLeft = 33;
const int bRight = 29;
const int bOk = 31;
const int bMenu = 25;
const int buttonPin[6]={bUp,bDown,bLeft,bRight,bOk,bMenu};
String  buttonArray[6]= {"Up","Down","Left","Right","Ok","Menu"};


int buttonStatebUp;
int buttonStatebDown;
int buttonStatebLeft;
int buttonStatebRight;
int buttonStatebOk;
int buttonStatebMenu;
int buttonState[6] = {buttonStatebUp,buttonStatebDown,
                      buttonStatebLeft,buttonStatebRight,
                      buttonStatebOk,buttonStatebMenu};

int lastButtonStatebUp = LOW;
int lastButtonStatebDown = LOW;
int lastButtonStatebLeft = LOW;
int lastButtonStatebRight = LOW;
int lastButtonStatebOk = LOW;
int lastButtonStatebMenu = LOW;
int lastButtonState[6] = {lastButtonStatebUp,lastButtonStatebDown,
                          lastButtonStatebLeft,lastButtonStatebRight,
                          lastButtonStatebOk,lastButtonStatebMenu};

*/

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long padLightGreen = 0 ;     // the green led in the pad will save millis
unsigned long padLightRed = 0;       // the red led in the pad will save millis


unsigned long time;              // This will help to handle the strin that goes to the spreedsheet
int prevDate ;               // Previous Date for keeping track of event log files
float today;

int days ;      // Count days in the flowering room
int daysM ;     // Count days in the mother  room
int daysC ;     // Count days in the cut room
int daysV ;     // Count days in the Summer  room in VEGETATIVE period
int daysF ;      // Count days in the  Summer room in FLOWERING period


unsigned long airCount = 0;                // Save the last time air come in the Flowering room
unsigned long airCountM = 0;               // Save the last time air come in the Mother's room
unsigned long airCountC = 0;               // Save the last time air come in the Cut's room
unsigned long airCountS = 0;               // Save the last time air come in the Summer room
unsigned long airCountD = 0;               // Save the last time air come in the Dry room



unsigned long airCountArray[5]= {airCount,airCountM,airCountC,airCountS,airCountD};

unsigned long airLast = 0;                // Save the last time air come in the Flowering room
unsigned long airLastM = 0;               // Save the last time air come in the Mother's room
unsigned long airLastC = 0;               // Save the last time air come in the Cut's room
unsigned long airLastS = 0;               // Save the last time air come in the Summer room
unsigned long airLastD = 0;               // Save the last time air come in the Dry room

unsigned long airLastArray[5]={airLast,airLastM,airLastC,airLastS,airLastD};
unsigned long airExtractLast;
unsigned long airExtractCount;
// Variables to store the power state of the light, fan and heater
boolean lightState       = false;
boolean lightMState      = false;
boolean lightSState      = false;
boolean rvkInState       = false;
boolean rvkOutState      = false;
boolean fanState         = false;
boolean fanMState        = false;
boolean fanCState        = false;
boolean fanSState        = false;
boolean fanDState        = false;
boolean fanSystemState   = false;
boolean humidifierState    = false;
boolean humidifierCState   = false;
boolean light10State,light11State;



boolean state[16]=
 { lightState,lightMState,lightSState,rvkInState, 
   rvkOutState,fanState,fanMState,fanCState,fanSState,fanDState,fanSystemState,
     humidifierState,humidifierCState,light10State,light11State,};
     
// Boolean to know when vegetative period is running
boolean vegetative = true;

boolean light,lightM,lightS,rvkIn,rvkOut,fan,fanM,fanC,
fanS,fanD,fanSystem,humidifier,humidifierC,light10,light11;

boolean relay[16]=
  { light,lightM,lightS,rvkIn,rvkOut,fan,fanM,fanC,fanS,fanD,
       humidifier,humidifierC,light10,light11,fanSystem };

    
    boolean relayLight[] = {light,lightM,lightS};
    boolean relayFans[] = {fan,fanM,fanC,fanS,fanD};
    boolean relayRvk[] = {rvkIn,rvkOut};
    
    String airStringOn[] = 
       { "Fan Flowering On" ,
         "Fan Mother On",
         "Fan Cuts On",
         "Fan Summer On",
         "Fan DryRoom On"
         "RVK air in On",
         "RVK air out ON",
          };
         
    String airStringOff[] = 
       { "Fan Flower Off" ,
         "Fan Mother Off",
         "Fan Cuts Off",
         "Fan Summer Off",
         "Fan DryRoom Off"
         "RVK air in Off",
         "RVK air out Off",
           };  
         
    String growRoom[] = 
       { "Flowering room",
         "Mother room " ,
         "Cuts room",
         "Summer room",
         "Dry room" };
     
    String growRoomLight[] = 
       { "Flowering room",
         "Mother room " ,
         "Summer room",
                         };  
                            
    String elements[] = {"Maximun temperature","Minimun temperature","Maximun humidity","Minimun humidity","Ldr values"};
                                                                        
    String LightOnString[]={"Flowering HPS ON","Mother Fluor ON","Summer HPS ON"};
    String LightOffString[]={"Flowering HPS OFF","Mother Fluor OFF","Summer HPS OFF"};                       
         
              
    byte bytePassed ;  
    byte bytePassed1 ;
    byte doubleBytePassed;
    byte byteToPass ;
    byte byteToPass1;
    byte doubleByteToPass;

    
// Maximum and minimum temperature and humidity by default
const  int maxTempDefault = 30;
const  int minTempDefault = 21;
const  int maxHumiDefault = 70;
const  int minHumiDefault = 30;  

const  int maxTempMDefault = 33;
const  int minTempMDefault= 21;
const  int maxHumiMDefault = 70;
const  int minHumiMDefault = 30; 

const  int maxTempCDefault = 29;
const  int minTempCDefault = 21;
const  int maxHumiCDefault = 90;
const  int minHumiCDefault = 70; 

const  int maxTempSDefault = 29;
const  int minTempSDefault = 21;
const  int maxHumiSDefault= 70;
const  int minHumiSDefault = 30;

const  int maxTempDDefault = 29;
const  int minTempDDefault = 10;
const  int maxHumiDDefault= 70;
const  int minHumiDDefault = 30; 
  
  int maxTemp = maxTempDefault;                         
  int minTemp = minTempDefault;
  int maxHumi = maxHumiDefault;
  int minHumi = minHumiDefault;  
  int maxTempM = maxTempMDefault;
  int minTempM = minTempMDefault;
  int maxHumiM = maxHumiMDefault;
  int minHumiM = minHumiMDefault; 
  int maxTempC = maxTempCDefault;
  int minTempC = minTempCDefault;
  int maxHumiC = maxHumiCDefault;
  int minHumiC = minHumiCDefault; 
  int maxTempS = maxTempSDefault;
  int minTempS = minTempSDefault;
  int maxHumiS = maxHumiSDefault;
  int minHumiS = minHumiSDefault;
  int maxTempD = maxTempDDefault;
  int minTempD = minTempDDefault;
  int maxHumiD = maxHumiDDefault;
  int minHumiD = minHumiDDefault;

  int maxTempArray[5] = {maxTemp,maxTempM,maxTempC,maxTempS,maxTempD};
  int minTempArray[5] = {minTemp,minTempM,minTempC,minTempS,minTempD};
  int maxHumiArray[5] = {maxHumi,maxHumiM,maxHumiC,maxHumiS,maxHumiD};
  int minHumiArray[5] = {minHumi,minHumiM,minHumiC,minHumiS,minHumiD};
  
  
     // Hours that the light is switched on and off by default
     


const int LightOnFlowering = 9;
const int LightOffFlowering = 21;

const  int LightOnHourMDefault = 6;
const  int LightOffHourMDefault = 23;
  
const  int LightOnHourSDefault = 6;                                         
const  int LightOffHourSDefault = 23;
  
const  int LightOnHourDefault = 9;
const  int LightOffHourDefault = 21;
  
   // ___________________________________Valours for the light periodes ..start with default valours until be edited
  int LightOnHour   = LightOnHourDefault;
  int LightOffHour  = LightOffHourDefault;
  int LightOnHourM  = LightOnHourMDefault;
  int LightOffHourM = LightOffHourMDefault;
  int LightOnHourS  = LightOnHourSDefault;                                            //<--------------------Summer room has two different periods
  int LightOffHourS = LightOffHourSDefault;
  
  int LightOnArray[] = {LightOnHourDefault,LightOnHourMDefault,LightOnHourSDefault};
  int LightOffArray[] = {LightOffHourDefault,LightOffHourMDefault,LightOffHourSDefault};
  

  

 void setup()
    { 
     pinMode(53,OUTPUT);
     digitalWrite(53,HIGH); 
     Serial.begin(9600);
     esp8266.begin(9600);
     lcd.begin(16,2);
     while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
     }

 
  pinMode(2, INPUT);                    // Test of the SQW pin, D2 = INPUT
  digitalWrite(2, HIGH);                // Test of the SQW pin, D2 = Pullup on

     // start the Ethernet connection:
  //if (Ethernet.begin(mac) == 0) {
    //Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
  //}
  // give the Ethernet shield a second to initialize:
 /* delay(1000);
  Serial.println("Ethernet connecting...");
  
  // print your localP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
    lcd.print(Ethernet.localIP()[thisByte], DEC); 
    lcd.print(".");    
  }
*/

/*  /////////////////////////////////////////                  RTC     configurat                 
   PLEASE NOTICE: WE HAVE MADE AN ADDRESS SHIFT FOR THE NV-RAM!!!
                  NV-RAM ADDRESS 0x08 HAS TO ADDRESSED WITH ADDRESS 0x00=0
                  TO AVOID OVERWRITING THE CLOCK REGISTERS IN CASE OF
                  ERRORS IN YOUR CODE. SO THE LAST ADDRESS IS 0x38=56!
*/
  RTC.setRAM(0, (uint8_t *)&startAddr, sizeof(uint16_t));// Store startAddr in NV-RAM address 0x08 

/*
   Uncomment the next 2 lines if you want to SET the clock
   Comment them out if the clock is set.
   DON'T ASK ME WHY: YOU MUST UPLOAD THE CODE TWICE TO LET HIM WORK
   AFTER SETTING THE CLOCK ONCE.
*/
  // TimeIsSet = 0xffff;
  // RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));  

/*
  Control the clock.
  Clock will only be set if NV-RAM Address does not contain 0xaa.
  DS1307 should have a battery backup.
*/
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet != 0xaa55)
  {
    RTC.stopClock();
        
    RTC.fillByYMD(2015,8,15);
    RTC.fillByHMS(23,30,0);
    
    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
  }
  else
  {
    RTC.getTime();
  }

/*
   Control Register for SQW pin which can be used as an interrupt.
*/
  RTC.ctrl = 0x00;                      // 0x00=disable SQW pin, 0x10=1Hz,
                                        // 0x11=4096Hz, 0x12=8192Hz, 0x13=32768Hz
  RTC.setCTRL();
  uint8_t MESZ;
  MESZ = RTC.isMEZSummerTime();
     
  delay(500);
    lcd.clear();
    //////////////////////////////////////////////////////////////////////rtc end


    lcd.createChar(0, smiley);
    lcd.setCursor(7,0);
    lcd.write(byte(0)); 
    delay(800);
    lcd.setCursor(4,2);
    lcd.print("Welcome !");
    delay(750);

    Serial.println("Welcome !");
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
                lcd.scrollDisplayLeft();
                // wait a bit:
                 delay(100);
                }                   
  
  pinMode(clockPin, OUTPUT);  // Clock pin of 74HC595
  pinMode(dataPin, OUTPUT);   // Data pin 
  pinMode(latchPin, OUTPUT);  // Latch to pass the bytes
 /*   lcd.clear();    
    lcd.print("Shifter ready");    
    delay(750);
    lcd.clear();
Serial.println("pH meter ready!");    //Test the serial monitor
 lcd.print("pH meter ready!");    //Test the serial monitor
  delay(500);
  lcd.clear();
  */
  pinMode(LED,OUTPUT);  
  pinMode(phDown,OUTPUT);
  pinMode(phUp,OUTPUT);
  pinMode(micro,OUTPUT);
  pinMode(bloom,OUTPUT);
    
 pinMode(AC_LOAD0, OUTPUT);// Set AC Load pin as output
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
  pinMode(AC_LOAD1, OUTPUT);// Set AC Load pin as output
  attachInterrupt(1, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
  pinMode(AC_LOAD2, OUTPUT);// Set AC Load pin as output
  attachInterrupt(3, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
   /* lcd.clear();    
    lcd.print("dimmer ready");    
    delay(750);
    lcd.clear();
 */   
  
  //____________________________________  Inintialize temp and humidity values with the current values readed by the sensors
  dht[0].read(humidity, temp_C);    //  Flowering room
    lcd.clear();    
    lcd.print("dht0 ready");    
    delay(350);
    lcd.clear();
  dht[1].read(humidityM, temp_CM);  //  Mother room
    lcd.clear();    
    lcd.print("dht1 ready");    
    delay(350);
    lcd.clear();   
  dht[2].read(humidityC, temp_CC);  //  Cuts room
    lcd.clear();    
    lcd.print("dht2 ready");    
    delay(350);
    lcd.clear();
  dht[3].read(humidityS, temp_CS);  //  Summer room
    lcd.clear();    
    lcd.print("dht3 ready");    
    delay(350);
    lcd.clear();  
 // dht[4].read(humidityD, temp_CD);  //  Dry room


// --------------------------EC meter-----------------------------
// initialize all the readings to 0:
  for (byte thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  TempProcess(StartConvert);   //let the DS18B20 start the convert
  AnalogSampleTime=millis();
  printTime=millis();
  tempSampleTime=millis();
  
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Garrulino");
    lcd.setCursor(3,2);
    lcd.print("it's ready");
    delay(750);
    lcd.clear();
   }
 
 void loop() {   
     lcd.print("starting loop");
     Serial.println("Starting loop...");
     //delay(500);
     lcd.clear();
     //serialEvent();
     /*while(Serial.available()) esp8266.write(Serial.read()); 
     delay(100); 
     while(esp8266.available()) Serial.write(esp8266.read());
     */delay(100); 
     //ec();                          // Function to read EC and water temp
     //readPH();                      // Function to read PH    
     pump();                        // Test the 4 pumps  
     //showClock();                   // Shows the date and time
     //sensorControl();               // Check every sensor and set commands to the relays
     //infoDisplay();                 // Shows the current info
     //lightControl();                // Check time and set lights on or off
     //postData();                    // Send data to the database through ethernet in this one
     //dataLog();                     // Start logging maximus and minimus values and save each one,from each room and for hour
     //controlRelays();               // Send to the shifter the right position of the relays
     //relayTest();                   // Test the shifter
   
     lcd.print("ending loop");  
     Serial.println("..ending loop.");   
    // delay(500);
     lcd.clear();
    }         
  
    void showClock() {
    RTC.getTime();                 
    thisYear = (RTC.year,DEC);
    thisMonth = (RTC.month,DEC);
    thisDay = (RTC.day,DEC);
    thisHour = (RTC.hour,DEC);
    thisMinute =(RTC.minute,DEC); 
    today = (RTC.dow,DEC); 
    
        lcd.print("Time  :");
        lcd.setCursor(0,1);
        lcd.print(RTC.hour,DEC);
        lcd.print(":");
        lcd.print(RTC.minute,DEC);
        delay(500); 
        lcd.clear();
          
     lcd.clear();
     Serial.print("clock");
     delay(500); 
    } 
   
  void relayTest() {
      byteToPass = B11111111;
      byteToPass1 = B00000000;
      int i;
      int readedBit ;
      int readedBit1;
      
      
      for (i = 0; i < 8; i++) {
       readedBit =  bitRead(byteToPass,i);
       
       if (readedBit == 0) {
         bitWrite(byteToPass,i,1);         
          } else { bitWrite(byteToPass,i,0);
        }
       
       readedBit1 =  bitRead(byteToPass1,i);
        
        if (readedBit1 == 0) {
         bitWrite(byteToPass1,i,1);         
         }   else { bitWrite(byteToPass1,i,0);
        }
          lcd.print("Setting Relays");
            digitalWrite(latchPin, LOW);
            delay(500);
            shiftOut(dataPin, clockPin,byteToPass); 
            //count down on RED LEDs
            shiftOut(dataPin, clockPin, byteToPass1);
            //return the latch pin high to signal chip that it 
            //no longer needs to listen for information
            digitalWrite(latchPin, HIGH);
            delay(500);
            lcd.print("wait shock");
            delay(1000);
            //delay(500);
            lcd.clear(); 
       }
      
     } 
    
     
 void shiftOut(int myDataPin, int myClockPin, byte myDataOut) 
   {
    int i = 0;
    int pinState;
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);
    delayMicroseconds(20);
    lcd.clear();
    lcd.print("Shifting bytes");
    delay(1000);
    lcd.clear();
    lcd.print("byte");
   
         for (i = 0; i < 8; i++)  
         
         {  digitalWrite(myClockPin, 0);
            pinState =  bitRead(myDataOut,i);
            lcd.setCursor(6+i,0);
            lcd.print(pinState);
            delay(150);
                     
           digitalWrite(myDataPin, pinState); 
           delayMicroseconds(20);
           digitalWrite(myClockPin, 1);
           delayMicroseconds(20);    
           digitalWrite(myDataPin, 0);

      }
        digitalWrite(myClockPin, 0); 
        digitalWrite(myDataPin, 0);       
        lcd.setCursor(0,2);
        lcd.print("Shift Done!");
        delay(1000);
        lcd.clear();
       
       
    }
  
 void infoDisplay() {
   
           lcd.print("Display");
           delay(500);
           lcd.clear();      
        for (int i = 0;i <= 3; i++)  
         {             
           lcd.print("function start");
           delay(750);
           lcd.clear();
            ldrValue[i] = analogRead(ldr[i]); //Read the intensity of light
            lcd.print("read ldr");
           delay(750);
           lcd.clear();
            dht[i].read(humi[i], temp[i]); //  Reading humidity and temperature
            lcd.print("read dht");
           delay(750);
           lcd.clear();
            float tempFloat = temp[i]; 
            lcd.print("float temp");
            delay(750);
           lcd.clear();
            float humiFloat = humi[i]; 
             lcd.print("float humi");
             delay(750);
           lcd.clear();
            
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print(growRoom[i]);
            delay(1000);
            lcd.setCursor(0,2);
            lcd.print("Temperature");
            lcd.print(":");
            lcd.print(temp[i]);
            delay(2000);
                          
       
           lcd.clear();
           lcd.print(growRoom[i]);
           lcd.setCursor(0,2);
           lcd.print("Humidity");
           lcd.print(":");
           lcd.print(humi[i]);
           delay(2000);
           
           lcd.clear();
           lcd.print(growRoom[i]);
           lcd.setCursor(0,2);
           lcd.print("Light");
           lcd.print(":");
           lcd.print(ldr[i],DEC);
           delay(2000);
           lcd.clear();                                              
            }
          }
 
 void controlRelays() 
 
  // Relays list : relay number-socket number - device controled
  // 1-1-Light 2-2-Lightveg 3-4-RVKdimmerOut 4-3-RVkdimmerIn  5-*fanCuts  9-5-LightMother 10-6-Fan 11-7-FanVeg 12-8-HumidifierCuts 
  //Relay byte order : 
  //                  byte 1: 1-light_flow 2-light-veg 3-RVKdimmerOut 4-RVKdimmerIn 5-Fancuts
  //                  byte 2: 2-lightMother 3-Fan 4-FanVeg 5-HumidifierCuts
     
     {
      
       
         if ((byteToPass != bytePassed) || (byteToPass1 != bytePassed1))
         {              
            
            lcd.clear();
            lcd.print("Setting Relays");
            digitalWrite(latchPin, LOW);
            delay(1500);
            shiftOut(dataPin, clockPin, byteToPass1); 
            //count down on RED LEDs
            shiftOut(dataPin, clockPin, byteToPass);
            //return the latch pin high to signal chip that it 
            //no longer needs to listen for information
            lcd.print("wait shock");
            delay(1000);
            lcd.clear();
            digitalWrite(latchPin, HIGH);
                                 
            bytePassed = byteToPass;
            bytePassed1 = byteToPass1;
            //byteToPass = 10000000;
            //byteToPass1 = 00000001;
            lcd.clear();

           }       
         }

 
    
    //-------LIGHT CONTROL ------------------------------------------------------------------
    
  void lightControl() {   // Check the time of the lights On/Off       
     
        vegetative = false; 
           
       for (int i = 0; i <= 2; i++) {
                
             if (i == 1) {
                              
                   if (vegetative == false) {
                     
                    if ((LightOnFlowering <= thisHour ) && (LightOffFlowering >= thisHour))  { 
                       if (relayLight[i] == false){                         
                          relayLight[i] = true;
                          bitWrite(byteToPass,i,1);
                          state[i] = true;                         
                          delay(2000);
                       }
                    }
                       
                      if ((LightOnFlowering <= thisHour ) && (LightOffFlowering <= thisHour)) { 
                                                 
                          relayLight[i] = false;
                          bitWrite(byteToPass,i,0);
                          state[i] = false;                          
                          today = (RTC.day,DEC);                    
                       }
                       
                   } 
             }
                    
                  
                     
             if ((LightOnArray[i] <= thisHour ) && (LightOffArray[i] >= thisHour))  { 
                  if (relayLight[i] == false)  {    
                         
                          relayLight[i] = true;
                          bitWrite(byteToPass,i,1);
                          state[i] = true;
                          today = (RTC.day,DEC);                          
                }
             }    
            if ((LightOnArray[i] <= thisHour ) && (LightOffArray[i] <= thisHour)) {                          
                          relayLight[i] = false;
                          bitWrite(byteToPass,i,0);
                          state[i] = false;
                      }                       
                   }  
                   
                    lcd.clear();
                    lcd.print("Light done");
                    delay(1500);
                    lcd.clear();                                         
             }     



    void sensorControl() 
      {   
                lcd.clear();
                lcd.print("Sensor's check");
                delay(1500);
                lcd.clear();
                //rtc.begin(DateTime(__DATE__, __TIME__));
                // DateTime now = rtc.now();
                lightS = true;        
                CO2Value = analogRead(CO2Sensor);
          
                lcd.clear();
                lcd.print("Co2 ppm :");
                lcd.setCursor(4,1);
                lcd.print(CO2Value);
                delay(1500);
                lcd.clear();
          


   // Read the values from the sensors .  The variable to store max and mins ,should be set in a string once per hour ,named as the id of 
  //   the int (sensor + hour())and send it to the array LogDaymaxmin [i]
   int temp_Cmax = temp_C;                         // ,where ''i'' will be the value of the hour .Ex : the max and min string about the twellve hour when is one o'clock
   int temp_Cmin = temp_C;                           // will be packed in rht [2] ..to 
   int temp_CMmax = temp_CM;
   int temp_CMmin = temp_CM;                // initalize them in the setup with same valour as the current value of each sensor,should change with the loop
   int temp_CCmax = temp_CC;                
   int temp_CCmin = temp_CC;
   int temp_CSmax = temp_CS;
   int temp_CSmin = temp_CS;
   int temp_CDmax = temp_CD;
   int temp_CDmin = temp_CD;
   int humi_max   = humidity;
   int humi_min   = humidity;
   int humi_Mmax = humidityM;
   int humi_Mmin = humidityM;
   int humi_Cmax = humidityC;
   int humi_Cmin = humidityC;
   int humi_Smax = humidityS;
   int humi_Smin = humidityS;
   int humi_Dmax = humidityD;
   int humi_Dmin = humidityD;     
 
   int temp_CmaxArray[] = {temp_Cmax,temp_CMmax,temp_CCmax,temp_CSmax,temp_CDmax};
   int temp_CminArray[] = {temp_Cmin,temp_CMmin,temp_CCmin,temp_CSmin,temp_CDmin};
   int humi_maxArray[]  = {humi_max,humi_Mmax,humi_Cmax,humi_Smax,humi_Dmax};
   int humi_minArray[]  = {humi_min,humi_Mmin,humi_Cmin,humi_Smin,humi_Dmin};
   
   lcd.clear();
   lcd.print("Default max/min");
   delay(1500);
   lcd.clear();
                       
        for (int i = 0;i <= 3; i++)   
         {  
            ldrValue[i] = analogRead(ldr[i]); //Read the intensity of light            
            dht[i].read(humi[i], temp[i]); //  Reading humidity and temperature
                        
            float tempFloat = temp[i]; 
            float humiFloat = humi[i];   
            
            if (temp_CmaxArray[i] < tempFloat) {temp_CmaxArray[i] = tempFloat;}
            if (temp_CminArray[i] > tempFloat) {temp_CminArray[i] = tempFloat;}
            if (humi_maxArray[i] >  humiFloat) {humi_maxArray[i] =  humiFloat;}
            if (humi_minArray[i] <  humiFloat) {humi_minArray[i] =  humiFloat;}
         
   lcd.clear();
   lcd.print("Logged max/min:");
   lcd.setCursor(0,1);
   lcd.print("i = ");
   lcd.setCursor(5,1);
   lcd.print(i);
   delay(1500);
   lcd.clear();
             
// Relays list : relay number-socket number - device controled
  // 1-1-Light 2-2-Lightveg 3-4-RVKdimmerOut 4-3-RVkdimmerIn  5-*fanCuts  9-5-LightMother 10-6-Fan 11-7-FanVeg 12-8-HumidifierCuts 
  //Relay byte order : 
   //                               bit1          bit2            bit3            bit4                 bit5              bit6           bit7           bit8
  //                  byte 1: 1-light_flow  2-light-veg   3-light Mother     4-RVKdimmerIn       5-RVKdimmerOut  6-Fan Flowering   7-Fan Mother   8-Fan cuts
  
  //                  byte 2: 9-Fan Summer  10-Fan Dry    11-Humidifier Flow 5-HumidifierCuts
                 
            
                                  
          if (maxTempArray[i] < temp[i]) {           //If the room temperature is over the maximun
          
            
            relayFans[i] = true;   // Start all the fans and rvk
            relayRvk[1] = true;
            relayRvk[2] = true;
            airLastArray[i] = millis();
            if (i<3) {                    // if i is bigger than 3,should be in the second byte
             bitWrite(byteToPass,6+i,1); }   
               else {bitWrite(byteToPass1,4-i,1);} 
               
            bitWrite(byteToPass,4,1);    //Set the rvkIn ON
            bitWrite(byteToPass1,5,1);   //Set the rvkout O
            lcd.print(airStringOn[i]);
                    
                   
           }  
           
          //If the room temperature is below the minimun 
          if (minTempArray[i] > temp[i]) {  
                        
            relayFans[i] = false;   // Stop all the fans and rvk
            relayRvk[1] = false;
            relayRvk[2] = false;
            if (i<3) {                    // if i is bigger than 3,should be in the second byte
               bitWrite(byteToPass,6+i,1); }   
               else {bitWrite(byteToPass1,4-i,1);}             bitWrite(byteToPass,4,0);    //Set the rvkIn off
            bitWrite(byteToPass1,5,0);   //Set the rvkout off
            airLastArray[i] = millis();     //Start the count since [i] Fan went off 
            airExtractLast = millis();   //Start the count since rvk went off
            airExtractCount = millis();//Start counting when rvks are on
            
          } 
            // If the temperature is perfect 
          if ((maxTempArray[i] > temp[i]) &&  (temp[i]> minTempArray[i])) {
                   
            
         // Fan control when Temperature is perfect __________________________________________________________________
                         // 
            
            if ( (i!=2) && (i!=4) ) {  // Dry room and cuts
             
                if ((airLastArray[i] == 0) && (airCountArray[i] == 0)) {
                                  // Start Fans [i]
               
                        
               relayFans[i] = true;
               //Set the Fan  ON 
               if (i<3) {                    // if i is bigger than 3,should be in the second byte
               bitWrite(byteToPass,6+i,1); }   
               else {bitWrite(byteToPass1,4-i,1);} 
               airCountArray[i] = millis();//Counting the time Fan is On
                
                } 
                
                if  (((millis()-airCountArray[i]) >= 120000) && (relayFans[i] = true))  {
                                  // Stop Fans [i]
              
               
               delay(800);               
                 
                 
               relayFans[i] = false;
                 //Set the Fan  ON 
               if (i<3) {                    // if i is bigger than 3,should be in the second byte
               bitWrite(byteToPass,6+i,1); }   
               else {bitWrite(byteToPass1,4-i,1);}                airLastArray[i]= millis();
               airCountArray[i] = 0;//Counting the time Fan is On
               lcd.print(airStringOff[i]);
               delay(2000);
               lcd.clear();
                               
                } 
               
              if  ((((millis()-airLastArray[i]) > 60000) && (relayFans[i] == false)) && (airLastArray != 0)){
                                  // Start Fans [i]
               lcd.print("Start Fan Again");  
               lcd.setCursor(0,2); 
               lcd.print(growRoom[i]); 
 
               delay(800);               
 
               delay(800);               
               
               delay(1500);
               lcd.clear();            
               relayFans[i] = true;
                 //Set the Fan  ON 
               if (i<3) {                    // if i is bigger than 3,should be in the second byte
               bitWrite(byteToPass,6+i,1); }   
               else {bitWrite(byteToPass1,4-i,1);} 
               airLastArray[i] = 
               airCountArray[i] = millis();//Counting the time Fan is On
               lcd.print(airStringOn[i]);
               delay(2000);
               lcd.clear();
                               
                } 
            }
            
         // RVK control when temperature is perfect______________________________________________________________________
         
                        // Move all the air after 6 minutes from the last air extraction when temperature 
                        //is perfect,is day and Rvks arent working yet.By rvk
              
              if ((airExtractCount==0)&&(airExtractLast==0))  {
               

                // Start rvk
               
               lcd.clear();               
               lcd.print("First Extracting"); 
               lcd.setCursor(0,2);
               lcd.print("Day mode");
               delay(1500);               
               relayRvk[1] = true;
               relayRvk[2] = true; 
               bitWrite(byteToPass,4,1);    //Set the rvkIn ON
               bitWrite(byteToPass,5,1);   //Set the rvkout ON
               airExtractCount = millis();//Counting the time rvks On
               airExtractLast = millis();
               lcd.print(airStringOn[5]);
               lcd.setCursor(0,1);
               lcd.print(airStringOn[6]);
               delay(2000);
               lcd.clear();
               
              } 
           
                     //Moving the air after 12 minutes,if day in summer/vegetative room and rvks werent 6 minutes
                     // working yet
              if (((millis()-airExtractLast) > 600000 )  && ((lightS == true) && (airExtractCount==0))) {
               
               lcd.clear();
               lcd.print("Extracting Again"); 
               lcd.setCursor(0,2);
               lcd.print("Day mode");                   
               delay(500);            
              
               relayRvk[1] = true;
               relayRvk[2] = true;
               bitWrite(byteToPass,4,1);    //Set the rvkIn ON
               bitWrite(byteToPass,5,1);   //Set the rvkout ON
               airExtractCount = millis();//Counting the time rvks On
               lcd.print(airStringOn[5]);
               lcd.setCursor(0,1);
               lcd.print(airStringOn[6]);
               delay(2000);
               lcd.clear();
               
              }    // St   // Stop moving the air after 6 minutes if its day
          
              if (((millis()-airExtractCount) > 60000) && (lightS == true) && (airExtractCount != 0)) {
               
                // Stop rvk

               delay(1500); 
               lcd.print("Stop Extraction"); 
               lcd.setCursor(0,2);
               lcd.print("Day mode");
                             
               delay(500);
               
               relayRvk[1] = false;
               relayRvk[2] = false; 
               bitWrite(byteToPass,4,0);    //Set the rvkIn ON
               bitWrite(byteToPass1,3,0);   //Set the rvkout ON
               airExtractCount = 0;//Counting the time rvks On
               airExtractLast = millis();
               lcd.print(airStringOff[5]);
               lcd.setCursor(0,1);
               delay(2000);
               lcd.clear();
               
              }         
                        // Move all the air after 6 minutes when temperature 
                        //is perfect.By rvk                NIGHT MODE
              if ((millis()-airExtractLast > 20000) && (lightS == false) && (airExtractCount ==0)) {
               
                               // Start rvk
               lcd.print("Start Extraction");  
               lcd.setCursor(0,2); 
               lcd.print("Night Mode"); 
               delay(1500);
               lcd.clear();            
               relayRvk[1] = true;
               relayRvk[2] = true; 
               bitWrite(byteToPass,4,1);    //Set the rvkIn ON
               bitWrite(byteToPass1,3,1);   //Set the rvkout ON
               airExtractCount = millis();//Counting the time rvks On
               lcd.print(airStringOn[5]);
               lcd.setCursor(0,1);
               lcd.print(airStringOn[6]);
               delay(2000);
               lcd.clear();
               
              } 
                           // Stop moving the air after 6 minutes if its night
          
              if (((millis() -airExtractCount) > 5000) && (lightS == false)) {
                                      // Stop rvk
               lcd.print("Stop Extraction");  
               lcd.setCursor(0,2); 
               lcd.print("Night Mode");
               delay(1500);
               lcd.clear();               
               relayRvk[1] = false;
               relayRvk[2] = false;
               bitWrite(byteToPass,4,0);    //Set the rvkIn ON
               bitWrite(byteToPass1,3,0);   //Set the rvkout ON
               airExtractCount = 0;//Counting the time rvks On
               airExtractLast = millis();
               lcd.print(airStringOff[5]);
               lcd.setCursor(0,1);
               lcd.print(airStringOff[6]);
               delay(2000);
               lcd.clear();
               
              }   
           }                               
         }          
      }
      
      
  void dataLog()  
 {    
  // RTC.getTime(); 
   if (thisHour != (RTC.hour,DEC)) {       
      
      for (int i = 0; i <= 5; i++) {
         for (int j = 0;j <=5; j++) {
           int data = arraysArray[i][j];
            dataString += String(data);
            if ( i < 4 ) {
              dataString += ",";
            }
         }
      }
     // dataString is an string with all the values ,we need to add the hour and the day  
      dataString += "-" ;
      dataString += (thisHour);
      dataString += "/" ;
      dataString += (thisDay);
      dataString += "/" ;
      dataString += (thisMonth);
      dataString += "/" ;     
      dataString += (thisYear) ;
      dataString += (".");
  
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      //File dataFile = SD.open("datalog.txt", FILE_WRITE);
      // if the file is available, write to it:
      File dataFile; // it's already declared above but not using during this test
      if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
       // print to the serial port too:
       lcd.print(dataString);
       delay(1000);
       lcd.clear();
      }
       // if the file isn't open, pop up an error:
       else {
       lcd.print("fail datalog.txt");
     }
   }
 }
 
                          //the interrupt function must take no parameters and return nothing
 void zero_crosss_int()  //function to be fired at the zero crossing to dim the light
    {
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms 
  // Every zerocrossing thus: (50Hz)-> 10ms (1/2 Cycle) 
  // For 60Hz => 8.33ms (10.000/120)
  // 10ms=10000us
  // (10000us - 10us) / 128 = 75 (Approx) For 60Hz =>65
    for (int i = 3; i <= 3;i++) 
        {
        int dimtime = (75*dimmingArray[3]);    // For 60Hz =>65    
        delayMicroseconds(dimtime);    // Wait till firing the TRIAC
        digitalWrite(AC_LOADArray[i], HIGH);   // Fire the TRIAC
        delayMicroseconds(10);         // triac On propogation delay (for 60Hz use 8.33)
        digitalWrite(AC_LOADArray[i], LOW);    // No longer trigger the TRIAC (the next zero crossing will swith it off) TRIAC
      }
    }
    

void clock()
{
  //RTC.getTime();
  if (RTC.hour < 10)                    // correct hour if necessary
  {
    lcd.print("0");
    lcd.print(RTC.hour, DEC);
    delay(1000);
    lcd.clear();
  } 
  else
  {
    lcd.print(RTC.hour, DEC);
    delay(1000);
    lcd.clear();
  }
  lcd.print(":");
  if (RTC.minute < 10)                  // correct minute if necessary
  {
    lcd.print("0");
    lcd.print(RTC.minute, DEC);
    delay(1000);
    lcd.clear();
  }
  else
  {
    lcd.print(RTC.minute, DEC);
    delay(1000);
    lcd.clear();
  }
  lcd.print(":");
  if (RTC.second < 10)                  // correct second if necessary
  {
    lcd.print("0");
    lcd.print(RTC.second, DEC);
    delay(1000);
    lcd.clear();
  }
  else
  {
    lcd.print(RTC.second, DEC);
    delay(1000);
    lcd.clear();
  }
  lcd.print(" ");
  if (RTC.day < 10)                    // correct date if necessary
  {
    lcd.print("0");
    lcd.print(RTC.day, DEC);
    delay(1000);
    lcd.clear();
  }
  else
  {
    lcd.print(RTC.day, DEC);
    delay(1000);
    lcd.clear();
  }
  lcd.print("-");
  if (RTC.month < 10)                   // correct month if necessary
  {
    lcd.print("0");
    lcd.print(RTC.month, DEC);
    delay(1000);
    lcd.clear();
  }
  else
  {
    lcd.print(RTC.month, DEC);
    delay(1000);
    lcd.clear();
  }
  lcd.print("-");
  lcd.print(RTC.year, DEC);          // Year need not to be changed
  lcd.print(" ");
  delay(1000);
    lcd.clear();
  switch (RTC.dow)                      // Friendly printout the weekday
  {
    case 1:
      lcd.print("MON");
      delay(1000);
    lcd.clear();
      break;
    case 2:
      lcd.print("TUE");
      delay(1000);
    lcd.clear();
      break;
    case 3:
      lcd.print("WED");
      delay(1000);
    lcd.clear();
      break;
    case 4:
      lcd.print("THU");
      delay(1000);
    lcd.clear();
      break;
    case 5:
      lcd.print("FRI");
      delay(1000);
    lcd.clear();
      break;
    case 6:
      lcd.print("SAT");
      delay(1000);
    lcd.clear();
      break;
    case 7:
      lcd.print("SUN");
      delay(1000);
    lcd.clear();
      break;
  }
  lcd.print("since 1.1.2000:");
  delay(1000);
    lcd.clear();
  lcd.print(RTC.time2000, DEC);
  delay(1000);
    lcd.clear();
  uint8_t MESZ = RTC.isMEZSummerTime();
  lcd.print(" MEZ=0, MESZ=1 : ");
  lcd.print(MESZ, DEC); 
  delay(1000);
  lcd.clear();
  
  lcd.print("Address is:");
  RTC.getRAM(0, (uint8_t *)&lastAddr, sizeof(uint16_t));
  lcd.print(lastAddr, HEX);
  delay(1000);
  lcd.clear();
  lastAddr = lastAddr + 1;              // we want to use it as addresscounter for example
  RTC.setRAM(0, (uint8_t *)&lastAddr, sizeof(uint16_t));
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet == 0xaa55)              // check if the clock was set or not
  {
    lcd.print("Clock set!");
    delay(1000);
    lcd.clear();
  }
  else
  {
    lcd.print("NOT set!");
    delay(1000);
    lcd.clear();
  }    
  delay(1000);                          // wait a second
 }
          
               // The function post the values in the database of the server
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  void readPH()
{
  Serial.println("readPh");    //Test the serial monitor
  delay(1000);
  lcd.print("readPh");    
  delay(1000);
  lcd.clear();
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      phArray[phArrayIndex++]=analogRead(SensorPin);
      Serial.print("Analog read : ");
      Serial.println(analogRead(SensorPin));
      if(phArrayIndex==ArrayLenth)phArrayIndex=0;
      voltage = avergearray(phArray, ArrayLenth)*5.0/1024;
      phValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
        Serial.println(phValue,DEC);
         lcd.print("Voltage");  
         lcd.print(1,0); 
         lcd.print(voltage,2); 
         delay(1000);
         lcd.clear();
         lcd.print("PH ");  
         lcd.print(1,0); 
         lcd.print(phValue,2); 
         delay(1000);
         lcd.clear();
      
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
  }
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    lcd.print("Error number ");
    delay(1000);
    lcd.clear();
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
    phValueOnCheck = phValue;
}

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void pump() {
  
  Serial.println("Start pumping");    //Test the serial monitor
  lcd.print("Start pumping ");
  delay(1000);
  lcd.clear();
  digitalWrite(micro,HIGH);
  delay(10000);
  digitalWrite(micro,LOW);
  Serial.println("pumping micro");    //Test the serial monitor
  lcd.print("pumping micro");
  delay(1000);
  lcd.clear();
/*
  Serial.println("Start pumping");    //Test the serial monitor
  lcd.print("Start pumping ");
  delay(1000);
  lcd.clear();
  digitalWrite(bloom,HIGH);
  delay(2000);
  digitalWrite(bloom,LOW);
  Serial.println("pumping bloom");    //Test the serial monitor
  lcd.print("pumping bloom");
  delay(1000);
  lcd.clear();
  
  Serial.println("Start pumping");    //Test the serial monitor
  lcd.print("Start pumping ");
  delay(1000);
  lcd.clear();
  digitalWrite(phUp,HIGH);
  delay(2000);
  digitalWrite(phUp,LOW);
  Serial.println("pumping phUP");    //Test the serial monitor
  lcd.print("pumping phUP");
  delay(1000);
  lcd.clear();

   Serial.println("Start pumping");    //Test the serial monitor
  lcd.print("Start pumping ");
  delay(1000);
  lcd.clear();
  digitalWrite(phDown,HIGH);
  delay(2000);
  digitalWrite(phDown,LOW);
  Serial.println("pumping phDown");    //Test the serial monitor
  lcd.print("pumping phDown");
  delay(1000);
  lcd.clear();
  */ 
  }

  void ec() {
  /*
   * 
   * 
   Every once in a while,sample the analog value and calculate the average.
  */

  lcd.print("Test Ec & temp");
  Serial.print("Test Ec & temp");
  delay(1500);
  lcd.clear();
  if(millis()-AnalogSampleTime>=AnalogSampleInterval)  
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - readings[index];
    // read from the sensor:
    readings[index] = analogRead(ECsensorPin);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    // if we're at the end of the array...
    if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=tempSampleInterval) 
  {
    tempSampleTime=millis();
    temperature = TempProcess(ReadTemperature);  // read the current temperature from the  DS18B20
    TempProcess(StartConvert);                   //after the reading,start the convert for next reading
  }
   /*
   Every once in a while,print the information on the serial monitor.
  */
  if(millis()-printTime>=printintervalEcEc)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;
    lcd.print("value:");
    lcd.print(AnalogAverage);   //analog average,from 0 to 1023
    lcd.setCursor(0,1);
    lcd.print(" Voltage:");
    lcd.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    lcd.print("mV    ");
    delay(1500);
    lcd.clear();
    lcd.print("temp:");
    lcd.print(temperature);    //current temperature
    delay(1500);
    lcd.clear();
    lcd.print("^C EC:");
    Serial.println("Analog value :");
    Serial.print(AnalogAverage,DEC);
    Serial.println("Voltage: ");
    Serial.println(averageVoltage,DEC);
    
    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
    if(CoefficientVolatge<150)lcd.print("No solution!");
    //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)lcd.print("Out of range!");
   //>20ms/cm,out of the range
    else
    { 
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
      //ECcurrent/=1000;    //convert us/cm to ms/cm   original
      ECcurrent/=10;    //convert us/cm to ms/cm
      delay(1000);
      lcd.clear();
      Serial.println("EC current :");
      Serial.println(ECcurrent,DEC);
      lcd.print(ECcurrent,2);  //two decimal
      lcd.println("us/cm");
      //Serial.println("us/cm");                  original
       delay(1500);
       lcd.clear();
    }
    delay(1500);
    lcd.clear();
  }

}
/*
ch=0,let the DS18B20 start the convert;ch=1,MCU read the current temperature from the DS18B20.
*/
float TempProcess(bool ch)
{
  //returns the temperature from one DS18B20 in DEG Celsius
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;
  if(!ch){
          if ( !ds.search(addr)) {
              Serial.println("no more sensors on chain, reset search!");
              lcd.print("no more sensors on chain, reset search!");
              delay(1500);
              lcd.clear();
              ds.reset_search();
              return 0;
          }      
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              Serial.println("CRC is not valid!");
              lcd.println("CRC is not valid!");
              delay(1500);
              lcd.clear();
              return 0;
          }        
          if ( addr[0] != 0x10 && addr[0] != 0x28) {
              Serial.print("Device is not recognized!");
              lcd.print("Device is not recognized!");
              delay(1500);
              lcd.clear();
              return 0;
          }      
          ds.reset();
          ds.select(addr);
          ds.write(0x44,1); // start conversion, with parasite power on at the end
  }
  else{  
          byte present = ds.reset();
          ds.select(addr);    
          ds.write(0xBE); // Read Scratchpad            
          for (int i = 0; i < 9; i++) { // we need 9 bytes
            data[i] = ds.read();
          }         
          ds.reset_search();           
          byte MSB = data[1];
          byte LSB = data[0];        
          float tempRead = ((MSB << 8) | LSB); //using two's compliment
          TemperatureSum = tempRead / 16;
    }
          return TemperatureSum;  
}

void postData()  
  { 
   // long postDelay = 60000;
   // long minuteRef ;
   // long lastPost ;
    minuteRef = (int)thisMinute;    
    hourRef = (int)thisHour;
    Serial.println("PostData");
    Serial.print("minuteRef =");
    Serial.println(minuteRef,DEC);
    Serial.print("lastPost =");
    Serial.println(lastPost,DEC);
    Serial.print("hourRf :");
    Serial.print(hourRef,DEC);
    if (((minuteRef - lastPost) >= postDelay) || (hourRef != lastPostHour))
   {    
    String outPutString = "";
    const char* come = ",";    
    lcd.print("Parsing data");
    Serial.println("Parsing data...");
    delay(1000);
    lcd.clear();
    tempF=(int)temp_C;
    tempS=(int)temp_CS;
    tempM=(int)temp_CM;
    tempC=(int)temp_CC;
    humiF=(int)humidity;
    humiS=(int)humidityS;
    humiM=(int)humidityM;
    humiC=(int)humidityC;
    
    String tempFstr = String(tempF);
    String humiFstr = String(humiF);
    String lightFstr = String(ldrValue0);
    String co2str = String(CO2Sensor);
    String tempSstr = String(tempS);
    String humiSstr = String(humiS);
    String lightSstr = String(ldrValue1);
    String co2Sstr = String(CO2Sensor);
    String tempMstr = String(tempM);
    String humiMstr = String(humiM);
    String lightMstr = String(ldrValue2);
    String co2Mstr = String(CO2Sensor);
    String tempCstr = String(tempC);
    String humiCstr = String(humiC);
    String lightCstr = String(ldrValue3);
    String co2Cstr = String(CO2Sensor);

    lcd.print("Flowering Values:");
    Serial.println("Flowering values :");
    lcd.setCursor(0,2);
    outPutString +=tempFstr;
    //lcd.print(outPutString);
    delay(1000);
    outPutString += come;
    outPutString += humiFstr;
    //lcd.print(outPutString);
    delay(1000);
    outPutString += come;
    outPutString += lightFstr;
    //lcd.print(outPutString);
    delay(1000);
    outPutString += come;
    outPutString += co2str;
    lcd.print(outPutString);
    delay(1000);
    lcd.clear();
    Serial.println(outPutString);
    
    lcd.print("Vegetative Values:");
    Serial.println("+ vegetative values");            
    outPutString += come;
    lcd.setCursor(0,2);
    outPutString += tempSstr;
    //lcd.print(outPutString);
   
    outPutString += come;
    outPutString += humiSstr;
    //lcd.print(outPutString);
    
    outPutString += come;
    outPutString += lightSstr;
    //lcd.print(outPutString);

    outPutString += come;
    outPutString += co2str;
    lcd.print(outPutString);
   
    outPutString += come;   
    lcd.clear();
    Serial.println(outPutString);

    lcd.print("Mother Values:");
    Serial.println("+Mother values");
    lcd.setCursor(0,2);
    outPutString +=tempMstr;
    //lcd.print(outPutString);
   
    outPutString += come;
    outPutString += humiMstr;
    //lcd.print(outPutString);
   
    outPutString += come;
    outPutString += lightMstr;
    //lcd.print(outPutString);
    
    outPutString += come;
    outPutString += co2Mstr;
    lcd.print(outPutString);
    
    outPutString += come;
    lcd.clear();
    Serial.println(outPutString);
    
    lcd.print("Cuts Values:");
    Serial.println("+ cuts values");
    lcd.setCursor(0,2);
    outPutString +=tempCstr;
    //lcd.print(outPutString);
   outPutString += come;
    outPutString += humiCstr;
    //lcd.print(outPutString);
   
    outPutString += come;
    outPutString += lightCstr;
    //lcd.print(outPutString);

    outPutString += come;
    outPutString += co2Cstr; 
    lcd.print(outPutString);
    delay(1000);
    lcd.clear();   
    Serial.println(outPutString);
    Serial.println();
   //Serial.println(outPutStringTest);
   //Serial.println();
    Serial.println("Outputstring seems as :");
    Serial.println(outPutString);
    esp8266.println(outPutString);
    Serial.println("outPutString has been printed in esp8266 serial ");
    Serial.println("....................................................");    
    delay(1000);
    lcd.clear();
    lastPost = thisMinute;
    lastPostHour = thisHour;
    serialEvent();
    //while(esp8266.available()) lcd.print(esp8266.read());
   
   /* while(esp8266.available())
     { 
      line += esp8266.read();
      delay(10);
      Serial.print(line); 
               
     }
     
    line = "";
    outPutString = "";
     */  // while(esp8266.available()) lcd.print(esp8266.read());
    }
  } 

 void serialEvent() { 
   Serial.println("Serial event ");       
   Serial.println("Serial available...");
    // get the new byte:
    inChar = (char)Serial.read();
    Serial.println("inChar :");
    Serial.print(inChar);
    delay(1000);
   // Serial.print(inChar);                                            // add it to the inputString:    
    inputString += inChar;
   // Serial.println(inputString); 
    

    inputString.toCharArray(arr, len) ;
  //Serial.println("arr ready");                                          // if the incoming character is a newline, set a flag
    if (inChar == '\n') {                                      // so the main loop can do something about it:  
    stringComplete = true;    
    delay(1000);  
    Serial.println("Connection done !");      
    Serial.println(arr);  
    Serial.println("...this was arr");
    delay(1000);
      
  
   } 
  
}  
