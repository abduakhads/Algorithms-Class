# Assignment – Spell Checkers, Labyrinths, Graphs, and Hash Tables

## Part One

### A. **"Me Spell Rite"**

The Evil Lord Wladimir has cast *Spellus Incorrectus* on your fellow students! Your mission is to implement a simple **spell checker** that determines whether each word in a text file is correctly spelled.

You should try the following data structures for storing the dictionary:

- Naive approach: **Linear list**
- **Balanced Binary Search Tree (BBST)** – you may use `std::set` or another library BBST
- **Trie**
- **Hash map**

**Tasks:**

- Implement all four methods
- Use an English word list (e.g., from the provided directory)
- Compare running times for:
  - **Building the dictionary**
  - **Spell checking** a large text

**Deliverables:**

- Short description of each algorithm
- Graphs showing **running time (y-axis)** vs. **text size (x-axis)**
- Summary of findings

---

### B. **"Triwizard Tournament"**

In the Triwizard Tournament, wizards must escape a labyrinth as quickly as possible. As the supervisor, your job is:

Given:

- A map of the labyrinth
- Starting positions of the **three wizards**
- Their **speeds** (in corridors per minute)

Predict **which wizard reaches the exit first**.

**Constraints:**

- Assume magical wands guide each wizard via the **shortest path**
- **Use Breadth-First Search (BFS)** **once only**, from the **exit** to all reachable positions

**Deliverables:**

- Implementation of the labyrinth traversal
- Output showing who wins and by how much time
- Justification of algorithm and BFS usage

---

## Part Two – **"Aunt's Namesday"**

Your Aunt Petunia is organizing her annual namesday party. Guests have interpersonal issues, and she wants to **seat them at two tables** so that **no two people who dislike each other sit at the same table**.

**Your Task:**

Given:

- A list of guests
- A list of **who doesn’t like whom**

Use a **non-recursive Depth-First Search (DFS)** to check if such a bipartition is possible and to generate a valid **sitting scheme**.

**Deliverables:**

- Non-recursive DFS implementation
- Seating plan output
- Statement if such partitioning is possible
- Explanation of the algorithm and how it avoids recursion

---

## Part Three – **"Full House"**

Implement and compare different **Hash Table** strategies:

### Hash Table Variants:

1. **Separate chaining**
2. **Open addressing with linear probing**
3. **Open addressing with double hashing**

**Tasks:**

- Implement all three
- Measure **search** and **insert** times at various **load factors**

**Deliverables:**

- Graphs comparing **running time (y-axis)** vs. **load factor (x-axis)** for all three
- Explanation of each method
- Discussion on trade-offs and performance trends

---
