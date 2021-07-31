

#include <WiFi.h>

#define LED_BUILTIN 2

const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

String header;
String LEDstate = "off"; 

void setup() {
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println("Configuring access point...");

 
 Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}



void loop() {
  WiFiClient client = server.available();   
  if (client) {                             
    Serial.println("New Client.");         
    String currentLine = "";              
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                  

          if (currentLine.length() == 0) {
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

          }
        
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);              
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                
        }

// Display the HTML web page

 client.println("<!DOCTYPE html><html>");
 client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
 client.println("<link rel=\"icon\" href=\"data:,\">");
 
 // CSS to style the on/off buttons 
           
 client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
 client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
 client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
 client.println(".button2 {background-color: #555555;}</style></head>");
            
     // Web Page Heading        
      client.println("<body><h1>ESP32 Web Server</h1>");
      client.println("</body></html>");
         // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  
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
}
