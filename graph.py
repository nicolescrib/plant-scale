import csv
import matplotlib.pyplot as plt
from datetime import datetime

# Read back the CSV we wrote
filepath = "./data/2025-09-25_15-08-02.csv"  # replace with your filename

timestamps = []
grams = []

with open(filepath, newline="") as file:
    reader = csv.DictReader(file)
    for row in reader:
        # parse ISO timestamp string into datetime object
        timestamps.append(datetime.fromisoformat(row["timestamp"]))
        grams.append(float(row["grams"]))

# Plot as a line graph
plt.plot(timestamps, grams, marker="o", linestyle="-")
plt.xlabel("Time")
plt.ylabel("Measurement")
plt.title("Measurements over Time")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("plot.png")

