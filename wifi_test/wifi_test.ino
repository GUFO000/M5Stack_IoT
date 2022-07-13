
#include <WiFi.h>
#include <M5Atom.h>
#include <WiFiClient.h>
#include <WiFiAP.h>


const char ssid[10] = "M5Network";
const char password[10] = "123456789";

CRGB colors[] = {
  0xfe0000, // red
  0x1ED35E, // green
  0xFFFFFF, // white
  0x031cfc, // blue
  0x2090f7, //light blue
  0x137a10, //green
  0xc76316, // orange
  0xc71616, //red
};

WiFiServer server(80);
//IPAdress ip(192, 168, 0, 1);
//IPAdress gateway(192, 168, 0, 1);
//IPAdress subnet(255, 255, 255, 0);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

void setup() {

  M5.begin(true, false, true);

  //M5.dis.fillpix(colors[2]);
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(ip, gateway, subnet);
  //WiFi.begin();

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  M5.dis.drawpix(0,colors[0]);
  M5.dis.drawpix(5,colors[0]);
  M5.dis.drawpix(10,colors[0]);
  M5.dis.drawpix(15,colors[0]);
  M5.dis.drawpix(20,colors[0]);
}


void loop() {

  WiFiClient client = server.available();

  if (client){
    Serial.println("New client");
    String currentLine = "" ;//to store the response of the WiFiClient

    while(client.connected()){
      if(client.available()){
        char c = client.read();

        Serial.println("client response: ");
        Serial.print(c);

        header += c;

        if (c == '\n') {
          //now check newline content, is blank the request is onver
          if (currentLine.length()==0){

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //secion to turn stuff on in the hardware based on the requests
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              M5.dis.fillpix(colors[2]);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              M5.dis.clear();
            }
            //HTML page goes here
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head> ");
            //client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>My firt page hosted on M5</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>LED Matix State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            /*client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }*/
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
M5.update();
}
