
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <analogWrite.h>

const char *ssid = "BOTZ";                                //Enter SSID
const char *password = "robots123";                       //Enter Password
const char *websockets_server = "http://10.0.0.100:8080"; //server adress and port

#define MOTOR1_PIN1 33
#define MOTOR1_PIN2 32
#define MOTOR1_VELOCITY 26

#define MOTOR2_PIN1 13
#define MOTOR2_PIN2 12
#define MOTOR2_VELOCITY 25

int left = 0;
int right = 0;

using namespace websockets;
WebsocketsClient client;
StaticJsonDocument<200> doc;

void onMessageCallback(WebsocketsMessage message)
{
    // Serial.print("Got Message: ");
    // Serial.println(message.data());

    Serial.print("Got Message: ");
    Serial.println(message.data());
    String json = (String)message.data();

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    String mode = doc["mode"];

    Serial.println("mode" + mode);

    if (mode == "server")
    {
        int left = (int)doc["left"];

        if (left > 0)
        {
            digitalWrite(MOTOR1_PIN1, HIGH);
            digitalWrite(MOTOR1_PIN2, LOW);
            analogWrite(MOTOR1_VELOCITY, left, 255);
        }
        else if (left < 0)
        {
            digitalWrite(MOTOR1_PIN1, LOW);
            digitalWrite(MOTOR1_PIN2, HIGH);
            analogWrite(MOTOR1_VELOCITY, (left * (-1)), 255);
        }
        else
        {
            digitalWrite(MOTOR1_PIN1, LOW);
            digitalWrite(MOTOR1_PIN2, LOW);
            analogWrite(MOTOR1_VELOCITY, 0, 255);
        }

        int right = (int)doc["right"];

        if (right > 0)
        {
            digitalWrite(MOTOR2_PIN1, HIGH);
            digitalWrite(MOTOR2_PIN2, LOW);
            analogWrite(MOTOR2_VELOCITY, right, 255);
            Serial.println("testeee");
        }
        else if (right < 0)
        {
            digitalWrite(MOTOR2_PIN1, LOW);
            digitalWrite(MOTOR2_PIN2, HIGH);
            analogWrite(MOTOR2_VELOCITY, (right * (-1)), 255);
        }
        else
        {
            digitalWrite(MOTOR2_PIN1, LOW);
            digitalWrite(MOTOR2_PIN2, LOW);
            analogWrite(MOTOR2_VELOCITY, 0, 255);
        }
    }
}

void setup()
{
    pinMode(MOTOR1_PIN1, OUTPUT);
    pinMode(MOTOR1_PIN2, OUTPUT);
    pinMode(MOTOR1_VELOCITY, OUTPUT);

    pinMode(MOTOR2_PIN1, OUTPUT);
    pinMode(MOTOR2_PIN2, OUTPUT);
    pinMode(MOTOR2_VELOCITY, OUTPUT);

    Serial.begin(115200);
    // Connect to wifi
    Serial.println("conectando no wifi");
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println(WiFi.localIP());
    Serial.println("conectado com sucesso no wifi");

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    // client.onEvent(onEventsCallback);

    // Connect to server
    client.connect(websockets_server);

    // Send a message
    // client.send("Hi Server!");
    // Send a ping
    client.ping();
}

void loop()
{
    client.poll();
}