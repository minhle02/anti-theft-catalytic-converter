from machine import UART, Pin
from time import sleep


class SIM800:
    error = "ERROR"
    ok = "OK";
    
    def __init__(self, reset_pin = 18, pin = 2, baud = 115200):
        self.reset_pin = Pin(reset_pin, Pin.OUT)
        self.reset_pin.value(1) # logic HIGH to prevent reset
        self.pin = pin;
        self.baud = baud;
        self.uart = UART(pin, baudrate=baud)
        
    def extractInfo(self, data):
        '''data read from SIM800 after writing command isn't readable,
        this function converts it to conventional string for outputing'''
        output = '';
        if (data == ""):
            return output
        if (data[0] == 'b'):
            data = data[1:len(data)]
            
        newlineYet = False;
        for c in data:
            if (c == "'"):
                continue
            if (c == '\r' or c == '\n'):
                newlineYet = True
                continue
            if (newlineYet):
                output += "\n"
            newlineYet = False;
            output += c;
        return output
    
    def writeCommand(self, command):
        "write command to module and return output"
        self.uart.write(command)
        sleep(1)
        data = self.uart.read();
        if (data == None):
            return "";
        output = self.extractInfo(data.decode())
        return output
    
    def checkStatus(self):
        "check status of modem"
        output = self.writeCommand("AT\r");
        if (output == ""):
            print("UART communication isn't set up correctly")
            return "ERROR"
        list_str = output.split("\n");
        # print(list_str[-1])
        return list_str[-1];
    
    def setUp(self):
        try:
            print("Initializing board...")
            stat = self.checkStatus()
            count = 0
            while (stat != "OK"):
                count = count + 1
                self.reset_pin.value(0)
                sleep(1)
                self.reset_pin.value(1)
                print("Re-initializing board...")
                sleep(20)
                stat = self.checkStatus()
                if (count > 3): 
                    raise RuntimeError("Cannot initialize board")
            print("Connection established")
                
        except RuntimeError as e:
            print(e)
    
    def makePhoneCall(self, phoneNum):
        out = self.writeCommand("ATD+ " + phoneNum + ";\r")
        print(out)
    
    def smsMessaging(self, phoneNum, text):
        success = True
        for i in range(3):
            if (self.checkStatus() == "OK"):
                sucess = False
                break
            sleep(8)
            print("...")
        if (not success):
            print("Board isn't ready")
            return
    
        for i in range(3):
            out = self.writeCommand("AT+CMGF=1\r")
            if (out.split("\n")[-1] == "OK"):
                print(self.writeCommand("AT+CMGS=\"" + phoneNum + "\"\r"))
                print(self.writeCommand(text + "\x1a"))
                return;
        print("Cannnot send SMS")
                
            
            
        



    
    
        
    
            
        

