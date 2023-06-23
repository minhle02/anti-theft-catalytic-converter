#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "twilio.hpp"

const char *ssid = "";
const char *password = "";

// Values from Twilio
static const char *account_sid = "";
static const char *auth_token = "";

// Phone Number
static const char *from_number = "";
static const char *to_number = "";

// list of gpio
static const int CSM_PINS[] = {}; //13, 14

// list of motion sensor
static const int MOTION_PINS[] = {18}; //18, 19

// led control
static const int ONBOARD_LED = 2;
static const int LED[] = {32, 33};
bool ledOn = false;

// relay control
static const int RELAY = 4;

// multithread control
TaskHandle_t Task0;

Twilio *twilio;

void setup()
{
    Serial.begin(115200);
    Serial.println();
    for (int pin : CSM_PINS)
    {
        pinMode(pin, INPUT);
    }

    for (int pin : MOTION_PINS)
    {
        pinMode(pin, INPUT);
    }
    for (int pin : LED)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    pinMode(ONBOARD_LED, OUTPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(ONBOARD_LED, LOW);
    digitalWrite(RELAY, LOW);
    WiFiInit();
    digitalWrite(LED[0], HIGH);
    digitalWrite(LED[1], HIGH);
    xTaskCreatePinnedToCore(task0Code, "Task 0", 1000, NULL, 1, &Task0, 0);
}

void loop()
{
    Serial.println("Start up success");
    //digitalWrite(ONBOARD_LED, HIGH);
    CSMCheck();
    MotionCheck();
}

void CSMCheck()
{
    for (int pin : CSM_PINS)
    {
        if (digitalRead(pin) == LOW)
        {
            Serial.println("CSM detected");
            AlarmSequence();
        }
    }
}

void MotionCheck()
{
    for (int pin : MOTION_PINS)
    {
        if (digitalRead(pin) == HIGH)
        {
            Serial.println("Motion detected");
            AlarmSequence();
        }
    }
}

void SendSMS(const char *number, char *content)
{
    twilio = new Twilio(account_sid, auth_token);
    delay(1000);
    String response;
    bool success = twilio->send_message(number, from_number, content, response);
    if (success)
    {
        Serial.println("Sent message successfully!");
    }
    else
    {
        Serial.println(response);
    }
}

void WiFiInit()
{
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void AlarmSequence()
{
    ledOn = true;
    delay(100);
    digitalWrite(RELAY, HIGH);

    /* for (int i = 0; i < 4; i++) {
        digitalWrite(LED[0], HIGH);
        digitalWrite(LED[1], LOW);
        delay(250);
        digitalWrite(LED[0], LOW);
        digitalWrite(LED[1], HIGH);
        delay(250);
    } */

    char *content = "Theft Detected";
    SendSMS(to_number, content);
    ledOn = false;
    digitalWrite(RELAY, LOW);
}

void task0Code(void *pvParameter)
{
    while (1)
    {
        if (ledOn)
        {
            digitalWrite(LED[0], HIGH);
            digitalWrite(LED[1], LOW);
            delay(250);
            digitalWrite(LED[0], LOW);
            digitalWrite(LED[1], HIGH);
            delay(250);
        }
        else
        {
            digitalWrite(LED[0], LOW);
            digitalWrite(LED[1], LOW);
        }
    }
}
