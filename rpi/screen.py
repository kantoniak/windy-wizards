# -*- coding:utf-8 -*-

from luma.core.interface.serial import i2c, spi
from luma.core.render import canvas
from luma.core import lib

from luma.oled.device import sh1106
import RPi.GPIO as GPIO

import time
import subprocess
from fake_receiver import MessageType, FakeMessage, FakeReceiver

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

# Load default font.
font = ImageFont.load_default()

# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
width = 128
height = 64
image = Image.new('1', (width, height))

# First define some constants to allow easy resizing of shapes.
padding = -2
top = padding
bottom = height-padding
# Move left to right keeping track of the current x position for drawing shapes.
x = 0

RST = 25
CS = 8		
DC = 24

USER_I2C = 0

if  USER_I2C == 1:
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(RST,GPIO.OUT)	
	GPIO.output(RST,GPIO.HIGH)
	
	serial = i2c(port=1, address=0x3c)
else:
	serial = spi(device=0, port=0, bus_speed_hz = 8000000, transfer_size = 4096, gpio_DC = 24, gpio_RST = 25)

device = sh1106(serial) #sh1106  

try:
	receiver = FakeReceiver(17, 23)
	opened = False
	open_time = 0
	connected = True
	alive_interval = 10
	last_alive = time.time()
	alert_time = 30

	while True:
		with canvas(device) as draw:

			status = receiver.getMessage()
			if status != None:
				if status.type == MessageType.WINDOW_OPEN and status.payload != opened:
					opened = status.payload
					if opened:
						open_time = time.time()
				elif status.type == MessageType.ALIVE:
					last_alive = time.time()
					connected = True

			if time.time() - last_alive > alive_interval:
				connected = False
			

			# temp = -40.4
			# hum = 40.1

			timer = time.time() - open_time
			min = int(timer/60)
			sec = timer - min
			min_str = '{:02d}'.format(min)
			sec_str = '{:05.2f}'.format(sec).replace(".", ":")
			alert = timer > alert_time and opened
			
			if connected:
				lines = [
					f'Sensor connected',
					"",
					f'Windows is: {"OPENED" if opened else "CLOSED"}',
					# f'Temperature: {temp}ºC',
					# f'Humidity: {hum}%',
					f'Timer: {min_str}:{sec_str}' if opened else "",
					"",
					"CLOSE WINDOW!" if alert else ""
				]
			else:
				lines = [
					"",
					"",
					"   Sensor DISCONNECTED!"
				]

			y = top
			step = 8
			for line in lines:
				draw.text((x, y), line, font=font, fill=255)
				y += step

except:
	print("except")
GPIO.cleanup()
