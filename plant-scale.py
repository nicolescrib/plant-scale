import os
import csv
import time
import sys
from datetime import datetime
import RPi.GPIO as GPIO
from hx711py.hx711 import HX711
import paho.mqtt

# Scale
def calibrate():
	hx.reset()
	hx.tare()
	test_weight = float(input("Enter known weight:"))
	input("Set weight on scale and press any key.")
	loop = 20
	loop_sum = 0
	for i in range(loop):
		w = hx.get_weight(5)
		loop_sum += w
	mean = loop_sum/loop
	return mean / test_weight

# MQTT

#paho.mqtt.connect("192.168.0.102", 1883, 60)
mqtt_topic="scale/weight"
mqtt_hostname="192.168.0.102"
mqttc.loop_forever()

# CSV Writer
data_dir = "data"
os.makedirs(data_dir, exist_ok=True)
filename = datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".csv"
filepath = os.path.join(data_dir, filename)

# HX711
hx = HX711(21, 20)
hx.set_reading_format("MSB", "MSB")
referenceUnit = calibrate()
hx.set_reference_unit(referenceUnit)


with open(filepath, mode="w", newline = "") as file:
	writer = csv.writer(file)
	writer.writerow(["timestamp", "grams"])

	while True:
		try:
			now = datetime.now().isoformat()
			val = hx.get_weight(5)
			print(val)
			writer.writerow([now, val])
			paho.mqtt.publish(mqtt_topic, val, mqtt_hostname) 
			file.flush()

			hx.power_down()
			hx.power_up()
			time.sleep(0.1)

		except (KeyboardInterrupt, SystemExit):
			sys.exit()
			
