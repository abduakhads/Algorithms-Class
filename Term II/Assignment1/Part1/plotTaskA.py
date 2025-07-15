import pandas as pd
import matplotlib.pyplot as plt
import sys
import os


def plot_from_csv(filename):
    # reading the CSV file
    df = pd.read_csv(filename)

    # extracting text lens
    x = df["TextLength"]

    # getting algo names (- first column)
    algorithms = df.columns[1:]

    # creating the plot
    plt.figure(figsize=(10, 6))
    for algo in algorithms:
        plt.plot(x, df[algo], marker="o", label=algo)

    # title from filename
    title = os.path.splitext(os.path.basename(filename))[0].replace("_", " ").title()

    plt.title(f"Search Algorithm Performance - {title}")
    plt.xlabel("Text Length")
    plt.ylabel("Execution Time")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # saving
    outname = os.path.splitext(filename)[0] + "_plot.png"
    plt.savefig(outname)
    print("\nplot created", outname)
    # plt.show()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        plot_from_csv(sys.argv[1])
    else:
        print("Provide file name!")
        sys.exit(1)
