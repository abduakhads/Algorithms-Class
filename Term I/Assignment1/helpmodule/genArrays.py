import random

small_sizes = [5, 10, 25, 50]
large_sizes = [100, 500, 1000, 5000, 10000]
# large_sizes = [100, 500, 1000, 5000, 10000, 50000, 100000]


def genRandArray(size: int) -> list:
    return random.sample(list(range(size)), size)


if __name__ == "__main__":
    with open("arrays.py", "w") as f:
        small = {}
        large = {}
        for i in small_sizes:
            small[i] = genRandArray(i)
        f.write(f"small = {small}\n")
        for i in large_sizes:
            large[i] = genRandArray(i)
        f.write(f"large = {large}")
