/*
  ESP8266 Connect to the internet by Rakshith Vasudev.

*/

#include "ESP8266WiFi.h"
#define REGULAR_CHECK 15*60*1000                  // Keep checking for the presence of internet at regular intervals defined by REGULAR_CHECK(ms).
#define WAIT_BEFORE_SECOND_REBOOT 4.5*60*1000     // Time to wait for the modem to be completely active after a restart.
#define BLIPS_DELAY 1*30*1000                     // Time to transistion between off and on : to clear buffers on 3G cellular and Wifi modem both.
#define WIFI_BREAK 1*60*1000                      // time to break if there isn't a wifi so that it doesn't keep trying to connect forever.

const char* ssid = "Guest";                   // Enter SSID.
const char* password = "Pandaexp!";           // Enter Password.
const char* host = "www.google.com";          // Enter Host to check for internet.
int relayPin = 2;                             // Define output pin for relay .
bool wificonnected = false;                   // Status to track for wifi availability.
bool hostconnected = false;                   // Status to track for internet availability.
WiFiClient client;                            // Internet connector.
bool firsttime = true;                        // This is required to ensure that the modem doesn't reboot the first time.



// https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html

void setup(void)
{
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);       // define pin 2 as output

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
   Power on and off the port.
*/
void restartpower() {

  Serial.println("\nRestarting device..");
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
   Try connecting to the internet and update the status of the host.
*/
void internetconnect()
{

  Serial.printf("\n[Connecting to %s ... ", host);

  // connect to the internet for the specified host.
  if (client.connect(host, 80))
  {
    Serial.println("connected!");
    hostconnected = true;
  }
  else
  {
    Serial.println("connection failed!");
    hostconnected = false;
  }
}





/**
    set relay status appropriately. If there is no internet, then turn the relay on.
**/
void checkrelay() {
  if (wificonnected == true && hostconnected == true) {

    // turn on power
    poweron();

  } else if (wificonnected != true || hostconnected != true) {
    // restart the system with timeset for modem come back.
    restartpower();
    Serial.println("Restarted board!");
  }
}


/**
    Update global wifi and host variables after a restart.
**/
void updatevariables() {

  // check if Wifi is really connected
  if (WiFi.status() == WL_CONNECTED) {
    wificonnected = true;
  } else {
    wificonnected = false;
  }

  // check if client is connected
  if (!client.connected() || !client.available()) {
    hostconnected = false;
  } else if (client.connected() || client.available()) {
    hostconnected = true;
  }
}


/**
   Check the availability of the wifi by directly conencting to the client.
*/
bool iswificonnected() {
  return WiFi.status() == WL_CONNECTED;
}

/**
   Check the availability of the internet by directly conencting to the client.
*/
bool isinternetconnected() {
  return (client.connected() || client.available());
}


void loop()
{

  // first connect to wifi and don't reboot
  if (!iswificonnected() && firsttime) {

    Serial.println("In 1!");

    // try to connect to the wifi
    wificonnect();

    //try to connect to the internet
    internetconnect();

    // update the state of the variables.
    updatevariables();

    // update to set the first time.
    firsttime = false;
  }




  // if wifi not connected then make the global connection be false, restart the device
  if (!iswificonnected() && !firsttime) {

    Serial.println("In 2");
    wificonnected = false;

    // try to connect to the wifi, this will take care of wifi restart
    wificonnect();

    // update the state of the variables.
    updatevariables();
  }


  // check if Wifi and internet is really connected
  // check if internet is really available: cases where there is wifi but no internet

  if (iswificonnected()) {
    wificonnected = true;
    if (!client.connected() || !client.available()) {
      Serial.println("In 3!");

      hostconnected = false;

      // try to the connect to the internet.
      internetconnect();

      // turn on power for relay if there's isn't any internet.
      checkrelay();

      // update the state of the variables.
      updatevariables();
    }
  }

  Serial.println("In the regular 2 second regular check");
  delay(REGULAR_CHECK);
  
}
