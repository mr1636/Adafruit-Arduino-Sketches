/***************************************************
  Adafruit IO Trigger Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Todd Treece for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Adafruit_IO_Client.h"

// door gpio pin
#define DOOR 13

// how long to sleep between checking the door state (in seconds)
#define SLEEP_LENGTH 3

// wifi credentials
#define WLAN_SSID       "NAME"
#define WLAN_PASS       "PASS"

// configure Adafruit IO access
#define AIO_KEY         "AIO KEY"

// create an Adafruit IO client instance
WiFiClient client;
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);

void setup() {
  Serial.begin(115200);
  Serial.println("HUZZAH Trigger Basic");

  pinMode(DOOR, INPUT_PULLUP);



}

unsigned long lastTime = millis();
unsigned long checkInterval = 360 * 1000;

void loop() {

  // if door isn't open, we don't need to send anything
  if (digitalRead(DOOR) == LOW) {
    Serial.println("Door closed");
    if(millis() - lastTime > checkInterval){
      door_closed();
      lastTime = millis(); 
    }
  } else {
    // the door is open if we have reached here,
    // so we should send a value to Adafruit IO.
    Serial.println("Door is open!");
    door_open();
  }

  delay( SLEEP_LENGTH * 1000 );
}

// connect to wifi network. used by
// door and battery functions.
void wifi_init() {

  // wifi init
  Serial.println("Starting WiFi");
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.write('.');
    delay(500);
  }

  // AIO init
  Serial.println("Connecting to Adafruit.io");
  aio.begin();

}

void door_open() {

  // turn on wifi if we aren't connected
  if (WiFi.status() != WL_CONNECTED) {
    wifi_init();
  }

  // grab the door feed
  Adafruit_IO_Feed door = aio.getFeed("FEED NAME");

  Serial.println("Sending door open to Adafruit.io");
  // send door open value to AIO
  door.send("1");

}

void door_closed() {

  // turn on wifi if we aren't connected
  if (WiFi.status() != WL_CONNECTED) {
    wifi_init();
  }

  // grab the door feed
  Adafruit_IO_Feed door = aio.getFeed("FEED NAME");

  Serial.println("Sending door working to Adafruit.io");
  // send door open value to AIO
  door.send("1");

}
