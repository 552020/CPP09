### 1. The maximum number of comparisons for binary insertion is readily seen to be 

$$
B(n) = \sum_{1 \leq k \leq n} \left\lfloor \log_2 k \right\rfloor = n \lfloor \log_2 n \rfloor - 2 \log_2 n + 1
$$

$$
\lim_{n \to \infty} \frac{S(n)}{n \log_2 n} = 1.
$$

Thus we have an approximate formula for \( S(n) \), but it is desirable to obtain more precise information. The following table gives exact values of the above quantities, for small \( n \):

| \( n \)          | 1 | 2 | 3 | 4 | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 |
|------------------|---|---|---|---|----|----|----|----|----|----|----|----|----|----|----|----|----|
| \( \left\lfloor \log_2 n \right\rfloor \) | 0 | 1 | 1 | 2 | 2  | 2  | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  | 4  | 4  | 4  |
| \( B(n) \)       | 0 | 1 | 3 | 5 | 7  | 9  | 11 | 14 | 17 | 21 | 25 | 29 | 33 | 37 | 41 | 45 | 49 | 54 |

### 2. **Merge insertion.** 
A pleasant generalization of the above method has been discovered by Lester Ford, Jr., and Selmer Johnson. Since it involves some aspects of merging and some aspects of insertion, we shall call it **merge insertion**. 

For example, consider the problem of sorting 21 elements. We start by comparing the ten pairs \( K_1 : K_2; K_3 : K_4; \dots; K_{19} : K_{20} \); then we sort the ten larger elements of the pairs, using merge insertion. As a result, we obtain the configuration

$$
\begin{array}{cccccccccc}
a_1 & a_2 & a_3 & a_4 & a_5 & a_6 & a_7 & a_8 & a_9 & a_{10} \\
b_1 & b_2 & b_3 & b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{11} 
\end{array}
$$

analogous to (3). The next step is to insert \( b_3 \) among \( b_1, b_2, a_1 \), then \( b_2 \) among the other elements less than \( a_2 \); we arrive at the configuration

$$
\begin{array}{cccccccccc}
c_1 & c_2 & c_3 & c_4 & c_5 & c_6 & c_7 & c_8 & c_9 & c_{10} \\
b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{10} & b_{11}
\end{array}
$$

Let us call the upper-line elements the **main chain**. We can insert \( b_5 \) into its proper place in the main chain, using three comparisons (first comparing it to \( c_4 \), then to \( c_9 \) etc.); then \( b_6 \) can be moved into the main chain in three more steps, leading to the configuration

$$
\begin{array}{cccccccccc}
c_1 & c_2 & c_3 & c_4 & c_5 & c_6 & c_7 & c_8 & c_9 & c_{10} \\
b_1 & b_2 & b_3 & b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{11}
\end{array}
$$

The next step is crucial; is it clear what to do? We insert \( b_1 \) (not bold) into the main chain, using only four comparisons. Then each of \( b_0, b_0, b_3, b_6 \) (in this order) can also be inserted into their proper places in the main chain, using at most four comparisons each.

A careful count of the comparisons involved here shows that the 21 elements have been sorted in at most 
$$
10 + 22 + 2 + 2 + 3 + 3 + 4 + 4 + 4 + 4 + 4 + 4 = 66 
$$
steps. Since
$$
2^{6} < 21 < 2^{6},
$$
we also know that no fewer than 66 would be possible in any event; hence
$$
S(21) = 66.
\tag{10}
$$

(Binary insertion would have required 74 comparisons.)

In general, merge insertion proceeds as follows for \(n\) elements:

i) **Make pairwise comparisons** of \(\lfloor n/2 \rfloor\) disjoint pairs of elements. (If \(n\) is odd, leave one element out.)  
ii) **Sort** the \(\lfloor n/2 \rfloor\) larger numbers, found in step (i), by merge insertion.  
iii) **Name the elements** \(a_1, a_2, \dots, a_{\lfloor n/2 \rfloor}, b_1, b_2, \dots, b_{\lfloor n/2 \rfloor}\) as in (7), where \(a_1 \leq a_2 \leq \dots \leq a_{\lfloor n/2 \rfloor}\) and \(b_i \leq a_i\) for \(1 \leq i \leq \lfloor n/2 \rfloor\); call the \(a\)'s the "main chain." Insert the remaining \(b\)'s into the main chain, using binary insertion, in the following order, leaving out all \(b_j\) for \(j > \lfloor n/2 \rfloor\):
$$
b_3, b_2, b_5; b_4, b_1, b_{10}, \dots, b_6; \dots, b_{8}, b_{7}, \dots, b_{\lfloor n/2 \rfloor + 1}.
\tag{11}
$$

We wish to define the sequence \((t_4, t_5, t_6, t_7, \dots) = (1, 3, 5, 11, \dots)\), which appears in (11), in such a way that each of \(b_1, b_2, \dots, b_{t_k+1}\) can be inserted into the main chain with at most \(k\) comparisons. Generalizing (7), (8), and (9), we obtain the diagram:

$$
x_1, x_2, \dots, a_{\lfloor n/2 \rfloor} \quad b_4, b_5, b_6, \dots, b_{t_k+1}
\quad a_{t_k+1}, a_{t_k+2}, \dots, a_n.
$$

where the main chain up to and including \(a_{t_k+1}\) contains \(2k-1 + (t_k - t_{k-1} - 1)\) elements. This number must be less than \(2^k\); our best bet is to set it equal to \(2^k - 1\), so that
$$
t_k - t_{k-1} = 2k.
\tag{12}
$$

Since \(t_4 = 1\), we may set \(t_6 = 1\) for convenience, and we find that
$$
t_k = 2^k - 2^{k-1} + 2^{k-2} - 2^{k-3} + \dots = 2^{k} - 2^{k-1} + \dots + (-1)^{k} 2^{0}
$$
or
$$
(2^{k+1} - (1 - (-1)^k)) / 3.
\tag{13}
$$

By summing a geometric series. (Curiously, this same sequence arose in our study of an algorithm for calculating the greatest common divisor of two integers; cf. exercise 4.5.2-27.)

### Page 3:

Let \( F(n) \) be the number of comparisons required to sort \( n \) elements by merge insertion. Clearly:

$$
F(n) = \lfloor n/2 \rfloor + F(\lfloor n/2 \rfloor) + G(n/2),
\tag{14}
$$

where \( G \) represents the amount of work involved in step (iii). If \( t_{k-1} \leq m \leq t_k \), we have

$$
G(m) = \sum_{t_{k-1} \leq i \leq t_k} (i - t_{k-1}) + k(m - t_{k-1}),
\tag{15}
$$

summing by parts. Let us set

$$
w_2 = t_0 + t_1 + \dots + t_{k-1} = 2^{k+1}/3,
\tag{16}
$$

so that \( (w_0, w_1, w_2, w_3, \dots) = (0, 1, 2, 5, 10, 21, \dots) \). Exercise 13 shows that

$$
F(n) - F(n - 1) = k \quad \text{iff} \quad w_k < n \leq w_{k+1},
\tag{17}
$$

and the latter condition is equivalent to

$$
\frac{2^{k+1}}{3} < n \leq \frac{2^{k+2}}{3},
$$

which simplifies to:

$$
k + 1 \leq \log_2 (3n) \leq k + 2;
$$

hence

$$
F(n) - F(n - 1) = \lfloor \log_2 (3n) \rfloor.
\tag{18}
$$

*This formula is due to A. Hadian [Ph.D. thesis, Univ. of Minnesota (1969), 38-42]. It follows that \( F(n) \) has a remarkably simple expression:*

$$
F(n) = \sum_{1 \leq k \leq n} \lfloor \log_2 (3k) \rfloor,
\tag{19}
$$

quite similar to the corresponding formula (3) for binary insertion. A "closed form" for this sum appears in exercise 14.

Equation (19) makes it easy to construct a table of \( F(n) \); we have:

| \( n \)          | 1 | 2 | 3 | 4 | 5 | 6 | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 |
|------------------|---|---|---|---|---|---|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
| \( \lfloor \log_2 n \rfloor \) | 0 | 1 | 1 | 2 | 2 | 2 | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  | 4  | 4  | 4  | 4  | 4  | 4  | 4  |
| \( F(n) \)       | 0 | 1 | 3 | 5 | 7 | 10 | 13 | 16 | 19 | 22 | 26 | 30 | 34 | 38 | 42 | 46 | 51 | 56 | 61 | 66 | 71 |

---

### Page 4:

Note that \( F(n) = \lfloor \log_2 n \rfloor \) for \( 1 \leq n \leq 11 \) and for \( 20 \leq n \leq 21 \), so we know that merge insertion is optimum for these \( n \):

$$
S(n) = \lfloor \log_2 n! \rfloor = F(n) \quad \text{for} \quad 1 \leq n \leq 11 \quad \text{and} \quad 20 \leq n \leq 21.
\tag{20}
$$

Hugo Steinhaus posed the problem of finding \( S(n) \) in the second edition of his classic book *Mathematical Snapshots* (Oxford University Press, 1950), pp. 38-39. He described the method of binary insertion, which is the best possible way to sort \( n \) objects if we start by sorting \( n - 1 \) of them first before the \( n \)-th is considered; and he conjectured that binary insertion is optimum in general. Several years later [*Calcutta Math. Soc. Golden Jubilee Commemoration 2* (1959), 323-327], he reported that two of his colleagues, S. Trybuta and C. Ping, had "recently" disproved his conjecture, and that they had determined \( S(n) \) for \( n \leq 11 \). Trybuta and Ping may have independently discovered the method of merge insertion, which was published soon afterward by Ford and Johnson [*JAMF 66* (1959), 387-389].

After the discovery of merge insertion, the first unknown value of \( S(n) \) was \( S(12) \). Table 1 shows that 12! is quite close to \( 2^{29} \), so that the existence of a 29-step sorting procedure for 12 elements is somewhat unlikely. An exhaustive search (about 60 hours on a Maniac II computer) was therefore carried out by Mark Wells, who discovered that \( S(12) = 30 \) [*Proc. IFIP Congress 65 2* (1965), 497-498]. Thus the merge insertion procedure turns out to be optimum for \( n = 12 \) as well.

---

#### Table 1: Values of Factorials in Binary Notation

| Binary Representation         | Factorial |
|-------------------------------|-----------|
| 1                              | 1!        |
| 10                             | 2!        |
| 110                            | 3!        |
| 11000                          | 4!        |
| 111100                         | 5!        |
| 11111000                       | 6!        |
| 1111111000                     | 7!        |
| 111111110000                   | 8!        |
| 11111111110000                 | 9!        |
| 1111111111110000               | 10!       |
| 111111111111110000             | 11!       |
| 11111111111111100000           | 12!       |
| 1111111111111111100000         | 13!       |
| 111111111111111111100000       | 14!       |
| 11111111111111111111100000     | 15!       |
| 100111101011011001000000000000 | 16!       |
