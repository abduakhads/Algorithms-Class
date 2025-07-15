import random
import time
from sortedcontainers import SortedSet
import matplotlib.pyplot as plt
import pandas as pd


class Node:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None


class BinarySearchTree:
    def __init__(self):
        self.root = None

    def _insert(self, node: Node, val):
        if node is None:
            return Node(val)
        elif val < node.val:
            node.left = self._insert(node.left, val)
        else:
            node.right = self._insert(node.right, val)
        return node

    def insert(self, val):
        self.root = self._insert(self.root, val)

    def insertVals(self, vals):
        for val in vals:
            self.insert(val)


def reorder(vals):
    if not vals:
        return []
    mid = len(vals) // 2
    return [vals[mid]] + reorder(vals[:mid]) + reorder(vals[mid + 1 :])


def main():
    hght = 10
    n = 2**hght - 1
    random_keys = random.sample(range(n), n)
    balanced_keys = reorder(sorted(random_keys))

    rand_bts = BinarySearchTree()
    comp_bts = BinarySearchTree()
    std_lset = SortedSet()

    results = {"rand_bts_ins": [], "comp_bts_ins": [], "std_lset_ins": []}

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        rand_bts.insert(key)
        overall_time += time.perf_counter() - start_time
        results["rand_bts_ins"].append(round(overall_time, 8))

    overall_time = 0
    for key in balanced_keys:
        start_time = time.perf_counter()
        comp_bts.insert(key)
        overall_time += time.perf_counter() - start_time
        results["comp_bts_ins"].append(round(overall_time, 8))

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        std_lset.add(key)
        overall_time += time.perf_counter() - start_time
        results["std_lset_ins"].append(round(overall_time, 8))

    df = pd.DataFrame(results, index=sorted(random_keys))
    df.to_csv("results/BSTresults.csv", index_label="nums")

    plt.plot(range(n), results["rand_bts_ins"], label="Random Order Insertion")
    plt.plot(range(n), results["comp_bts_ins"], label="Balanced Order Insertion")
    plt.plot(range(n), results["std_lset_ins"], label="SortedSet Order Insertion")
    plt.xlabel("Number of Keys Inserted")
    plt.ylabel("Cumulative Time (seconds)")
    plt.title("Binary Tree Insertion Times")
    plt.legend()
    plt.grid()

    plt.tight_layout()
    plt.savefig(f"results/BSTresult.png")
    print(f"New results/BSTresults.png file created")
    print(f"New results/BSTresults.svg file created")


if __name__ == "__main__":
    main()
