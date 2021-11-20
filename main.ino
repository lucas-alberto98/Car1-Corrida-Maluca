#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "BOTZ"; //Enter SSID
const char* password = "robots123"; //Enter Password
const char* websockets_server = "http://10.0.0.100:8080"; //server adress and port

#define MOTOR1_PIN1 36
#define MOTOR1_PIN2 39
#define MOTOR1_VELOCITY 24

#define MOTOR2_PIN1 34
#define MOTOR2_PIN2 35
#define MOTOR2_VELOCITY 25

int left = 0;
int right = 0;

using namespace websockets;
WebsocketsClient client;

void onMessageCallback(WebsocketsMessage message) {
    StaticJsonDocument<200> doc;


    Serial.print("Got Message: ");
    char data[] = message.getData();

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    char* mode = doc["mode"];

    if(mode === "server"){
       int left = doc["left"];

       if(left > 0){
            digitalWrite(MOTOR1_PIN1, HIGH);
            digitalWrite(MOTOR1_PIN2, LOW);
            analogWrite(MOTOR1_VELOCITY, left);
        }
        else if(left < 0){
            digitalWrite(MOTOR1_PIN1, LOW);
            digitalWrite(MOTOR1_PIN2, HIGH);
            analogWrite(MOTOR1_VELOCITY, (left * (-1)));
        }
        else{
            digitalWrite(MOTOR1_PIN1, LOW);
            digitalWrite(MOTOR1_PIN2, LOW);
            analogWrite(MOTOR1_VELOCITY, 0);
        }

        int right = doc["right"];

        if(right > 0){
            digitalWrite(MOTOR2_PIN1, HIGH);
            digitalWrite(MOTOR2_PIN2, LOW);
            analogWrite(MOTOR2_VELOCITY, right);
        }
        else if(right < 0){
            digitalWrite(MOTOR2_PIN1, LOW);
            digitalWrite(MOTOR2_PIN2, HIGH);
            analogWrite(MOTOR2_VELOCITY, (right * (-1)));
        }
        else{
            digitalWrite(MOTOR2_PIN1, LOW);
            digitalWrite(MOTOR2_PIN2, LOW);
            analogWrite(MOTOR2_VELOCITY, 0);
        }
     
    }
}


void setup() {
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
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
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
    client.send("Hi Server!");
    // Send a ping
    client.ping();


}

void loop() {
    client.poll();
}