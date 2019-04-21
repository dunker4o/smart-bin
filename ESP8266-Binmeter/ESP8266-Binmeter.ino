/*
 *  ESP8266 Sketch that reads the distance from one ultrasound snesor and 
 *  sends it over HTTP on demand.
 * 
 *  Authors: Borislav Gachev, Ruigang Chen, Ronaldas Gadzimugometovas
 *  Date: Easter 2019
 *  
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * @Authors wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.   B,R,R
 * ----------------------------------------------------------------------------
 *  
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define AP_SSID "SmartBin"
#define AP_PASS "Binmaster3000"

ESP8266WebServer server(80);

//TODO: Include pseudo-code that would read from the HC-SR04 sensor based on schematics and earlier sketch


void handleRequest(){
  String html = "<h3>Welcome to our simple ESP8266 page :)</h3>";
  server.send(200, "text/html", html);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  Serial.println();
  
  Serial.print("Starting Access Point on ESP8266.");
  WiFi.softAP(AP_SSID, AP_PASS);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("ESP8266 IP: ");
  Serial.println(myIP);
  
  server.on("/", handleRequest);
  server.begin();
  
  Serial.println("Request handler started.");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
