# -*- coding: utf-8 -*-
import os
import glob
import time
import subprocess
import RPi.GPIO as GPIO

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

base_dir = '/sys/bus/w1/devices/'


GPIO.setmode(GPIO.BCM)
GPIO.setup(23,GPIO.OUT)
files = [ f for f in os.listdir(base_dir) if f.startswith('28')]

def read_temp_raw(file):
    device_folder = glob.glob(base_dir + file)[0]
    device_file = device_folder + '/w1_slave'
    catdata = subprocess.Popen(['cat', device_file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out,err = catdata.communicate()
    out_decode = out.decode('utf-8')
    lines=out_decode.split('\n')
    return lines
       
def read_temp(file):
    lines = read_temp_raw(file)
    while lines[0].strip()[-3:]!='YES':
        time.sleep(0.2)
        print('lol')
        lines = read_temp_raw(file)
    equals_pos = lines[1].find('t=')
    if (equals_pos !=-1):
        temp_string = lines[1][equals_pos+2:]
        temp_c = round(float(temp_string)/1000.0, 1)
        return temp_c

#while True:
#    print files
#    for filename in files:
#        temp=read_temp(filename)
#        print(temp)
#        if (temp>26):
#            GPIO.output(23,GPIO.HIGH)
#        else:
#            GPIO.output(23,GPIO.LOW)
#        time.sleep(0.1)
