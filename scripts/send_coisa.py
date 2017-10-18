#!/usr/bin/python2.7

import sys
import base64
import json
import math
import requests
import subprocess
import os

num_args = len(sys.argv)

if num_args < 2:
	print 'Possible uses: 1)', sys.argv[0], 'Your_Binary_File [Desired_IP] (server must be compiled with FIX_IO_CONTENT = 0)'
	print '               2)', sys.argv[0], 'Your_Binary_File Input_File Output_File [Desired_IP] (server must be compiled with FIX_IO_CONTENT = 1)'
	quit()

fixed_io = num_args > 3

def send_code(partial_file_encoded, final_package):
	ip = '127.0.0.1'
	parameters = {'sendByteCode': partial_file_encoded, 'finalPackage': final_package}

	if fixed_io:
		if num_args == 5:
			ip = sys.argv[4]
		parameters.update({'inputDataPath': sys.argv[2], 'outputDataPath': sys.argv[3]})
	else:
		if num_args == 3:
			ip = sys.argv[2]

	url = ('http://' + ip + ':9999/commands/execute')
	payload = {'name': '_coisa.setConfig', 'component': 'base', 'parameters': parameters}
	headers = {'Content-Type': 'application/json', 'Content-Length': '333'}

	r = requests.post(url, data=json.dumps(payload), headers=headers)
	print r.text
	print(r.status_code, r.reason)
	print(r.text[:300] + '...')

server = None

if num_args == 4:
	home = os.path.expanduser("~")
	server = subprocess.Popen(home + '/Arduino/libraries/libMiletusCoisa/linux/bin/linux_example_wifi')

QTD_CHAR = 3.0 * (700 if fixed_io else 500)

with open(sys.argv[1], "rb") as executable_file:
	binary_file = executable_file.read()
	total_packages = math.ceil(len(binary_file) / QTD_CHAR)
	for i in range(0, int(total_packages)):
		send_code(base64.b64encode(binary_file[i*int(QTD_CHAR):(i+1)*int(QTD_CHAR)]),'true' if i == total_packages - 1 else 'false')

if server != None:
	server.communicate()