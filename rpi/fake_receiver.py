import RPi.GPIO as GPIO
from enum import Enum
import time

class MessageType(Enum):
    UNKNOWN = 0
    ALIVE = 1
    WINDOW_OPEN = 2

class FakeMessage:
    def __init__(self, id, type, payload):
        self.id = id
        self.type = type
        self.payload = payload

class FakeReceiver:
    def __init__(self, powerPin, pin):
        self.id = 404
        self.aliveInterval = 10
        self.lastAlive = 0
        self.pressed = False

        self.powerPin = powerPin
        self.pin = pin
        self.isOpened = False

        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.powerPin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.setup(self.pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

    def __sendMessage(self, type):
        if type == MessageType.WINDOW_OPEN:
            return FakeMessage(self.id, MessageType.WINDOW_OPEN, self.isOpened)
        elif type == MessageType.ALIVE:
            return FakeMessage(self.id, MessageType.ALIVE, self.isOpened)
        return None


    def getMessage(self):
        #device is turned off
        if GPIO.input(self.powerPin) == GPIO.LOW:
            return None

        if not self.pressed and GPIO.input(self.pin) == GPIO.HIGH:
            self.isOpened = not self.isOpened
            self.pressed = True
            return self.__sendMessage(MessageType.WINDOW_OPEN)
        elif GPIO.input(self.pin) == GPIO.LOW:
            self.pressed = False

        now = time.time()
        if now - self.lastAlive > self.aliveInterval:
            self.lastAlive = now
            return self.__sendMessage(MessageType.ALIVE)
