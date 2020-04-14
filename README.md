# Algorithm2017
Course Assignments of Foundations of Algorithms (011146.01) at USTC in 2017 Fall.

## project 1: Sorting
- ex1: Sorting `n` elements where each element is a randomly-generated string with size of 1..32. All the strings contains only lower case letters.
  - **Algorithms: Insertion Sort, Heap Sort, Merge Sort, Quick Sort**.
- ex2: Sorting `n` elements where each element is a randomly-generated integer in the range of `[1, 65535]`.
  - **Algorithms: Bubble Sort, Quick Sort, Radix sort, Counting Sort**.

## project 2: Algorithm Design Strategy
- ex1: **Matrix Chain Ordering Problem (MCOP)**
  - For given `n`, randomly generating `n+1` integers (`p0`, `p1`, ..., `pn`) as the scale of the matrices. The size of the `i`-th matrix is `p(i-1)xp(i)`. Use **dynamic programming** to determine the optimal order of the matrix china multiplications.
- ex2: **Fast Fourier Transform (FFT)**
  - For given `n`, randomly generating `2n` real values (`a0`, `a1`, ..., `a(n-1)`) and (`b0`, `b1`, ..., `b(n-1)`) as the coefficient vector of polynomials `A(x)` and `B(x)`. Use FFT to calculate the product of `A(x)` and `B(x)`.

## project 3: Red-Black Tree
- ex1: Implement fundamental algorithms on the Red-Black Tree. Generate `n` random positive integers (`k1`, `k2`, ..., `kn`) as the key word of each node. Insert these `n` nodes to a empty red-black tree.
- ex2: For the above described reb-black tree, find the `n/3`-th and `n/4`-th least nodes and delete them.

The supported operations on the Red-Black Tree:
- Left/Right rotate, node insertion and deletion, node iteration
- Search the `i`-th least key on the red-black tree
- Output all the node on the tree

## project 4: Algorithms in Graph Theory
- ex1: **Strongly Connected Component**
  - Computing all the strongly connected components on a directed graph. 
  - The number of the vertices is `N` and the number of edges is `NlogN`.
- ex2: **Shorted Path**
  - Find the shortest paths between all pairs of vertices in an edge-weighted, undirected graph with Johnson's Algorithm.
  - The number of the vertices is `N` and the number of edges is `NlogN`.
