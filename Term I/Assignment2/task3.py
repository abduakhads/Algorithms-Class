import random
import time
from collections import deque
from sortedcontainers import SortedSet

from task1 import BinarySearchTree

from collections import deque


class TernaryNode:
    def __init__(self, key1, key2=None):
        self.key1 = key1
        self.key2 = key2
        self.left = None
        self.middle = None
        self.right = None


class TernaryTree:
    def __init__(self):
        self.root = None

    def insert(self, key):
        self.root = self._insert(self.root, key)

    def _insert(self, node: TernaryNode, key):
        if node is None:
            return TernaryNode(key)
        elif node.key2 is None:
            if node.key1 > key:
                node.key1, node.key2 = key, node.key1
            else:
                node.key2 = key
            return node

        if key < node.key1:
            node.left = self._insert(node.left, key)
        elif key > node.key2:
            node.right = self._insert(node.right, key)
        else:
            node.middle = self._insert(node.middle, key)

        return node

    def bfs(self):
        if not self.root:
            return []

        queue = deque([(self.root, 0)])
        result = []

        while queue:
            node, level = queue.popleft()

            result.append((node.key1, level))

            if node.key2 is not None:
                result.append((node.key2, level))

            if node.left:
                queue.append((node.left, level + 1))
            if node.middle:
                queue.append((node.middle, level + 1))
            if node.right:
                queue.append((node.right, level + 1))

        return result


class AvlNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.height = 1


class AVLTree:
    def __init__(self):
        self.root = None

    def height(self, node):
        if node is None:
            return 0
        return node.height

    def upd_height(self, node):
        node.height = max(self.height(node.left), self.height(node.right)) + 1

    def balance_factor(self, node):
        if node is None:
            return 0
        return self.height(node.left) - self.height(node.right)

    def right_rotate(self, y):
        x = y.left
        T2 = x.right

        x.right = y
        y.left = T2

        self.upd_height(y)
        self.upd_height(x)

        return x

    def left_rotate(self, x: AvlNode):
        y = x.right
        T2 = y.left

        y.left = x
        x.right = T2

        self.upd_height(x)
        self.upd_height(y)

        return y

    def balance(self, node: AvlNode):
        balance = self.balance_factor(node)
        # print(f"Balancing Node {node.key} with BF={balance}")

        if balance > 1:
            if self.balance_factor(node.left) < 0:
                node.left = self.left_rotate(node.left)
            return self.right_rotate(node)

        if balance < -1:
            if self.balance_factor(node.right) > 0:
                node.right = self.right_rotate(node.right)
            return self.left_rotate(node)

        return node

    def _insert(self, root: AvlNode, key):
        if root is None:
            return AvlNode(key)

        if key < root.key:
            root.left = self._insert(root.left, key)
        elif key > root.key:
            root.right = self._insert(root.right, key)
        else:
            return root

        self.upd_height(root)

        return self.balance(root)

    def insert(self, key):
        self.root = self._insert(self.root, key)

    def bfs(self):
        if not self.root:
            return []

        queue = deque([(self.root, 0)])
        result = []

        while queue:
            node, level = queue.popleft()

            result.append((node.key, level))

            if node.left:
                queue.append((node.left, level + 1))
            if node.right:
                queue.append((node.right, level + 1))

        return result


def reorder(vals):
    if not vals:
        return []
    mid = len(vals) // 2
    return [vals[mid]] + reorder(vals[:mid]) + reorder(vals[mid + 1 :])


def reorder3(keys):
    if not keys:
        return []
    elif len(keys) <= 2:
        return keys

    mid1 = len(keys) // 3 - 1
    mid2 = (len(keys) // 3 - 1) * 2 + 1
    return (
        [keys[mid1], keys[mid2]]
        + reorder(keys[:mid1])
        + reorder(keys[mid1 + 1 : mid2])
        + reorder(keys[mid2 + 1 :])
    )


def main():
    hght = 10
    n = 2**hght - 1
    # n = 9

    random_keys = random.sample(range(n), n)
    balanced_keys = reorder(sorted(random_keys))
    balanced3_keys = reorder3(sorted(random_keys))

    rand_tst = TernaryTree()
    comp_tst = TernaryTree()
    avl_bst = AVLTree()
    rand_bts = BinarySearchTree()
    comp_bts = BinarySearchTree()
    std_lset = SortedSet()

    results = {
        "rand_tst": [],
        "comp_tst": [],
        "avl_bst": [],
        "rand_bts_ins": [],
        "comp_bts_ins": [],
        "std_lset_ins": [],
    }

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        rand_tst.insert(key)
        overall_time += time.perf_counter() - start_time
        results["rand_tst"].append((overall_time))

    overall_time = 0
    for key in balanced3_keys:
        start_time = time.perf_counter()
        comp_tst.insert(key)
        overall_time += time.perf_counter() - start_time
        results["comp_tst"].append((overall_time))
    # print(balanced_keys)
    # print(comp_tst.bfs())

    overall_time = 0
    for key in random_keys:
        start_time = time.perf_counter()
        avl_bst.insert(key)
        overall_time += time.perf_counter() - start_time
        results["avl_bst"].append((overall_time))

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

    import matplotlib.pyplot as plt
    import pandas as pd

    df = pd.DataFrame(results, index=sorted(random_keys))
    df.to_csv("results/TSTresultsComp.csv", index_label="nums")
    plt.grid()
    plt.plot(range(n), results["rand_tst"], label="TernaryTree Random Order Insertion")
    # plt.scatter(range(n), results["rand_tst"])
    plt.plot(
        range(n), results["comp_tst"], label="TernaryTree Balanced Order Insertion"
    )
    # plt.scatter(range(n), results["comp_tst"])
    plt.plot(range(n), results["avl_bst"], label="AvlTree Random Order Insertion")
    # plt.scatter(range(n), results["avl_bst"])
    plt.plot(
        range(n), results["rand_bts_ins"], label="BinaryTree Random Order Insertion"
    )
    plt.plot(
        range(n), results["comp_bts_ins"], label="BinaryTree Balamced Order Insertion"
    )
    plt.plot(
        range(n), results["std_lset_ins"], label="SortedSet Random Order Insertion"
    )
    plt.xlabel("Number of Keys Inserted")
    plt.ylabel("Cumulative Time (seconds)")
    plt.title("Trees Insertion Times")
    plt.legend()

    plt.tight_layout()
    plt.savefig(f"results/TSTresultsComp.png")
    print(f"New results/TSTresultsComp.png file created")
    print(f"New results/TSTresultsComp.svg file created")


if __name__ == "__main__":
    main()
