import QBinomialQuotientPositivityGapFour

/-!
This file is an informational trusted-base audit. Running it separates the
kernel-checked, problem-specific reduction from the final publication theorem.

The reduction declarations must depend only on Lean/mathlib's standard logical
infrastructure (`propext`, `Classical.choice`, and `Quot.sound`). The external
weighted-complete-intersection bridge and the final theorems that invoke it are
expected to contain exactly the deliberately documented named trust boundary
`weightedCompleteIntersectionBridge_external`.
-/

open QBinomialQuotientPositivityGapFour

#print axioms cyclotomicCondition_of_polynomialQuotient
#print axioms isQIntegerPolynomialQuotient_iff
#print axioms hallCondition_of_cyclotomic
#print axioms hallCondition_of_polynomialQuotient
#print axioms hallCondition_of_qIntegerPolynomialQuotient
#print axioms coefficientwiseToNat_map_of_nonnegative

#print axioms weightedCompleteIntersectionBridge_external
#print axioms quotient_positivity_of_weightedCompleteIntersection
#print axioms qInteger_quotient_coeff_nonnegative_of_weightedCompleteIntersection
#print axioms q_binomial_quotient_positivity_gap_four_of_weightedCompleteIntersection
#print axioms q_binomial_qInteger_quotient_positivity_gap_four_of_weightedCompleteIntersection
#print axioms quotient_positivity
#print axioms qInteger_quotient_positivity
#print axioms qInteger_quotient_coeff_nonnegative
#print axioms q_binomial_quotient_positivity_gap_four
#print axioms q_binomial_qInteger_quotient_positivity_gap_four
