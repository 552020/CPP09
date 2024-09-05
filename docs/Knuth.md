# Knuth

1. The maximum number of comparisons for binary insertion is readily seen to be 

   \[
   B(n) = \sum_{1 \leq k \leq n} \left\lfloor \log_2 k \right\rfloor = n \left\lfloor \log_2 n \right\rfloor - 2 \log_2 n + 1
   \tag{3}
   \]

   \[
   \lim_{n \to \infty} \frac{S(n)}{n \log_2 n} = 1.
   \tag{4'}
   \]

   Thus we have an approximate formula for \( S(n) \), but it is desirable to obtain more precise information. The following table gives exact values of the above quantities, for small \( n \):

   \[
   \begin{array}{|c|c|}
   \hline
   n & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15 & 16 & 17 \\
   \hline
   \left\lfloor \log_2 n \right\rfloor & 0 & 1 & 1 & 2 & 2 & 2 & 2 & 3 & 3 & 3 & 3 & 3 & 3 & 3 & 4 & 4 & 4 \\
   \hline
   B(n) & 0 & 1 & 3 & 5 & 7 & 9 & 11 & 14 & 17 & 21 & 25 & 29 & 33 & 37 & 41 & 45 & 49 & 54 \\
   \hline
   \end{array}
   \]

2. **Merge insertion.** A pleasant generalization of the above method has been discovered by Lester Ford, Jr., and Selmer Johnson. Since it involves some aspects of merging and some aspects of insertion, we shall call it **merge insertion**. 

   For example, consider the problem of sorting 21 elements. We start by comparing the ten pairs \( K_1 : K_2; K_3 : K_4; \dots; K_{19} : K_{20} \); then we sort the ten larger elements of the pairs, using merge insertion. As a result we obtain the configuration

   \[
   \begin{array}{cccccccccc}
   a_1 & a_2 & a_3 & a_4 & a_5 & a_6 & a_7 & a_8 & a_9 & a_{10} \\
   b_1 & b_2 & b_3 & b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{11} 
   \end{array}
   \tag{7}
   \]

   analogous to (3). The next step is to insert \( b_3 \) among \( b_1, b_2, a_1 \), then \( b_2 \) among the other elements less than \( a_2 \); we arrive at the configuration

   \[
   \begin{array}{cccccccccc}
   c_1 & c_2 & c_3 & c_4 & c_5 & c_6 & c_7 & c_8 & c_9 & c_{10} \\
   b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{10} & b_{11} 
   \end{array}
   \tag{8}
   \]

   Let us call the upper-line elements the **main chain**. We can insert \( b_5 \) into its proper place in the main chain, using three comparisons (first comparing it to \( c_4 \), then to \( c_9 \) etc.); then \( b_6 \) can be moved into the main chain in three more steps, leading to the configuration

   \[
   \begin{array}{cccccccccc}
   c_1 & c_2 & c_3 & c_4 & c_5 & c_6 & c_7 & c_8 & c_9 & c_{10} \\
   b_1 & b_2 & b_3 & b_4 & b_5 & b_6 & b_7 & b_8 & b_9 & b_{11} 
   \end{array}
   \tag{9}
   \]

   The next step is crucial; is it clear what to do? We insert \( b_1 \) (not bold) into the main chain, using only four comparisons. Then each of \( b_0, b_0, b_3, b_6 \) (in this order) can also be inserted into their proper places in the main chain, using at most four comparisons each.