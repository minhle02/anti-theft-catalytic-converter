from sim800 import SIM800

s = SIM800()
s.setUp()
s.smsMessaging("12063845010","Test")
