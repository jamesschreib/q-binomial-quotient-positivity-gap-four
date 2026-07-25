# The $\ell=k-4$ case of $q$-binomial quotient positivity

The $\ell=k-4$ case of the positivity conjecture for a quotient of $q$-binomial coefficients states that, for $4 \le k \le n$, if

$$
\frac{\genfrac{[}{]}{0pt}{}{n}{k}_q}{\genfrac{[}{]}{0pt}{}{n}{k-4}_q}
$$

is a polynomial, then all its coefficients are nonnegative.

Formalized in Lean 4.30 using the named weighted-complete-intersection axiom supplied by Gatzweiler–Levicán-Santibáñez–Yoshida, Propositions 3.3 and 3.6. Reproduce it with `lake build`.
