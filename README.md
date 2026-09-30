# EECS 348 Lab 5 — C++ Matrix Operations

Submit the public repository URL. The submission includes `main.cpp`,
`Makefile`, `output.txt` (actual program output), and `ai_usage_log.md`.
`input.txt` is the supplied sample input. The README, `.gitignore`, and `tests/`
support building, practicing the demo, and reproducing checks.
Instructor handouts and compiled artifacts stay local and are ignored by Git.

## Build and run

```sh
make
./main
```

At the prompt, enter `input.txt` or another file path. The first integer is a
positive size N, followed by exactly two N×N matrices, A then B. Entries are
decimal integers in the C++ `int` range; whitespace separates them. Leading
zeroes are decimal. Filenames containing spaces work.

The program prints A, B, A+B, A×B, and the two diagonal sums of A. It then
demonstrates swapping rows 0 and 2, swapping columns 0 and 2, and updating
entry (1,2) to 99. Each edit starts from the original A, as in the sample.
Every numbered problem has its own function. Multiplication uses a row of A
and a column of B; see the assignment's
[matrix multiplication reference](https://www.mathsisfun.com/algebra/matrix-multiplying.html).

You can provide the filename directly, or supply all seven edit parameters:

```sh
./main input.txt
./main input.txt 0 3 1 2 2 1 -99
```

The argument order is `filename row1 row2 col1 col2 update_row update_col new_value`.
All indices start at 0. The second example swaps rows 0 and 3, swaps columns
1 and 2, and updates entry (2,1) to -99, each on a separate copy of A.
For a 1×1 input, use seven zeroes to demonstrate valid edits at index 0.
The default sample indices may be invalid for small N; each invalid operation
prints a message and the unchanged matrix, then continues.

Bad filenames, nonpositive N, malformed or missing values, extra data, and
out-of-range entries produce an error on standard error and exit status 1.
There is no fixed dimension limit; memory and runtime constrain large N.
Column widths adapt to the longest printed value, with at least one separating
space. Arithmetic uses `long long`, and an overflowing accumulated result is
reported instead of wrapping. A new value for an edit may use the `long long`
range. Swapping an index with itself is valid. For odd N, the shared centre
entry contributes once to each diagonal sum.

## Tests and output

```sh
make test
printf 'input.txt\n' | ./main > output.txt
make clean
```

The Python standard-library suite has 12 test methods. It checks the supplied
output, independent arithmetic results for sizes 1, 2, 3, 4, 5, and 7, identity
and zero matrices, negative and wide values, both diagonal sums, separate
edits of the original matrix, both index bounds, same-index swaps, malformed
files, missing files, EOF, command-line validation, and overflow detection.
`make clean` removes the executable.

## Cycle demo

Use Cycle for the live demo, matching the grading environment. Start from
your Mac terminal:

```sh
ssh m755b852@cycle1.eecs.ku.edu
```

Then run on Cycle:

```sh
demo_dir=$(mktemp -d /tmp/m755b852-c-test.XXXXXX)
trap 'cd /tmp; rm -rf -- "$demo_dir"' EXIT HUP INT TERM
git clone https://github.com/MatthewBisbee/eecs-348-lab-5.git "$demo_dir/repo"
cd "$demo_dir/repo"
hostname
g++ --version
make
./main
```

Enter `input.txt`. Explain entrywise addition, row-column multiplication,
the two diagonal index formulas, and bounds checks before editing. Then:

```sh
./main input.txt -1 2 0 4 4 0 99
printf '1\n3\n4\n' > "$demo_dir/single.txt"
./main "$demo_dir/single.txt" 0 0 0 0 0 0 99
make test
make clean
exit
```

The first command demonstrates invalid edits leaving A unchanged. The 1×1
case gives sum 7, product 12, and both diagonal sums 3. No live TA demo has
been performed by the assistant.

## AI usage

The conversation and significant coding actions are recorded in
[ai_usage_log.md](ai_usage_log.md), following the Lab 04 format. Only actual
student messages are numbered as interactions; automatic app and environment
context is excluded.

## KU Cycle validation

Tested on `cycle1.eecs.ku.edu` on September 30, 2026 with g++ 11.4.0.
`make clean all test` passed all 12 test methods without compiler warnings.
The generated sample output matched `output.txt` byte for byte; invalid edit
indices printed three unchanged matrices. Tracked files, commit history, and
clean Git status were checked before and after testing. `make clean` removed
the executable. A cleanup trap removed the unique remote temporary directory,
and a separate read-only SSH check verified its absence.

An unauthenticated fresh HTTPS clone of the public GitHub repository also
passed all 12 tests on Cycle 1 without warnings. The sample output matched,
Git stayed clean, and remote cleanup was verified separately.
