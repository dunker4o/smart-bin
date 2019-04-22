/*
 *  ESP8266 Sketch that reads the distance from one ultrasound sensor and
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
#define ECHO_PIN 15
#define TRIG_PIN 12
#define PULSE_WAIT_TIME 15000 //in microseconds
#define MAX_PULSE_DURATION 2942 // multiply by 0.017 to get max distance [50cm atm]

long  duration;
int   distance;


void pollSensor(){
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, PULSE_WAIT_TIME);

  if(duration > 0){
    distance = duration * 0.017;
  }

}


void handleRequest(){

  pollSensor();

  String html ="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><title>Smart bin-meter</title><meta name=\"description\" content=\"Bin-meter readings\"><meta name=\"author\" content=\"DST 2019 Home automation team\"></head><body><h2 style=\"color: #5e9ca0;display: flex; flex-direction: column; justify-content: center; text-align: center;\">Welcome to our Bin-meter page :)</h2><div style=\"color: #2e6c80; display: flex; flex-direction: column; justify-content: center; text-align: center;\" id=\"distanceDiv\">Current cm distance to rubbish:<div id=\"distanceVal\">";
  html += distance;
  html +="</div></div><div style=\"color: #2e6c80; display: flex; flex-direction: column; justify-content: center; text-align: center;\" id=\"durationDiv\">Current pulse duration in ms:<div id=\"durationVal\">";
  html += duration;
  html +="</div></div></body></html>";

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

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
