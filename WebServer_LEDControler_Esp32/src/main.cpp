#include <Arduino.h>
#include <WiFi.h>              //This is to connect to wifi router broadcast on an open port
#include <ESPAsyncWebServer.h> //This is to host the site
#include <LittleFS.h>          // This is to manage files, now interacting with data file

const char *ssid = "Tell my WiFi love her";
const char *password = "8008135!";

const int greenLED = 5;
const int yellowLED = 18;
const int redLED = 19;
bool greenLEDState = false;
bool yellowLEDState = false;
bool redLEDState = false;
bool xmasState = false;

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup()
{
  // Physical Setup
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  Serial.begin(115200);

  //
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin())
  {
    Serial.println("LittleFS Error");
    return;
  } // loading littleFS, catching error

  // 1. Listen for someone hitting the "/toggleGreenLED" URL
  server.on("/toggleGreenLED", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                // 2. Flip the state tracker (if it was false, make it true, and vice versa)
                greenLEDState = !greenLEDState;

                // 3. The Hardware Action: Physically change the voltage on Pin 5
                if (greenLEDState == true)
                {
                  digitalWrite(greenLED, HIGH); // Turn it ON
                }
                else
                {
                  digitalWrite(greenLED, LOW); // Turn it OFF
                }

                // Print to the Serial Monitor to verify it worked
                Serial.println("\n=============\nGREEN TOGGLED!\n=============\n");

                // 4. The Receipt: Tell the browser "Message received, status 200 OK"
                // If you forget this, your webpage will freeze up!
                request->send(200, "text/plain", "Success"); });

  server.on("/toggleYellowLED", HTTP_GET, [](AsyncWebServerRequest *request)
            {
         
                yellowLEDState = !yellowLEDState;

                if (yellowLEDState == true)
                {
                  digitalWrite(yellowLED, HIGH); // Turn it ON
                }
                else
                {
                  digitalWrite(yellowLED, LOW); // Turn it OFF
                }

// Print to the Serial Monitor to verify it worked     
                Serial.println("\n=============\nYELLOW TOGGLED!\n=============\n");

                request->send(200, "text/plain", "Success"); });

  server.on("/toggleRedLED", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                redLEDState = !redLEDState;

                if (redLEDState == true)
                {
                  digitalWrite(redLED, HIGH); // Turn it ON
                }
                else
                {
                  digitalWrite(redLED, LOW); // Turn it OFF
                }

                // Print to the Serial Monitor to verify it worked
                Serial.println("\n=============\nRED TOGGLED!\n=============\n");
            
                request->send(200, "text/plain", "Success"); });

  server.on("/toggleXmas", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                xmasState = !xmasState;

                if (xmasState == true)
                {
                  for (int i = 0; i < 10; i++)
                  {
                    digitalWrite(redLED, HIGH);
                    digitalWrite(yellowLED, HIGH);
                    digitalWrite(greenLED, LOW);
                    delay(300);

                    digitalWrite(redLED, LOW);
                    digitalWrite(yellowLED, HIGH);
                    digitalWrite(greenLED, HIGH);
                    delay(300);

                    digitalWrite(redLED, HIGH);
                    digitalWrite(yellowLED, LOW);
                    digitalWrite(greenLED, HIGH);
                    delay(300);
                  }
                  xmasState = false;
                  
                }
                else
                {
                  digitalWrite(redLED, LOW);
                  digitalWrite(redLED, LOW);
                  digitalWrite(redLED, LOW); // Turn it OFF
                }

                // Print to the Serial Monitor to verify it worked
                Serial.println("\n=============\nXMAS TOGGLED!\n=============\n");
            
                request->send(200, "text/plain", "Success"); });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  // Start the server
  server.begin();
  Serial.println("Server started!");
}

void loop()
{
  // Still nothing needed here! The server runs in the background.
}