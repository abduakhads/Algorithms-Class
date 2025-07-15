import timeit

import task1A, task1B
from helpmodule import genArrays, arrays

from helpmodule.plot_csv import makeSum


def sumTask1A():
    results = {algo.__name__: [] for algo in task1A.algos}
    for i in genArrays.small_sizes:
        for algo in task1A.algos:
            results[algo.__name__].append(
                round(
                    timeit.timeit(
                        lambda: algo(arrays.small[i]), globals=globals(), number=10
                    ),
                    4,
                )
            )
    makeSum(results, genArrays.small_sizes, "Sorting Algos Performance Small Sizes ")

    for i in genArrays.large_sizes:
        for algo in task1A.algos:
            results[algo.__name__].append(
                round(
                    timeit.timeit(
                        lambda: algo(arrays.large[i]), globals=globals(), number=1
                    ),
                    4,
                )
            )

    all_sizes = genArrays.small_sizes + genArrays.large_sizes
    makeSum(results, all_sizes, "Sorting Algos Performance Overall")


def sumTask1B():
    algos = [task1A.mergeSort, task1A.quickSort, task1A.countSort2]
    results = {algo.__name__: [] for algo in algos}

    for i in genArrays.large_sizes:
        for algo in algos:
            if algo.__name__ == "countSort2":
                res = round(
                    timeit.timeit(
                        lambda: algo(arrays.large[i], 0, i),
                        globals=globals(),
                        number=10,
                    ),
                    4,
                )
            else:
                res = round(
                    timeit.timeit(
                        lambda: algo(arrays.large[i]), globals=globals(), number=10
                    ),
                    4,
                )
            results[algo.__name__].append(res)

    makeSum(results, genArrays.large_sizes, "Linear Sort Algo Comparision")

    task1B.main()


if __name__ == "__main__":
    sumTask1A()
    sumTask1B()
