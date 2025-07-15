import sys
import csv
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) < 2:
        print("Usage: python plot_csv.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]

    try:
        with open(filename, newline="") as csvfile:
            reader = csv.reader(csvfile)
            headers = next(reader)
            values = next(reader)

            labels = headers
            data = [float(v) for v in values]

            plt.figure(figsize=(10, 6))
            plt.bar(labels, data, color="skyblue")
            plt.ylabel("Time / Performance")
            plt.title("Data Structure Performance Comparison")
            plt.grid(axis="y", linestyle="--", alpha=0.7)
            plt.tight_layout()
            plt.savefig("".join(filename.split(".")[:-1]) + ".png")

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
