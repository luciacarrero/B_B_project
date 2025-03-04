/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include "U8x8lib.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

//#define SCREEN_WIDTH 128  // OLED display width, in pixels
//#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x78, 0xE3, 0x6D, 0x13, 0x8C, 0x50};

// Define variables to store BME280 readings to be sent

// Define variables to store incoming readings
float incomingTemp;
float incomingHum;

// Variable to store if sending data was successful
String success;



//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float temp;
    float hum;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

/*esp_now_peer_info_t peerInfo;

Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}*/

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingTemp = incomingReadings.temp;
  incomingHum = incomingReadings.hum;
}
 
void setup() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  // Init Serial Monitor
  Serial.begin(115200);

  // Init BME280 sensor

  // Init OLED display
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    u8x8.drawString(0,0,"error");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  //getReadings();
 
  // Set values to send
 /* BME280Readings.temp = temperature;
  BME280Readings.hum = humidity;
  BME280Readings.pres = pressure;*/

  // Send message via ESP-NOW
  /*esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &BME280Readings, sizeof(BME280Readings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }*/
  updateDisplay();
  delay(10000);
}
/*void getReadings(){
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = (bme.readPressure() / 100.0F);
}*/

void updateDisplay(){

  static char tempchar[6];
  static char humchar[6];
  // Display Readings on OLED Display
  dtostrf(incomingReadings.temp,6,1,tempchar);
  dtostrf(incomingReadings.hum,6,2,humchar);
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Temperature: ");
  u8x8.drawString(0,0,"Temperature");
  Serial.print(incomingReadings.temp);
  u8x8.drawString(0,1,tempchar);
  Serial.println(" ºC");
  u8x8.drawString(7,1, "C");
  Serial.print("Humidity: ");
  u8x8.drawString(0,2,"Humidity:");
  Serial.print(incomingReadings.hum);
  u8x8.drawString(0,3,humchar);  
  Serial.println(" %");
  u8x8.drawString(7,3,"%");
}
