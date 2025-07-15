# Assignment 1 - Getting Things in Order

## Task 1 – "Ordnung must sein"

### A. Sorting Algorithms Implementation and Analysis

Implement the following sorting algorithms:

- Bubble Sort
- Insertion Sort
- Merge Sort
- Quick Sort

Test and compare their running times on inputs of various sizes:

- Small inputs (approx. 5–50 elements)
- Large inputs (larger arrays)

**Requirements:**

- Carefully choose or average appropriate inputs to justify your conclusions.
- Include a graph plotting running time as a function of array size for all algorithms.

---

### B. Matching Credit Card Records

The Olsen Gang has obtained a set of 20,000 credit card details split into two parts during transfer over the dark web. The second part is randomly shuffled.

- The first dataset is sorted by increasing expiration dates and PIN.
- The goal: match the records from the second part with the first.

**Task:**

- Choose and implement a **linear time algorithm** to match the records efficiently.
- Empirically investigate at what data size the linear solution outperforms a log-linear algorithm.

---

## Task 2 – "Dangerous Minds"

Implement your own version of a C++ vector (dynamic array) specifically for integers.

**Features to implement:**

- Read and write access to elements
- Append elements
- Resize the array (including truncation if downsizing)
- Delete one element or a segment of elements
- Implement table doubling for dynamic reallocation

**Testing:**

- Measure accumulated running times for large sequences of append operations.
- Compare your implementation's performance with the standard C++ vector and your own linked list implementation.
- Analyze if you can observe when reallocations occur in the vector implementations.

**Notes:**

- Element access time should be **O(1)**
- Amortized append time should be **O(1)**

---

