# GIMPLE Dump Commands for Each Optimization Technique

## Prerequisites
- GCC installed (MinGW/MSYS2 on Windows)
- Source file: `code.c`

---

## 0. Base GIMPLE (No Optimization — Original 3AC)
```bash
gcc -fdump-tree-gimple -c code.c
```
**Output:** `code.c.006t.gimple`  
This is the unoptimized GIMPLE — your starting point for all comparisons.

---

## 1. Constant Propagation
```bash
gcc -O3 -fdump-tree-ccp1 -c code.c
```
**Output:** `code.c.*.ccp1`  
**Pass:** CCP (Conditional Constant Propagation) replaces variables with their known constant values.

---

## 2. Constant Folding
```bash
gcc -O3 -fdump-tree-ccp1 -c code.c
```
**Output:** `code.c.*.ccp1`  
**Note:** Constant folding is performed as part of the CCP pass. After constants are propagated, expressions like `25 + 15` are evaluated at compile time within the same pass. The CCP dump shows both propagation and folding results.

To also see folding done during **Full Redundancy Elimination**:
```bash
gcc -O3 -fdump-tree-fre1 -c code.c
```
**Output:** `code.c.*.fre1`

---

## 3. Algebraic Simplification & Strength Reduction
```bash
gcc -O3 -fdump-tree-forwprop1 -c code.c
```
**Output:** `code.c.*.forwprop1`  
**Pass:** Forward Propagation applies algebraic identities (e.g., `x * 1 → x`, `x + 0 → x`).

For strength reduction (e.g., `x * 4 → x << 2`):
```bash
gcc -O3 -fdump-tree-optimized -c code.c
```
**Output:** `code.c.*.optimized`  
**Note:** Strength reduction is applied across multiple late passes. The `optimized` dump shows the final tree after all simplifications.

---

## 4. Copy Propagation
```bash
gcc -O3 -fdump-tree-copyprop1 -c code.c
```
**Output:** `code.c.*.copyprop1`  
**Pass:** Replaces uses of `y` in `y = x` with `x` directly, eliminating redundant copies like `step2 = step1; final_step = step2`.

---

## 5. Common Subexpression Elimination (CSE)
```bash
gcc -O3 -fdump-tree-fre1 -c code.c
```
**Output:** `code.c.*.fre1`  
**Pass:** FRE (Full Redundancy Elimination) detects expressions computed multiple times with unchanged operands (e.g., `p * q` computed twice) and reuses the first result.

---

## 6. Unreachable Code Elimination
```bash
gcc -O3 -fdump-tree-cfg -c code.c
```
**Output:** `code.c.*.cfg`  
**Pass:** CFG cleanup removes basic blocks that have no incoming control-flow edges.

Also visible after **VRP (Value Range Propagation)** which proves conditions are always true/false:
```bash
gcc -O3 -fdump-tree-vrp1 -c code.c
```
**Output:** `code.c.*.vrp1`  
**Note:** VRP determines that `i < -1` is always false (since `i` starts at 0 and increments), making the block unreachable.

---

## 7. Dead Code Elimination
```bash
gcc -O3 -fdump-tree-dce1 -c code.c
```
**Output:** `code.c.*.dce1`  
**Pass:** DCE removes statements whose results are never used (e.g., `temp_audit = 500`, `check1`, `check2`, `normalized`, `scaled_idx`).

For **Aggressive Dead Code Elimination** (also removes dead control flow):
```bash
gcc -O3 -fdump-tree-cddce1 -c code.c
```
**Output:** `code.c.*.cddce1`  
**Pass:** CD-DCE (Control Dependence DCE) is more aggressive — removes dead code and dead branches.

---

## 8. Loop Optimization
**Loop Invariant Code Motion (LICM):**
```bash
gcc -O3 -fdump-tree-lim2 -c code.c
```
**Output:** `code.c.*.lim2`  
**Pass:** Hoists loop-invariant expressions (e.g., `base_val + 15`, `apply_tax(33)`, `p * q`) out of the loop.

**Induction Variable Optimization:**
```bash
gcc -O3 -fdump-tree-ivopt -c code.c
```
**Output:** `code.c.*.ivopt`  
**Pass:** Optimizes loop induction variables (e.g., replacing `i * 4` with an additive counter `+= 4`).

**Loop Unrolling:**
```bash
gcc -O3 -fdump-tree-cunrolli -c code.c
```
**Output:** `code.c.*.cunrolli`  
**Pass:** Unrolls loops with known small trip counts.

---

## 9. Function Inlining & Function Cloning

**Function Inlining (Early):**
```bash
gcc -O3 -fdump-tree-einline -c code.c
```
**Output:** `code.c.*.einline`  
**Pass:** Small functions like `apply_tax()` and `process_batch()` are inlined — their bodies replace the call site.

**Function Inlining (Full):**
```bash
gcc -O3 -fdump-tree-inline -c code.c
```
**Output:** `code.c.*.inline`

**Function Cloning (IPA Constant Propagation):**
```bash
gcc -O3 -fdump-ipa-cp -c code.c
```
**Output:** `code.c.*.cp`  
**Pass:** IPA-CP creates specialized clones of functions for specific constant arguments (e.g., a clone of `apply_tax` specialized for `amount=33`).

---

## Quick Reference: Dump ALL Passes at Once
To generate dumps for **every** GIMPLE pass in one command:
```bash
gcc -O3 -fdump-tree-all -fdump-ipa-all -c code.c
```
This creates many files (one per pass). You can then compare any specific pass.

To clean up all dump files:
```bash
del code.c.* 2>nul
```
(On Linux/Mac: `rm -f code.c.*`)

---

## Recommended Workflow

```bash
# Step 1: Generate base GIMPLE (no optimization)
gcc -fdump-tree-gimple -c code.c

# Step 2: Generate all optimization pass dumps at O3
gcc -O3 -fdump-tree-all -fdump-ipa-all -c code.c

# Step 3: Compare specific passes
# The files are numbered in execution order, e.g.:
#   code.c.006t.gimple        ← base
#   code.c.012t.ccp1          ← after constant propagation/folding
#   code.c.014t.forwprop1     ← after algebraic simplification
#   code.c.016t.copyprop1     ← after copy propagation
#   code.c.018t.fre1          ← after CSE
#   code.c.022t.dce1          ← after dead code elimination
#   code.c.XXXt.vrp1          ← after unreachable code (VRP)
#   code.c.XXXt.lim2          ← after loop invariant motion
#   code.c.XXXt.einline       ← after function inlining
#   code.c.XXXt.optimized     ← final optimized GIMPLE
```

> **Note:** The exact numbering (e.g., `012t`, `014t`) may vary across GCC versions. The pass name suffix (`.ccp1`, `.fre1`, etc.) is always reliable.

---

## Pass Name Summary Table

| Optimization                  | GCC Flag                    | Pass Name     |
|-------------------------------|-----------------------------|---------------|
| Constant Propagation          | `-fdump-tree-ccp1`          | `ccp1`        |
| Constant Folding              | `-fdump-tree-ccp1`          | `ccp1`        |
| Algebraic Simplification      | `-fdump-tree-forwprop1`     | `forwprop1`   |
| Strength Reduction            | `-fdump-tree-optimized`     | `optimized`   |
| Copy Propagation              | `-fdump-tree-copyprop1`     | `copyprop1`   |
| CSE                           | `-fdump-tree-fre1`          | `fre1`        |
| Unreachable Code Elimination  | `-fdump-tree-vrp1`          | `vrp1`        |
| Dead Code Elimination         | `-fdump-tree-dce1`          | `dce1`        |
| Loop Invariant Motion         | `-fdump-tree-lim2`          | `lim2`        |
| Induction Variable Opt.       | `-fdump-tree-ivopt`         | `ivopt`       |
| Function Inlining             | `-fdump-tree-einline`       | `einline`     |
| Function Cloning              | `-fdump-ipa-cp`             | `cp`          |
| Final Optimized GIMPLE        | `-fdump-tree-optimized`     | `optimized`   |
