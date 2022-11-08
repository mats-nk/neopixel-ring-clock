#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIN 4 //D2
#define NUM_PIXELS 24 // Number of pixels in the ring

const char *ssid     = "***"; // Wifi name
const char *password = "***"; // Wifi password

const long utcOffsetInSeconds = 3600; // UTC Offset of timezone -- 3600 for CET

Adafruit_NeoPixel pixels(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800); // Initialise NeoPixel Library

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  pixels.begin();
  pixels.clear();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();
  pixels.clear();

  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  int hour_pixel = (int) roundf((float) 30 * (float) timeClient.getHours() / (float) 360 * (float) NUM_PIXELS);

  int minute_pixel = (int) roundf((float) 6 * (float) timeClient.getMinutes() / (float) 360 * (float) NUM_PIXELS);

  int second_pixel = (int) roundf((float) 6 * (float) timeClient.getSeconds() / (float) 360 * (float) NUM_PIXELS);

  Serial.print(hour_pixel);
  Serial.print(",");
  Serial.print(minute_pixel);
  Serial.print(",");
  Serial.println(second_pixel);
  
  for(int i = 0; i < NUM_PIXELS; i++) {
    if (i + 1 == second_pixel) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }
  
    if (i + 1 == minute_pixel) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 150));
    }

    if (i + 1 == hour_pixel) {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    }
    
  }
  
  pixels.show();
  delay(1000);
}