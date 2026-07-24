import Lake

open Lake DSL

package «q-binomial-quotient-positivity-gap-four»

require mathlib from git
  "https://github.com/leanprover-community/mathlib4.git" @ "v4.30.0"

@[default_target]
lean_lib QBinomialQuotientPositivityGapFour where
  moreLeanArgs := #["-DwarningAsError=true"]

@[default_target]
lean_lib AxiomAudit where
  moreLeanArgs := #["-DwarningAsError=true"]
