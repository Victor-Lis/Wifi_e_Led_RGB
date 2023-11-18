/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <iostream>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define PIN_RED    23 // GPIO23
#define PIN_GREEN  22 // GPIO22
#define PIN_BLUE   21 // GPIO21

// Set these to your desired credentials.
const char *ssid = "esp32_server";
const char *password = "esp32@server";

WiFiServer server(80);


void setup() {
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  
  IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
    
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    //Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<!DOCTYPE html><html lang='pt-BR'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Leds</title></head><style>* {margin:0;padding:0;border:0;font-family:sans-serif;}body {background-color:#202020;display:flex;flex-direction:column;align-items:center;justify-content:space-around;min-height:80vh;padding:10vh 0;}input {background-color:#202020;width:100px;height:100px;border-radius:100px;}button {width:100px;height:30px;text-transform:uppercase;font-weight:bold;font-size:15px;border:2px solid #fff;background:transparent;color:#fff;transition:0.25s;&:hover {transition:0.5s;opacity:0.6;cursor:pointer;}}</style><body><input type='color' id='color'><button onclick='getColor()'>Enviar!</button></body><script>let currentURL = window.location.href;function setColor() {if (currentURL.includes('?rgb=')) {let rgb = currentURL.split('?rgb=')[1];let hex = rgbToHex(rgb);let red = rgb.slice(0, 3);let green = rgb.slice(3, 6);let blue = rgb.slice(6, 9);document.querySelector('button').style.border = '2px solid rgb(' + red + ', ' + green + ', ' + blue + ')';}}function getColor() {let color = document.querySelector('#color').value;let a = document.createElement('a');a.setAttribute('href', '?rgb=' + hexToRgb(color));a.click();}function hexToRgb(hex) {hex = hex.replace('#', '');let [r, g, b] = hex.match(/[0-9a-f]{2}/g);r = parseInt(r, 16);g = parseInt(g, 16);b = parseInt(b, 16);if (`${r}`.length == 2) {r = '0' + r;} else if (`${r}`.length == 1) {r = '00' + r;}if (`${g}`.length == 2) {g = '0' + g;} else if (`${g}`.length == 1) {g = '00' + g;}if (`${b}`.length == 2) {b = '0' + b;} else if (`${b}`.length == 1) {b = '00' + b;}return `${r}${g}${b}`;}function rgbToHex(rgb) {let r = rgb[0].toString(16);let g = rgb[1].toString(16);let b = rgb[2].toString(16);r = r.padStart(2, '0');g = g.padStart(2, '0');b = b.padStart(2, '0');return '#' + r + g + b;}setColor();</script></html>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if ((currentLine.indexOf("Referer")+1) && currentLine.length() == 42) {
          Serial.println(currentLine);
          String red = String(currentLine[33]) + String(currentLine[34]) + String(currentLine[35]);

          String green = String(currentLine[36]) + String(currentLine[37]) + String(currentLine[38]);

          String blue= String(currentLine[39]) + String(currentLine[40]) + String(currentLine[41]);

          analogWrite(PIN_RED, red.toInt());
          analogWrite(PIN_GREEN, green.toInt());
          analogWrite(PIN_BLUE, blue.toInt());
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("Client Disconnected.");
  }
}
