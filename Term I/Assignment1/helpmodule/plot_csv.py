import pandas as pd
import matplotlib.pyplot as plt


def makeSum(results, sizes, fname):
    df = pd.DataFrame(results, index=sizes)
    df.to_csv(f"results/{'_'.join(fname.lower().split(' '))}.csv", index_label="size")
    print(f"New results/{'_'.join(fname.lower().split(' '))}.csv file created")
    plt.figure()
    for name in results.keys():
        plt.plot(sizes, df[name], label=name)
        plt.scatter(sizes, df[name])
    plt.xlabel("Input Size")
    plt.ylabel("Time (seconds)")
    plt.title(f"{fname.title()}")
    plt.legend()
    plt.savefig(f"results/{'_'.join(fname.lower().split(' '))}.png")
    print(f"New results/{'_'.join(fname.lower().split(' '))}.png file created")
