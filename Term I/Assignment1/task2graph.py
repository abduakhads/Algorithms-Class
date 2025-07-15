import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/MyVector_results.csv")
df2 = pd.read_csv("results/StdVector_results.csv")

df["Reallocation"] = df["Reallocation"].str.strip()
df2["Reallocation"] = df2["Reallocation"].str.strip()

plt.plot(df["Elements"], df["Time"], label="MyVec Insertion Time", color="blue")
plt.plot(df2["Elements"], df2["Time"], label="StdVec Insertion Time", color="green")

highlight = df[df["Reallocation"] == "here"]
highlight2 = df2[df2["Reallocation"] == "here"]

if not highlight.empty:
    plt.scatter(
        highlight["Elements"],
        highlight["Time"],
        color="red",
        label="Reallocation of MyVec",
        zorder=5,
    )
if not highlight2.empty:
    plt.scatter(
        highlight2["Elements"],
        highlight2["Time"],
        color="orange",
        label="Reallocation of StdVec",
        zorder=5,
    )

plt.xlabel("Elements")
plt.ylabel("Time (seconds)")
plt.title("Insertion Time vs Elements")

plt.legend()

plt.savefig(f"results/MyVecVsStdVec.png")
print(f"New results/MyVecVsStdVec.png file created")
