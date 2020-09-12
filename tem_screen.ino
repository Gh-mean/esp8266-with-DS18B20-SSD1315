/**********************************************
 * Compilation time：2020.7.24
 * Author：Mark z
 * Version number：V1.0
 * Function:使用DS18B20获得温度，使用pwm根据温度控制风扇转速，并使用SSD1315屏幕显示温度及其转速
 */



//for screen's library
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

//for DS18B20's library
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//for ntp's library
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//screen

String version_namber="v1.0";

//DS18B20
String temperature_before = "2";
String temperature = "1";

//for wifi 
const char *ssid     = "JHC_SLF";
const char *password = "justdoit1";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);


void setup() {
  Serial.begin(115200);
  
  //screen
  Wire.begin(); 
  oled.init();                      // Initialze SSD1315 OLED display
  oled.clearDisplay();              // Clear screen
  Serial.println("screen init seccess");
  
  //DS18B20
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  Serial.println("DS18B20 load seccess");
  sensors.begin();

  //for wifi
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print(temperature);
  Serial.print("      ");
  Serial.println(temperature_before);
  delay(320);
  while (temperature == "-127.00")
  {
   Serial.println("TEMP ERROR!!");
   oled.setTextXY(5,2);
   oled.putString("TEMP ERROR!!");
    while (1)
    {
      sensors.requestTemperatures();
      temperature = sensors.getTempCByIndex(0);
      if (temperature != "-127.00"){break;}
      delay(320);
     }
   oled.clearDisplay();
  }
//for wifi
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  //screen
  if (temperature != temperature_before){
  Serial.println("display:"+temperature);
  oled.setTextXY(2,1);
  oled.putString("TEMP:");
  oled.putString(temperature);
  oled.setTextXY(6,1);
  oled.putString("v1.0");
  temperature_before = temperature;
  }
  oled.setTextXY(4,1);
  oled.putString(timeClient.getFormattedTime());
    delay(1000);
}
