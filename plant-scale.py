import os
import csv
import time
import sys
from datetime import datetime
import RPi.GPIO as GPIO
from hx711py.hx711 import HX711

def cleanAndExit():
    print("Cleaning...")
        
    print("Bye!")
    sys.exit()

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

data_dir = "data"
os.makedirs(data_dir, exist_ok=True)

filename = datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".csv"
filepath = os.path.join(data_dir, filename)

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
			file.flush()

			hx.power_down()
			hx.power_up()
			time.sleep(0.1)

		except (KeyboardInterrupt, SystemExit):
			cleanAndExit()
