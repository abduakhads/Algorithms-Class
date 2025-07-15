def bubbleSort(arr):
    for i in range(len(arr)):
        # no_swap = True
        for j in range(len(arr) - 1 - i):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                # no_swap = False
        # if no_swap:
        #     break
    return arr


def insertionSort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1

        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
    return arr


def mergeSort(arr):
    n = len(arr)

    if n == 1:
        return arr

    mid = n // 2
    left = arr[:mid]
    right = arr[mid:]

    left = mergeSort(left)
    right = mergeSort(right)

    return merge(left, right)


def merge(left, right):
    arr = []
    i = j = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            arr.append(left[i])
            i += 1
        else:
            arr.append(right[j])
            j += 1

    while i < len(left):
        arr.append(left[i])
        i += 1

    while j < len(right):
        arr.append(right[j])
        j += 1

    return arr


def quickSort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quickSort(left) + middle + quickSort(right)


algos = [quickSort, mergeSort, insertionSort, bubbleSort]


def countSortMod(arr, a, b, key):
    count = {i: 0 for i in range(a, b + 1)}
    for i in arr:
        count[key(i)] += 1

    for k in range(1, len(count)):
        count[k] += count[k - 1]

    new_arr = [0] * len(arr)

    for k in reversed(arr):
        new_arr[count[key(k)] - 1] = k
        count[key(k)] -= 1

    return new_arr


def countSortMod2(arr, a, b, key):
    count = [0] * (b - a + 1)

    for i in range(len(arr)):
        count[key(arr[i]) - a] += 1

    for k in range(1, len(count)):
        count[k] += count[k - 1]

    new_arr = [0] * len(arr)

    for i in reversed(range(len(arr))):
        new_arr[count[key(arr[i]) - a] - 1] = arr[i]
        count[key(arr[i]) - a] -= 1

    return new_arr


def countSort(arr, a=0, b=10000):
    count = {i: 0 for i in range(a, b + 1)}
    for i in arr:
        count[i] += 1

    for k in range(1, len(count)):
        count[k] += count[k - 1]

    new_arr = [0] * len(arr)

    for k in reversed(arr):
        new_arr[count[k] - 1] = k
        count[k] -= 1

    return new_arr


def countSort2(arr, a, b):
    count = [0] * (b - a + 1)

    for i in range(len(arr)):
        count[arr[i] - a] += 1

    for k in range(1, len(count)):
        count[k] += count[k - 1]

    new_arr = [0] * len(arr)

    for i in reversed(range(len(arr))):
        new_arr[count[arr[i] - a] - 1] = arr[i]
        count[arr[i] - a] -= 1

    return new_arr
