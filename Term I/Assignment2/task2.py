import random
import time
from sortedcontainers import SortedSet
import matplotlib.pyplot as plt
import pandas as pd
from task1 import BinarySearchTree, Node, reorder


class BSTMod(BinarySearchTree):
    def __init__(self):
        super().__init__()

    def _remove(self, node: Node, key):
        if node is None:
            return node
        if key < node.val:
            node.left = self._remove(node.left, key)
        elif key > node.val:
            node.right = self._remove(node.right, key)
        else:
            if node.left is None:
                return node.right
            elif node.right is None:
                return node.left

            temp = self.min_node(node.right)
            node.val = temp.val
            node.right = self._remove(node.right, temp.val)
        return node

    def min_node(self, node):
        current = node
        while current.left is not None:
            current = current.left
        return current

    def remove(self, key):
        self.root = self._remove(self.root, key)


def main():
    hght = 10
    n = 2**hght - 1
    random_keys = random.sample(range(n), n)
    balanced_keys = reorder(sorted(random_keys))

    rand_bts = BSTMod()
    comp_bts = BSTMod()
    std_lset = SortedSet()

    for key in random_keys:
        rand_bts.insert(key)
        std_lset.add(key)

    for key in balanced_keys:
        comp_bts.insert(key)

    results = {"rand_bts_rem": [], "comp_bts_rem": [], "std_lset_rem": []}

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        rand_bts.remove(key)
        overall_time += time.perf_counter() - start_time
        results["rand_bts_rem"].append(round(overall_time, 8))

    overall_time = 0
    for key in balanced_keys:
        start_time = time.perf_counter()
        comp_bts.remove(key)
        overall_time += time.perf_counter() - start_time
        results["comp_bts_rem"].append(round(overall_time, 8))

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        std_lset.remove(key)
        overall_time += time.perf_counter() - start_time
        results["std_lset_rem"].append(round(overall_time, 8))

    df = pd.DataFrame(results, index=sorted(random_keys))
    df.to_csv("results/BSTMODresults.csv", index_label="nums")

    plt.plot(range(n), results["rand_bts_rem"], label="Random Order Removal")
    plt.plot(range(n), results["comp_bts_rem"], label="Balanced Order Removal")
    plt.plot(range(n), results["std_lset_rem"], label="SortedSet Order Removal")
    plt.xlabel("Number of Keys Removed")
    plt.ylabel("Cumulative Time (seconds)")
    plt.title("Binary Tree Removal Times")
    plt.legend()
    plt.grid()

    plt.tight_layout()
    plt.savefig(f"results/BSTMODresult.png")
    print(f"New results/BSTMODresults.png file created")
    print(f"New results/BSTMODresults.svg file created")


if __name__ == "__main__":
    main()
