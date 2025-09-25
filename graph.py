from __future__ import annotations

import numpy as np
import csv
from datetime import datetime
from pathlib import Path
from typing import Iterable

import matplotlib.pyplot as plt


DATA_DIR = Path("./data")
OUTPUT_DIR = Path("./data_graphs")


def iter_csv_files(directory: Path) -> Iterable[Path]:
    """Yield CSV files in *directory*, sorted alphabetically."""

    return sorted(p for p in directory.glob("*.csv") if p.is_file())
def read_measurements(filepath: Path) -> tuple[list[datetime], list[float]]:
    """Read timestamp and gram measurements from *filepath*."""

    timestamps: list[datetime] = []
    grams: list[float] = []

    with filepath.open(newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            timestamps.append(datetime.fromisoformat(row["timestamp"]))
            grams.append(float(row["grams"]))

    return timestamps, grams


def create_graph(csv_path: Path, output_path: Path) -> None:
    """Create and save a graph for *csv_path* to *output_path*."""

    timestamps, grams = read_measurements(csv_path)

    if not timestamps or not grams:
        print(f"Skipping {csv_path.name}: no data found.")
        return

    plt.figure()
    plt.plot(timestamps, grams, linestyle="-")
    plt.xlabel("Time")
    plt.ylabel("Measurement (grams)")
    plt.axhline(y=608, color="red", label= "actual weight")
    plt.axhline(y=np.mean(grams), color="green", label= "avg measurement")
    plt.title(f"Measurements over Time\n{csv_path.stem}")
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.legend()
    plt.savefig(output_path)
    plt.close()
    print(f"Saved graph for {csv_path.name} -> {output_path}")


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)

    for csv_path in iter_csv_files(DATA_DIR):
        output_path = OUTPUT_DIR / f"{csv_path.stem}.png"

        if output_path.exists():
            print(f"Graph already exists for {csv_path.name}, skipping.")
            continue

        create_graph(csv_path, output_path)


if __name__ == "__main__":
    main()

