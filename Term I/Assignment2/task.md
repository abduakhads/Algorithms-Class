# Assignment 2 - Tree Museum

## Task 1 – "An Introduction to Applied Dendrology"  
*(Basic task for mark 3)*

Implement a binary tree-based container for integers. Generate a large set of random keys and empirically compare the cumulative running times for inserting the keys into:

a) A binary tree, in the original random order.  
b) A binary tree, in the best-case order for building a perfectly balanced tree — e.g., inserting keys level by level (root, its children, grandchildren, …).  
> *Note:* You need to rearrange the keys beforehand to ensure no node is inserted before its parent. Assume you have \( n = 2^m - 1 \) keys.  
c) A library solution, e.g., `std::set`.

---

## Task 2 – "Pruning the Bushes"  
*(Additional task for mark 4)*

Extend Task 1 by comparing the **key removal times** for the two data structures, using keys in the original random order.

---

## Task 3 – "Grow Your Own Arboretum"  
*(Additional task for mark 5)*

Extend Tasks 1 and 2 by including:

d) A **ternary tree** (two keys, three children) inserted in the original random order.  
> *Note:* This is NOT a trie/prefix tree, but a ternary search tree with two keys per node.  

e) A ternary tree inserted in the best-case order (similar to Task 1b).  

f) Your own implementation of a **self-balancing tree**, e.g., an AVL tree.

---
