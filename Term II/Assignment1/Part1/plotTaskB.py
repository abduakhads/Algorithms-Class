import pandas as pd
import matplotlib.pyplot as plt
import sys
import os


def plot(filename):
    df = pd.read_csv(filename)

    row = df.iloc[0]

    labels = row.index[1:]
    values = row.values[1:]

    plt.figure(figsize=(8, 6))
    plt.bar(labels, values, color=["skyblue", "salmon"])

    plt.ylabel("Running Time")
    plt.title(f'Bar Chart for Size {int(row["Size"])}')
    plt.grid(axis="y", linestyle="--", alpha=0.7)

    plt.tight_layout()
    outname = os.path.splitext(filename)[0] + "_plot.png"
    plt.savefig(outname, dpi=300)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        plot(sys.argv[1])
    else:
        print("Provide file name!")
        sys.exit(1)
