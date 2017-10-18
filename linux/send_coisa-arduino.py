#!/usr/bin/python2.7

import sys
import base64
import json
import requests
import serial
import math
import time

num_args = len(sys.argv)

if num_args < 2:
	print 'Usage ', sys.argv[0], 'Your_Binary_File Desired_IP [Input_File Output_File]'
	quit()

fixed_io = num_args > 3

def send_code(partial_file_encoded, final_package):
	parameters = {'sendByteCode': partial_file_encoded, 'finalPackage': final_package}

	url = ('http://' + sys.argv[2] + ':9999/commands/execute')
	payload = {'name': '_coisa.setConfig', 'component': 'base', 'parameters': parameters}
	headers = {'Content-Type': 'application/json', 'Content-Length': '333'}

	r = requests.post(url, data=json.dumps(payload), headers=headers)
	print r.text
	print(r.status_code, r.reason)
	print(r.text[:300] + '...')

server = None

QTD_CHAR = 3.0 * (300 if fixed_io else 100)

with open(sys.argv[1], "rb") as executable_file:
	binary_file = executable_file.read()
	total_packages = math.ceil(len(binary_file) / QTD_CHAR)
	for i in range(0, int(total_packages)):
		send_code(base64.b64encode(binary_file[i*int(QTD_CHAR):(i+1)*int(QTD_CHAR)]),'true' if i == total_packages - 1 else 'false')

if fixed_io:
	ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=1.5)  # open serial port
	# ser = serial.Serial(port='/dev/tty.usbserial-A900LQ9M', baudrate=115200, timeout=1.5)  # open serial port

	with open(sys.argv[3], "r") as input_file:
		values = input_file.readline() # read the first line

		for val in values:
			time.sleep(0.005) # delays for X seconds
			ser.write(unicode(val).encode())
			# ser.write(unicode(val))

	with open(sys.argv[4], "w") as output_file:
		values = ''

		while True:
			value = ser.readline()

			if not value:
				break

			values += value

		output_file.write(values)

	ser.close()