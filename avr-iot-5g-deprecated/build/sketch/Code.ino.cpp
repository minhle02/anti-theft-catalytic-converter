#line 1 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
#include <log.h>
#include <lte.h>
#include <sequans_controller.h>
#include <string.h>
#include <Arduino.h>
#include <led_ctrl.h>

/*Constant declaration*/
static const long BAUD_RATE = 115200;
static const String PHONE_NUMBER = "+12063845010";

#line 12 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
void SMSMessaging(String phoneNum, String content);
#line 33 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
void setup();
#line 51 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
void loop();
#line 12 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
void SMSMessaging(String phoneNum, String content) {
    Lte.begin();
    Log.info("Send SMS with message: " + content);
    String command = "AT+SQNSMSSEND=";
    command += "\"" + phoneNum + "\",";
    command += "\"" + content + "\"";
    char *passcommand = &command[0];
    char response[128] = "";
    ResponseResult res = SequansController.writeCommand(passcommand, response, sizeof(response));

    if (res == ResponseResult::OK)
    {
        Log.info("Send success");
    }
    else
    {
        Log.info("Cannot send yet");
    }
    Lte.end();
}

void setup() {
    Log.begin(115200);
    LedCtrl.startupCycle();

    LedCtrl.beginManual();
    LedCtrl.off(Led::CELL);
    LedCtrl.off(Led::CON);
    LedCtrl.off(Led::DATA);
    LedCtrl.off(Led::ERROR);
    LedCtrl.off(Led::USER);

    pinConfigure(PIN_PE1, PIN_DIR_INPUT);
    pinConfigure(PIN_PE2, PIN_DIR_INPUT);
    Log.info("Start reading pin input");
    int readP1 = digitalRead(PIN_PE1);
    Log.info("Pin P1 val: " + readP1);
}

void loop() {
    
    if (digitalRead(PIN_PE1) == 1)
    {
        Log.info("Pin PE1 trigger");
        LedCtrl.on(Led::USER);
        delay(3000);
        LedCtrl.off(Led::USER);
    }
    else if (digitalRead(PIN_PE2) == 1)
    {
        Log.info("Pin PE2 trigger");
        LedCtrl.on(Led::USER);
        delay(3000);
        LedCtrl.off(Led::USER);
    }
}
