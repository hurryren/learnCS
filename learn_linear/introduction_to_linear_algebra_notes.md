# Chapter 1 : Introduction to Vectors

The heart of linear algebra is in two operations-both with vetors. We add vectors to get  $v+w$. We multiply them by numbers $c$ and $d$ to get $cv$ and $dw$. Combining those two operations(adding $cv$ to $dw$) gives the **linear combination** $cv +dw$.

$$Linear combination cv+dw=c\begin{bmatrix}1\\1\end{bmatrix} + d\begin{bmatrix}2\\3\end{bmatrix}=c\begin{bmatrix}c+2d\\c+3d\end{bmatrix}$$

LInear combinations are all-important in this subject! Sometimes we want one particular combination, the specific choice $c=2$ and $d=1$ that produces $cv+dw=(4,5)$. Other times we want al the combinations of $v$ and $w$ (coming from all $c$ and $d$).

The vectors $cv$ lie along a line. When $w$ is not one that line, **the combinations** $cv+dw$ **fill the whole tow-dimensional plane**. Starting from four vectors $u, v, w, z$ in four-dimensional space, their combinations $cu + dv + ew + fz$ are likely to fill the space but not always. The vectors and their combinations could lie in a plane or on a line.

Chapter 1 explains these central ideas, on which everything builds. We start with two dimensional vectors and three-dimensional vectors, which are reasonable to draw. Then we move into higher dimensions. The really impressive feature of linear algebra is how smoothly it takes that step into $n$-dimensional space. Your mental picture stays completely correct, even if drawing a ten-dimensional vector is impossible.

This is where the book is going (into $n$-dimensional space). The first steps are the operations in Sections 1.1 and 1.2. Then Section 1.3 oulines three fundamental ideas.

**1.1 Vector addition $v + w$ and linear combinations $cv + dw$.**

**1.2 The dot product $v.w$ of two vectors and the length $||v||=\sqrt{v.v}$.**

**1.3 Matrices $A$, linear equations $Ax = b$, solutions $x = A^{-1}b$.**





## 1.1 Vectors and Linear Combinations

>   1.   $3v+5w$ is a typical **linear combination** $cv+dw$ of the vectors $v$ and $w$.
>   2.   For $v=\begin{bmatrix}1\\1 \end{bmatrix}$ and $w=\begin{bmatrix}2\\3 \end{bmatrix}$ that combination is $$3\begin{bmatrix}1\\1 \end{bmatrix}+5\begin{bmatrix}2\\3 \end{bmatrix}=\begin{bmatrix}3+10\\3+15 \end{bmatrix}=\begin{bmatrix}13\\18 \end{bmatrix}$$.
>   3.   The vector $\begin{bmatrix}2\\3 \end{bmatrix}=\begin{bmatrix}2\\0 \end{bmatrix}+\begin{bmatrix}0\\3 \end{bmatrix}$ goes across to $x=2$ and up to $y=3$ in the $xy$ plane.
>   4.   The combinations $c\begin{bmatrix}1\\1 \end{bmatrix} + d\begin{bmatrix}2\\3 \end{bmatrix}$ fill the whole $xy$ plane. The produce every $\begin{bmatrix}x\\y \end{bmatrix}$.
>   5.   The combinations $c\begin{bmatrix}1\\1\\1 \end{bmatrix} + d\begin{bmatrix}2\\3\\4 \end{bmatrix}$ fill a **plane** in $xyz$ space. Same plane for $\begin{bmatrix}1\\1\\1 \end{bmatrix}, \begin{bmatrix}3\\4\\5 \end{bmatrix}$.
>   6.   But $\begin{matrix} c+2d=1 \\ c+3d=0\\c+4d=0 \end{matrix}$ has no solution because its right side $\begin{bmatrix}1\\0\\0 \end{bmatrix}$ is not on that plane.











 



 











































