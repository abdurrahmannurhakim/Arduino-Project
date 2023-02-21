//============================================================================
// Name        : IoT Fuel Sensor.Ino
// Author      : Abdurrahman Nurhakim
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : For Monitoring Fuel in Vehicel using Internal Wifi via MQTT Protocol
//============================================================================

// Import required libraries
#include "WiFi.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "DHT.h"
#include <elapsedMillis.h>
#define DHTPIN 14
#define DHTTYPE DHT22
elapsedMillis timer;
DHT dht(DHTPIN, DHTTYPE);

float t, h;
float v;
float f;
float dt;
float Angle_1;
float dAngle;
float Velocity;
int flagVelocity;
int counter;
String dataState;

const char* ssid     = "ECS Monitoring";
const char* password = "hariffdte450";
AsyncWebServer server(80);


void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  dht.begin();

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  handleNoConnection();
  server.begin();
}

void loop() {
  t = IsnanCheck((float)dht.readTemperature()); //satuan -> derajat
  int CRC, head;
  char D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15;
  String DATA, DATAVIEW;
  float InFuel = analogRead(33);
  float Fuel = abs(InFuel - 48) * 100 / 4000.00;
  float InVoltage = analogRead(34);
  float Voltage = InVoltage * 25.55 / 4096.00;
  float sudut = ((float)counter) * 360.00 / 400.00;

  //Data Sensor

  D1 = (char)Unit(parsDecimal(Voltage));
  D2 = (char)Tens(parsDecimal(Voltage));
  D3 = (char)Hundreds(parsDecimal(Voltage));
  D4 = (char)Unit(parsDecimal(parsFloat(Voltage)));
  D5 = (char)Tens(parsDecimal(parsFloat(Voltage)));
  D6 = (char)Unit(parsDecimal(Fuel));
  D7 = (char)Tens(parsDecimal(Fuel));
  D8 = (char)Hundreds(parsDecimal(Fuel));
  D9 = (char)Unit(parsDecimal(parsFloat(Fuel)));
  D10 = (char)Tens(parsDecimal(parsFloat(Fuel)));
  D11 = (char)Unit(parsDecimal(t));
  D12 = (char)Tens(parsDecimal(t));
  D13 = (char)Hundreds(parsDecimal(t));
  D14 = (char)Unit(parsDecimal(parsFloat(t)));
  D15 = (char)Tens(parsDecimal(parsFloat(t)));


  //CRC Sum
  CRC = (int)('#') + (int)D1 + (int)D2 + (int)D3;
  CRC += (int)D4 + (int)D5 + (int)D6 + (int)D7;
  CRC += (int)D8 + (int)D9 + (int)D10 + (int)D11;
  CRC += (int)D12 + (int)D13 + (int)D14 + (int)D15;

  //Data Parsing
  DATA = String('#') + String(D1) + String(D2);
  DATA += String(D3) + String(D4) + String(D5);
  DATA += String(D6) + String(D7) + String(D8);
  DATA += String(D9) + String(D10) + String(D11);
  DATA += String(D12) + String(D13) + String(D14);
  DATA += String(D15) + String((char)CRC);

  Serial2.println(DATA);
  Serial.print(t); Serial.print("\t");
  Serial.println(Fuel);
}

void gettemperature()
{

  float InFuel = analogRead(33);
  float InVoltage = analogRead(34);
  float Volt = InVoltage * 25.55 / 4096.00;
  t = IsnanCheck((float)dht.readTemperature());
  h = IsnanCheck((float)dht.readHumidity());
  v = toPercent(Volt, 22.5, 25.52);
  f = abs(InFuel - 48) * 100 / 4000.00;
}

void handleNoConnection() /*------------------------------------------------------------------------------------------------------------------------> Rubah*/
{
  gettemperature();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/Style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/Style.css", "text/css");
  });
  server.on("/Script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/Script.js", "text/js");
  });

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(v).c_str());
  });
  server.on("/fuel", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(f).c_str());
  });

  delay(1000);
}

float toPercent(float InVal, float MinVal, float MaxVal)
{
  float Interval = MaxVal - MinVal;
  float Result;

  if (InVal - MinVal >= 0 && (InVal - MinVal) < (MaxVal - MinVal))
  {
    Result = abs(InVal - MinVal) * (100.00 / Interval);
  } else if (InVal - MinVal >= (MaxVal - MinVal))
  {
    Result = 100;
  } else
  {
    Result = 0;
  }
  return Result;
}

float IsnanCheck(float Input)
{
  float result;
  if (isnan(Input)) {
    result = 0;
  } else {
    result = Input;
  }
  return result;
}

float fil_average(float IN_SUM, float IN_devide)
{
  float IN_fil_average;
  IN_fil_average = IN_SUM / IN_devide;
  return IN_fil_average;
}

int parsDecimal(float Data)
{
  int GetData;
  GetData = (int)Data;
  return GetData;
}

int parsFloat(float Data)
{
  int GetFloat;
  GetFloat = (Data - ((int)Data)) * 100;
  return GetFloat;
}

int Unit(int Input)
{
  int cal, result;
  cal = Input % 10;
  if (cal == 0) {
    result = 12;
  } else if (cal == 9) {
    result = 11;
  } else {
    result = cal;
  }
  return result;
}

int Tens(int Input)
{
  int cal, result;
  cal = ((Input % 100) - (Input % 10)) / 10;
  if (cal == 0) {
    result = 12;
  } else if (cal == 9) {
    result = 11;
  } else {
    result = cal;
  }
  return result;
}

int Hundreds(int Input)
{
  int cal, result;
  cal = (Input % 1000) / 100;
  if (cal == 0) {
    result = 12;
  } else if (cal == 9) {
    result = 11;
  } else {
    result = cal;
  }
  return result;
}

String processor(const String& var) {
  if (var == "TEMP") {
    dataState = String(t);
  }
  else if (var == "HUMIDITY") {
    dataState = String(h);
  }
  else if (var == "VOLTAGE") {
    dataState = String(v);
  }
  else if (var == "FUEL") {
    dataState = String(f);
  } else {}
  return String(dataState);
}
