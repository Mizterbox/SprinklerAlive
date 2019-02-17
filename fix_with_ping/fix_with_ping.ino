#include "ESP8266WiFi.h"
#include <ESP8266Ping.h>
#define REGULAR_CHECK 1*30*1000                  // Keep checking for the presence of internet at regular intervals defined by REGULAR_CHECK(ms).
#define WAIT_BEFORE_SECOND_REBOOT 1*45*1000     // Time to wait for the modem to be completely active after a restart.
#define BLIPS_DELAY 1*30*1000                     // Time to transistion between off and on : to clear buffers on 3G cellular and Wifi modem both.
#define WIFI_BREAK 1*45*1000                      // time to break if there isn't a wifi so that it doesn't keep trying to connect forever.

const char* ssid = "MizterBox_453";          // Enter SSID.
const char* password = "79346605";           // Enter Password.
const char* host = "www.google.com";          // Enter Host to check for internet.
int relayPin = 4;                             // Define output pin for relay .
WiFiClient client;                            // Internet connector.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);       // define output pin as output
}
void wificonnect()
{
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // connect to WIFI
  Serial.print("Connecting to WIFI");
  // Connection process can take couple of seconds and we are checking
  // for this to complete in the following loop.
  unsigned long starttime, endtime;
  starttime  = millis();
  endtime = starttime;
  while (WiFi.status() != WL_CONNECTED)
  {
    // delay dots in printing
    delay(500);
    Serial.print(".");
    // restart the device if the wifi isn't found after WIFI_BREAK time
    if (endtime - starttime > WIFI_BREAK) {
      restartpower();
    }
    endtime = millis();
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
/**
   Power on the port: Turn the relay ON (low is ON if relay is LOW trigger. change it to HIGH if you have got HIGH trigger relay).
*/
void setlow() {
  digitalWrite(relayPin, LOW);
}
/**
   Power off the port
*/
void sethigh() {
  digitalWrite(relayPin, HIGH);
}
/**
   Avoid confusion - reverse states
*/
void poweroff() {
  sethigh();
}
/**
   Avoid confusion - reverse states
*/
void poweron() {
  setlow();
}
/**
   Power on and off the port.
*/
void restartpower() {
  Serial.println("Restarting device..");
  // power off
  sethigh();
  // add delay to notice the transistion
  delay(BLIPS_DELAY);
  
  // power on
  setlow();
  // wait for the modem to be completely active.
  delay(WAIT_BEFORE_SECOND_REBOOT);
  Serial.println("Restarting complete.");
}
void loop() {
  
  if(WiFi.status() != WL_CONNECTED){
    wificonnect();
  } 
  bool ret = Ping.ping(host, 10);
  
  while(!ret){
    restartpower();
    ret = Ping.ping(host, 10);
  }
  
  Serial.println(String("Connected to ")+String(host));
  Serial.println(String("In the regular periodic check of ") + String(REGULAR_CHECK) + String(" ms."));
  delay(REGULAR_CHECK);
}



#include "ESP8266WiFi.h"
#include <ESP8266Ping.h>
#define REGULAR_CHECK 1*30*1000                  // Keep checking for the presence of internet at regular intervals defined by REGULAR_CHECK(ms).
#define WAIT_BEFORE_SECOND_REBOOT 1*45*1000     // Time to wait for the modem to be completely active after a restart.
#define BLIPS_DELAY 1*30*1000                     // Time to transistion between off and on : to clear buffers on 3G cellular and Wifi modem both.
#define WIFI_BREAK 1*45*1000                      // time to break if there isn't a wifi so that it doesn't keep trying to connect forever.
const char* ssid = "MizterBox_453";          // Enter SSID.
const char* password = "79346605";           // Enter Password.
const char* host = "www.google.com";          // Enter Host to check for internet.
int relayPin = 4;                             // Define output pin for relay .
WiFiClient client;                            // Internet connector.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);       // define output pin as output
}
void wificonnect()
{
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // connect to WIFI
  Serial.print("Connecting to WIFI");
  // Connection process can take couple of seconds and we are checking
  // for this to complete in the following loop.
  unsigned long starttime, endtime;
  starttime  = millis();
  endtime = starttime;
  while (WiFi.status() != WL_CONNECTED)
  {
    // delay dots in printing
    delay(500);
    Serial.print(".");
    // restart the device if the wifi isn't found after WIFI_BREAK time
    if (endtime - starttime > WIFI_BREAK) {
      restartpower();
    }
    endtime = millis();
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
/**
   Power on the port: Turn the relay ON (low is ON if relay is LOW trigger. change it to HIGH if you have got HIGH trigger relay).
*/
void setlow() {
  digitalWrite(relayPin, LOW);
}
/**
   Power off the port
*/
void sethigh() {
  digitalWrite(relayPin, HIGH);
}
/**
   Avoid confusion - reverse states
*/
void poweroff() {
  sethigh();
}
/**
   Avoid confusion - reverse states
*/
void poweron() {
  setlow();
}
/**
   Power on and off the port.
*/
void restartpower() {
  Serial.println("Restarting device..");
  // power off
  sethigh();
  // add delay to notice the transistion
  delay(BLIPS_DELAY);
  
  // power on
  setlow();
  
  // wait for the modem to be completely active.
  delay(WAIT_BEFORE_SECOND_REBOOT);
  Serial.println("Restarting complete.");
}
void loop() {

  // Connect to wifi if wifi not connected 
  if(WiFi.status() != WL_CONNECTED){
    wificonnect();
  } 

  // try pinging 10 times before concluding the status of the internet   
  bool ret = Ping.ping(host, 10);

  // if there isn't a response from host, then restart device and try pinging again.
  while(!ret){
    restartpower();
    ret = Ping.ping(host, 10);
  }
  
  Serial.println(String("Connected to ")+String(host));
  Serial.println(String("In the regular periodic check of ") + String(REGULAR_CHECK) + String(" ms."));
  
  // wait until the regular time is done for the next time the loop runs.
  delay(REGULAR_CHECK);
}
