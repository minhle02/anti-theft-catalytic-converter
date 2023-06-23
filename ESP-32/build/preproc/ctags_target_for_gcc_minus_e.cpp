# 1 "/Users/francis-hung/Documents/Code/Senior_Design/ESP-32/ESP-32.ino"

# 3 "/Users/francis-hung/Documents/Code/Senior_Design/ESP-32/ESP-32.ino" 2



# 7 "/Users/francis-hung/Documents/Code/Senior_Design/ESP-32/ESP-32.ino" 2

const char *ssid = "SPU-Wireless";
const char *password = "SPU-Wireless";

// Values from Twilio
static const char *account_sid = "ACa6463eb61ef853a11e0fdc643f45c599";
static const char *auth_token = "9f81a4182da2bb2df4241ae21b1e3d2d";

// Phone Number
static const char *from_number = "+18339521285";
static const char *to_number = "+12064463571";

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
        pinMode(pin, 0x01);
    }

    for (int pin : MOTION_PINS)
    {
        pinMode(pin, 0x01);
    }
    for (int pin : LED)
    {
        pinMode(pin, 0x03);
        digitalWrite(pin, 0x0);
    }
    pinMode(ONBOARD_LED, 0x03);
    pinMode(RELAY, 0x03);
    digitalWrite(ONBOARD_LED, 0x0);
    digitalWrite(RELAY, 0x0);
    WiFiInit();
    digitalWrite(LED[0], 0x1);
    digitalWrite(LED[1], 0x1);
    xTaskCreatePinnedToCore(task0Code, "Task 0", 1000, 
# 63 "/Users/francis-hung/Documents/Code/Senior_Design/ESP-32/ESP-32.ino" 3 4
                                                      __null
# 63 "/Users/francis-hung/Documents/Code/Senior_Design/ESP-32/ESP-32.ino"
                                                          , 1, &Task0, 0);
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
        if (digitalRead(pin) == 0x0)
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
        if (digitalRead(pin) == 0x1)
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
    WiFi.mode(WIFI_MODE_STA); // Optional
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
    digitalWrite(RELAY, 0x1);

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
    digitalWrite(RELAY, 0x0);
}

void task0Code(void *pvParameter)
{
    while (1)
    {
        if (ledOn)
        {
            digitalWrite(LED[0], 0x1);
            digitalWrite(LED[1], 0x0);
            delay(250);
            digitalWrite(LED[0], 0x0);
            digitalWrite(LED[1], 0x1);
            delay(250);
        }
        else
        {
            digitalWrite(LED[0], 0x0);
            digitalWrite(LED[1], 0x0);
        }
    }
}
