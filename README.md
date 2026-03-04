# Converting C Code to Three-Address Code (3AC) and Viewing Output

This guide explains how to generate **Three-Address Code (3AC)** from `code.c` using GCC and how to view the output.

---

## Prerequisites

- **GCC** (MinGW on Windows, or GCC on Linux/macOS)
- A terminal / command prompt

> If you don't have GCC, install MinGW-w64 from [https://www.mingw-w64.org/](https://www.mingw-w64.org/) or use `pacman -S mingw-w64-x86_64-gcc` on MSYS2.

---

## What is Three-Address Code (3AC)?

Three-Address Code is an **intermediate representation (IR)** used by compilers where each instruction has **at most three operands** — typically:

```
result = operand1 op operand2
```

GCC generates this IR internally and can dump it via GIMPLE (GCC's 3AC-like IR).

---

## Method 1: Using GCC GIMPLE Dump (Recommended)

GCC can dump its internal **GIMPLE** representation, which is essentially Three-Address Code.

### Step 1: Generate 3AC (GIMPLE) without optimizations

```bash
gcc -fdump-tree-gimple -O0 -o code.exe code.c
```

This produces a file named something like:

```
code.c.005t.gimple
```

### Step 2: View the 3AC output

```bash
# Windows (PowerShell)
Get-Content code.c.005t.gimple

# Windows (CMD)
type code.c.005t.gimple

# Linux / macOS
cat code.c.005t.gimple
```

### Step 3: Generate 3AC with optimizations (to compare)

```bash
gcc -fdump-tree-gimple -O3 -o code_opt.exe code.c
```

This generates an optimized GIMPLE file. Compare the two to see how optimizations transform the 3AC.

---

## Method 2: View All Optimization Passes

To see how the 3AC changes through each compiler pass:

```bash
gcc -fdump-tree-all -O0 -o code.exe code.c
```

This generates **many** `.tree` dump files showing every transformation stage. Key files to look at:

| File Pattern                  | What It Shows                        |
|-------------------------------|--------------------------------------|
| `*.gimple`                    | Initial 3AC (GIMPLE)                 |
| `*.ssa`                       | Static Single Assignment form        |
| `*.optimized`                 | Final optimized tree IR              |
| `*.cfg`                       | Control Flow Graph                   |

---

## Method 3: View SSA Form (Enhanced 3AC)

SSA form is a refined version of 3AC where each variable is assigned exactly once:

```bash
gcc -fdump-tree-ssa -O0 -o code.exe code.c
```

View the output:
```bash
# Windows (PowerShell)
Get-Content code.c.*ssa

# Linux / macOS
cat code.c.*ssa
```

---

## Method 4: View RTL (Lower-Level IR)

To see the Register Transfer Language representation (closer to assembly):

```bash
gcc -fdump-rtl-expand -O0 -o code.exe code.c
```

---

## Quick Command Summary

| Command | Purpose |
|---------|---------|
| `gcc -fdump-tree-gimple -O0 -o code.exe code.c` | 3AC without optimization |
| `gcc -fdump-tree-gimple -O3 -o code_opt.exe code.c` | 3AC with optimization |
| `gcc -fdump-tree-all -O0 -o code.exe code.c` | All IR passes (unoptimized) |
| `gcc -fdump-tree-all -O3 -o code_opt.exe code.c` | All IR passes (optimized) |
| `gcc -fdump-tree-ssa -O0 -o code.exe code.c` | SSA form |
| `gcc -fdump-tree-optimized -O3 -o code_opt.exe code.c` | Final optimized IR |

---

## Running the Compiled Program

After compilation, run the executable to see the program output:

```bash
# Windows
.\code.exe

# Linux / macOS
./code.exe
```

**Expected output:**
```
Final Balance: 195000000
```

---

## Example: What the GIMPLE 3AC Looks Like

For the function `apply_tax(int amount)`:

**Original C:**
```c
int apply_tax(int amount) {
    return amount + 5;
}
```

**GIMPLE 3AC (approximate):**
```
apply_tax (int amount)
{
  int D.XXXX;

  D.XXXX = amount + 5;
  return D.XXXX;
}
```

For the loop body, the 3AC breaks every expression into simple three-address instructions:

**Original C:**
```c
int step1 = base_val + 15;
int step2 = step1;
int final_step = step2;
int scaled_idx = i * 4;
int normalized = (scaled_idx * 1) + 0;
```

**GIMPLE 3AC (approximate):**
```
step1 = base_val + 15;
step2 = step1;
final_step = step2;
scaled_idx = i * 4;
_tmp1 = scaled_idx * 1;
normalized = _tmp1 + 0;
```

---

## Step-by-Step Walkthrough

### 1. Open a terminal in this folder

```bash
cd "f:\ASC 23-27\Sem 6\Compiler Designs\Case Study\Revised"
```

### 2. Generate unoptimized 3AC

```bash
gcc -fdump-tree-gimple -O0 -o code.exe code.c
```

### 3. Find the generated GIMPLE file

```bash
# PowerShell
Get-ChildItem *.gimple
```

### 4. Read the 3AC

```bash
Get-Content code.c.005t.gimple
```

### 5. Generate optimized 3AC for comparison

```bash
gcc -fdump-tree-gimple -O3 -o code_opt.exe code.c
```

### 6. Compare both files

```bash
# PowerShell - view side by side
Compare-Object (Get-Content code.c.005t.gimple) (Get-Content code_opt.exe-code.c.005t.gimple)
```

### 7. Run the program

```bash
.\code.exe
```

---

## Optimizations Visible in 3AC

When comparing `-O0` vs `-O3` GIMPLE dumps, you can observe these optimizations from `code.c`:

| Optimization | What to Look For |
|---|---|
| **Constant Folding** | `40 * 2` → `80`, `base_val + 15` → `40` |
| **Constant Propagation** | `base_val = 25` propagated into `step1 = 25 + 15` |
| **Copy Propagation** | `step2 = step1; final_step = step2` → `final_step = step1` |
| **Algebraic Simplification** | `scaled_idx * 1 + 0` → `scaled_idx` |
| **Strength Reduction** | `i * 4` → `i << 2` (shift instead of multiply) |
| **Common Subexpression Elimination** | `p * q` computed once, reused for `check1` and `check2` |
| **Dead Code Elimination** | `temp_audit = 500` removed (unused) |
| **Unreachable Code Elimination** | `if (i < -1)` block removed (always false for loop `i >= 0`) |
| **Function Inlining** | `apply_tax(33)` replaced with `33 + 5 = 38` inline |

---

## Cleanup

To remove all generated dump files:

```bash
# PowerShell
Remove-Item *.gimple, *.ssa, *.optimized, *.tree, *.exe -ErrorAction SilentlyContinue

# Linux / macOS
rm -f *.gimple *.ssa *.optimized *.tree *.exe
```
