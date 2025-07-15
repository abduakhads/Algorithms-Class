import pandas as pd
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 3:
    print("Usage: python plot.py <csv_filename>")
    sys.exit(1)

filename = sys.argv[1]
opr_name = sys.argv[2]

df = pd.read_csv(filename)

plt.figure(figsize=(10, 6))
plt.plot(df["LoadFactor"], df["Chaining"], marker="o", label="Chaining")
plt.plot(df["LoadFactor"], df["LinearProbing"], marker="s", label="Linear Probing")
plt.plot(df["LoadFactor"], df["DoubleHashing"], marker="^", label="Double Hashing")

plt.title(opr_name.title() + " vs " + "Load Factor")
plt.xlabel("Load Factor")
plt.ylabel("Time (micro)")
plt.grid(True)
plt.legend()

# Show plot
plt.tight_layout()
plt.savefig("".join(filename.split(".")[:-1]) + ".png")
