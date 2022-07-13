
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
String LEDState = "off";

//var to track time
unsigned long time_milli;
unsigned long timeSinceLastModeButtonPress = 0;
unsigned long timeBtwLastModeButtonPress  = 100;

bool flag;

long var = 0;

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
  //Serial.println("New void loop started");
  time_milli = millis();

  WiFiClient client = server.available();

  /*if (M5.Btn.wasPressed()) {
    Serial.println("Button pressed ------------------------");
    if (time_milli - timeSinceLastModeButtonPress > timeBtwLastModeButtonPress) {
      timeSinceLastModeButtonPress  = time_milli;
      Serial.println("Time check passed");

      //flags to update teh webpage
      client = true;
      flag = true;

      if(LEDState == "on"){
        Serial.println("LED State was on - turnign off");
        LEDState == "off";
        M5.dis.clear();
      }
      else if(LEDState == "off"){
        Serial.println("LED State was off -> turning on");
        LEDState = "on";
        M5.dis.fillpix(colors[1]);
      }
    }
  }*/

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //INTERESTING PART 2
          Serial.println("I AM HIGH");
          client.println("I AM HIGH");
        }
        if (currentLine.endsWith("GET /L")) {
          //INTERESTING PART 3
          Serial.println("I AM LOW");
          client.println("I AM LOW");
        }

        if (currentLine.endsWith("GET /home")) {
          Serial.println("Home");

          // html and javascript code sent to the browser
          /*

            <!DOCTYPE HTML>
            <html>
            <head>
            <script type="text/javascript">
            window.onload = function() {
            httpGetAsync("/value");
            function httpGetAsync(theUrl) {
            var xmlHttp = new XMLHttpRequest();
            var frequency_val = 1000;
            xmlHttp.onreadystatechange = function() {
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
                document.getElementById("val").innerHTML = xmlHttp.responseText; }
            xmlHttp.open("GET", theUrl, true);
            xmlHttp.send(null);

            setTimeout(function(){httpGetAsync("/value")}, frequency_val); } }
            </script>

            </head>
            <body>
            <h2 style="font-size: 38px; display: inline; color: gray">LEVEL: </h2>
            <h2 id="val" style="font-size: 36px; display: inline; color:maroon"> </h2>
            <p style=\"font-size: 36px; display: inline; color:blue\">Click <a href=\"/H\">here</a> to turn ON the LED.</p> <br>
            <p style=\"font-size: 36px; display: inline; color:green\">Click <a href=\"/L\">here</a> to turn OFF the LED.</p> <br>

            </body>
            </html>

          */

          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<script type=\"text/javascript\">");
          client.println("window.onload = function() {");
          client.println("httpGetAsync(\"/value\");");
          client.println("function httpGetAsync(theUrl) { ");
          client.println("var xmlHttp = new XMLHttpRequest();");
          client.println("var frequency_val = 1000;");
          client.println("xmlHttp.onreadystatechange = function() { ");
          client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
          client.println("document.getElementById(\"val\").innerHTML = xmlHttp.responseText; }");
          client.println("xmlHttp.open(\"GET\", theUrl, true); ");
          client.println("xmlHttp.send(null)");
          client.println("setTimeout(function(){httpGetAsync(\"/value\")}, frequency_val); } }");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2 style=\"font-size: 38px; display: inline; color: gray\">Value: </h2> ");
          client.println("<h2 id=\"val\" style=\"font-size: 36px; display: inline; color:maroon\"> </h2> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">Click <a href=\"/H\">here</a> to turn ON the LED.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:green\">Click <a href=\"/L\">here</a> to turn OFF the LED.</p> <br>");
          client.println("</body>");
          client.println("</html>");
        }

        if (currentLine.endsWith("GET /value")) {
          var++;
          Serial.println(var);
          client.println(var);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }



M5.update();
}
