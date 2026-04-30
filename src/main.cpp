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

unsigned long prevMilliSec = 0; // this will be used to check when last the previous step happened
const long interval = 300;      // each interval between steps in loops is set to 300ms
int xmasStep = 0;               // this will be used to loop through different light combos in an if elif elif statement

void turnAllOff()
{
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  greenLEDState = false;
  yellowLEDState = false;
  redLEDState = false;
};

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80); // Port 80 is standard for HTTP unencrypted websites

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

  // Xmas cycles through 3 different frames/steps to simulate christmas lights!
  server.on("/toggleXmas", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                xmasState = !xmasState;
                xmasStep = 0;

                if (xmasState == true)
                {
                  xmasStep = 0;
                  prevMilliSec = millis();
                  
                }
                else
                {
                  turnAllOff();
                }

                // Print to the Serial Monitor to verify it worked
                Serial.println("\n=============\nXMAS TOGGLED!\n=============\n");
            
                request->send(200, "text/plain", "Success"); });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html"); // tells server that it must retrieve index.html by default
  // Start the server
  server.begin();
  Serial.println("Server started!");
}

void loop()
{
  // Xmas lights loop

  if (xmasState == true)
  {
    unsigned long currentMilliSec = millis();

    if (currentMilliSec - prevMilliSec >= interval)
    {                                 // Check to see if the interval has passed
      prevMilliSec = currentMilliSec; // update to current MilliSec
      if (xmasStep == 0)
      {
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        redLEDState = true;
        yellowLEDState = true;
        greenLEDState = false;
        xmasStep = 1;
      }
      else if (xmasStep == 1)
      {
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, HIGH);
        redLEDState = false;
        yellowLEDState = true;
        greenLEDState = true;
        xmasStep = 2;
      }
      else if (xmasStep == 2)
      {
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        redLEDState = true;
        yellowLEDState = false;
        greenLEDState = true;
        xmasStep = 0;
      }
    }
  }
}