# 1 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino"
# 2 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2
# 3 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2
# 4 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2
# 5 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2
# 6 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2
# 7 "/Users/francis-hung/Documents/Code/avr-iot/Code/Code.ino" 2

/*Constant declaration*/
static const long BAUD_RATE = 115200;
static const String PHONE_NUMBER = "+12063845010";

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

    pinConfigure((31), PIN_DIR_INPUT);
    pinConfigure((32), PIN_DIR_INPUT);
    Log.info("Start reading pin input");
    int readP1 = digitalRead((31));
    Log.info("Pin P1 val: " + readP1);
}

void loop() {

    if (digitalRead((31)) == 1)
    {
        Log.info("Pin PE1 trigger");
        LedCtrl.on(Led::USER);
        delay(3000);
        LedCtrl.off(Led::USER);
    }
    else if (digitalRead((32)) == 1)
    {
        Log.info("Pin PE2 trigger");
        LedCtrl.on(Led::USER);
        delay(3000);
        LedCtrl.off(Led::USER);
    }
}
