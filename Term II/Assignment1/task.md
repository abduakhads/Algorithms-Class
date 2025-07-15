# Assignment 1 – Pattern Matching and Image Hashing

## Part One

### A. **"Mom bought me a new computer"**

Implement the following **pattern matching algorithms**:

- Brute-force
- Sunday
- Knuth-Morris-Pratt (KMP)
- Finite State Machine (FSM)
- Rabin-Karp
- Gusfield’s Z-algorithm

**Tasks:**

- Compare their running times using:
  - A **small pattern** (a few words or a sentence)
  - A **large pattern** (a paragraph)
- Use chapters from a book as your text data.
- Test across several **text lengths** (x-axis) and measure **running time** (y-axis).
- Your results should be reproducible.

**Deliverables:**

- A short **description of each algorithm**
- A **graph** showing running time (RT) vs. text length
- Discussion of findings

---

### B. **"Wacky Races"**

Empirically **prove** that there are cases where:

1. **Binary Sunday** is at least **2× faster** than **Gusfield Z**
2. **KMP** is at least **2× faster** than **Rabin-Karp**
3. **Rabin-Karp** is at least **2× faster** than **Sunday**

**Requirements:**

- Use a specific **pattern (P)** and **text (T)** for each case.
- T must be **at least 100kB** in size.
- Explain **why the speed difference occurs** in each comparison.
- Ensure **reproducibility** of all tests.

---

## Part Two – **"What was that char again?"**

Extend **Brute-force** and **Sunday** algorithms to support **wildcards**:

- `?` matches **any single character**
- `*` matches **any sequence** (including empty)
- Escape character: `\` can escape `*`, `?`, or `\` itself

**Tasks:**

- Implement extended versions of Brute-force and Sunday
- Return a **Boolean match result**
- Include an explanation of **how** you modified the algorithms to support wildcards

---

## Part Three – **"Jewish-style carp"**

Develop a version of **Rabin-Karp** for 2D pattern search:

> Check if the **top-right K × K** corner of an `M × N` picture is duplicated **anywhere else** in the picture.

**Constraints:**

- Picture is a 2D array of arbitrary items
- The algorithm **must run in linear time** relative to the number of pixels in the image
- Use **bitwise AND masks (`&`)** instead of slow modulo prime operations

**Deliverables:**

- Efficient implementation of 2D Rabin-Karp with hash masking
- Justification of linear time complexity
- Description of hash function used

---
