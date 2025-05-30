#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char *ssid = "Ciao";
const char *password = "Leo235711";

const char *serverHost = "192.168.50.12";
const int serverPort = 3004;

WiFiUDP udp;

const int numPairs = 8;
const int startPin = 22;
const float distance = 0.05;

unsigned long triggerTimes[numPairs][2];
bool triggered[numPairs][2];
unsigned long lastDetectionTime[numPairs] = {0};

const int debounceTime = 200;

void connectToWiFi()
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n Connected to WiFi!");
}

void sendUDP(int sensore, float velocita)
{
    StaticJsonDocument<128> jsonDoc;
    jsonDoc["sensore"] = sensore;
    jsonDoc["velocita"] = velocita;

    char buffer[128];
    size_t len = serializeJson(jsonDoc, buffer);

    udp.beginPacket(serverHost, serverPort);
    udp.write((uint8_t *)buffer, len);
    udp.endPacket();
}

void setup()
{
    Serial.begin(115200);

    connectToWiFi();

    for (int i = 0; i < numPairs * 2; i++)
    {
        pinMode(startPin + i, INPUT_PULLUP);
    }

    for (int i = 0; i < numPairs; i++)
    {
        triggered[i][0] = false;
        triggered[i][1] = false;
    }

    udp.begin(12345);
}

void loop()
{
    unsigned long currentMillis = millis();

    for (int i = 0; i < numPairs; i++)
    {
        if (currentMillis - lastDetectionTime[i] < debounceTime)
        {
            continue;
        }

        int pinA = startPin + i * 2;
        int pinB = startPin + i * 2 + 1;

        int sensorA = digitalRead(pinA);
        int sensorB = digitalRead(pinB);

        if (sensorA == LOW && !triggered[i][0])
        {
            triggerTimes[i][0] = currentMillis;
            triggered[i][0] = true;
        }

        if (sensorB == LOW && !triggered[i][1])
        {
            triggerTimes[i][1] = currentMillis;
            triggered[i][1] = true;
        }

        if (triggered[i][0] && triggered[i][1])
        {
            unsigned long timeDiff = triggerTimes[i][1] - triggerTimes[i][0];

            if (timeDiff > 0)
            {
                float speed = distance / (timeDiff / 1000.0);

                Serial.print("Velocità sensore ");
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(speed, 2);
                Serial.println(" m/s");

                sendUDP(i + 1, speed);
            }

            triggered[i][0] = false;
            triggered[i][1] = false;

            lastDetectionTime[i] = currentMillis;
        }
    }
}
