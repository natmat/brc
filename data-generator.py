import random
import string
import time

NUM_STATIONS = 10_000
NUM_RECORDS = 1_000_000_000
OUTPUT_FILE = "measurements.txt"

MIN_TEMP = -99.9
MAX_TEMP = 99.9

def random_station_name(i: int) -> str:
    """Generate a pseudo-random but deterministic station name."""
    prefix = ''.join(random.choices(string.ascii_uppercase, k=3))
    return f"{prefix}-{i:04d}"  # e.g., ABC-0001

def random_temperature() -> float:
    """Generate a temperature value with one decimal place."""
    return round(random.uniform(MIN_TEMP, MAX_TEMP), 1)

def generate_dataset():
    start = time.time()

    stations = [random_station_name(i) for i in range(NUM_STATIONS)]
    
    with open(OUTPUT_FILE, "w", buffering=1024 * 1024) as f:  # 1 MB buffer
        for i in range(NUM_RECORDS):
            station = random.choice(stations)
            temp = random_temperature()
            f.write(f"{station};{temp}\n")

            if (i + 1) % 10_000_000 == 0:
                elapsed = time.time() - start
                print(f"Generated {i+1:,} records in {elapsed:.1f}s")

    print(f"\n✅ Done: {OUTPUT_FILE} ({NUM_RECORDS:,} records, {NUM_STATIONS:,} stations)")
    print(f"Total time: {time.time() - start:.1f}s")

if __name__ == "__main__":
    generate_dataset()
