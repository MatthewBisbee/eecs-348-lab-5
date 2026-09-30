"""Black-box checks for the executable; uses only Python's standard library."""
import os
from pathlib import Path
import random
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
PROGRAM = Path(os.environ.get("MATRIX_PROGRAM", str(ROOT / "main")))


def read_matrix(output, heading, n):
    lines = output.split(heading + "\n", 1)[1].splitlines()
    if lines[0].startswith("Invalid "):
        lines = lines[1:]
    return [[int(v) for v in line.split()] for line in lines[:n]]


class MatrixTests(unittest.TestCase):
    def run_file(self, text, operations=None):
        with tempfile.TemporaryDirectory(prefix="lab5-test-") as folder:
            filename = Path(folder) / "matrix data.txt"
            filename.write_text(text)
            args = [str(PROGRAM), str(filename)]
            if operations is not None:
                args += [str(v) for v in operations]
            return subprocess.run(args, capture_output=True, text=True, timeout=10)

    def run_matrices(self, a, b, operations=None):
        text = str(len(a)) + "\n" + "\n".join(
            " ".join(map(str, row)) for row in a + b) + "\n"
        return self.run_file(text, operations)

    def check_arithmetic(self, a, b):
        run = self.run_matrices(a, b)
        self.assertEqual(run.returncode, 0, run.stderr)
        n = len(a)
        self.assertEqual(read_matrix(run.stdout, "Matrix A:", n), a)
        self.assertEqual(read_matrix(run.stdout, "Matrix B:", n), b)
        self.assertEqual(read_matrix(run.stdout, "A + B:", n),
                         [[x + y for x, y in zip(ar, br)] for ar, br in zip(a, b)])
        # Build B's columns independently from the C++ indexing loops.
        columns = list(zip(*b))
        self.assertEqual(read_matrix(run.stdout, "A * B:", n),
                         [[sum(x*y for x, y in zip(row, col)) for col in columns]
                          for row in a])
        self.assertIn(f"Main diagonal sum: {sum(a[i][i] for i in range(n))}\n", run.stdout)
        self.assertIn(f"Secondary diagonal sum: {sum(a[i][-1-i] for i in range(n))}\n", run.stdout)

    def test_supplied_sample(self):
        run = subprocess.run([str(PROGRAM)], input="input.txt\n", cwd=ROOT,
                             capture_output=True, text=True, timeout=10)
        self.assertEqual(run.returncode, 0, run.stderr)
        expected = (ROOT / "tests" / "sample_expected.txt").read_text()
        self.assertEqual(run.stdout.rstrip(), expected.rstrip())
        self.assertEqual((ROOT / "output.txt").read_text(), run.stdout)

    def test_known_small_matrices(self):
        for a, b in [([[3]], [[4]]), ([[1, 2], [3, 4]], [[5, 6], [7, 8]]),
                     ([[1, 2, 3], [4, 5, 6], [7, 8, 9]],
                      [[2, 0, 1], [1, 3, 0], [0, 1, 4]])]:
            with self.subTest(n=len(a)):
                self.check_arithmetic(a, b)

    def test_varied_sizes_and_negative_entries(self):
        randomizer = random.Random(348)
        for n in (1, 2, 3, 4, 5, 7):
            for _ in range(3):
                a, b = [[[randomizer.randint(-20, 20) for _ in range(n)]
                         for _ in range(n)] for _ in range(2)]
                self.check_arithmetic(a, b)

    def test_identity_and_zero(self):
        a = [[-10, 5000, 4], [3, -2, 17], [0, 8, -90]]
        self.check_arithmetic(a, [[1, 0, 0], [0, 1, 0], [0, 0, 1]])
        self.check_arithmetic(a, [[0]*3 for _ in range(3)])

    def test_edits_start_from_original(self):
        a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
        run = self.run_matrices(a, a, [2, 0, 1, 2, 2, 1, -12345])
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(read_matrix(run.stdout, "Problem 5 - Rows 2 and 0 swapped:", 3),
                         [a[2], a[1], a[0]])
        self.assertEqual(read_matrix(run.stdout, "Problem 6 - Columns 1 and 2 swapped:", 3),
                         [[1, 3, 2], [4, 6, 5], [7, 9, 8]])
        self.assertEqual(read_matrix(run.stdout, "Problem 7 - Updated matrix:", 3),
                         [[1, 2, 3], [4, 5, 6], [7, -12345, 9]])

    def test_all_index_bounds_and_same_indices(self):
        for n in (1, 3):
            a = [[i*n+j for j in range(n)] for i in range(n)]
            for first, second in [(0, n-1), (0, 0), (-1, 0), (0, -1),
                                  (n, 0), (0, n), (n+5, 0), (0, n+5)]:
                with self.subTest(n=n, first=first, second=second):
                    valid = 0 <= first < n and 0 <= second < n
                    run = self.run_matrices(a, a, [first, second, first, second,
                                                  first, second, 99])
                    self.assertEqual(run.returncode, 0, run.stderr)
                    rows = [row[:] for row in a]
                    cols = [row[:] for row in a]
                    updated = [row[:] for row in a]
                    if valid:
                        rows[first], rows[second] = rows[second], rows[first]
                        for row in cols:
                            row[first], row[second] = row[second], row[first]
                        updated[first][second] = 99
                    else:
                        self.assertEqual(run.stdout.count("matrix unchanged."), 3)
                    self.assertEqual(read_matrix(run.stdout,
                        f"Problem 5 - Rows {first} and {second} swapped:", n), rows)
                    self.assertEqual(read_matrix(run.stdout,
                        f"Problem 6 - Columns {first} and {second} swapped:", n), cols)
                    self.assertEqual(read_matrix(run.stdout, "Problem 7 - Updated matrix:", n), updated)

    def test_malformed_files(self):
        for text in ("", "0", "-2", "1.5 3 4", "abc", "2 1 2 3 4 5", "1 3 x",
                     "1 3.2 4", "1 3 4 trailing", "1 3 4 5", "1 2147483648 4",
                     "1 -2147483649 4", "9223372036854775808", "1 +-3 4"):
            with self.subTest(text=text):
                run = self.run_file(text)
                self.assertNotEqual(run.returncode, 0)
                self.assertIn("Error:", run.stderr)

    def test_missing_file_and_eof(self):
        with tempfile.TemporaryDirectory() as folder:
            run = subprocess.run([str(PROGRAM), str(Path(folder)/"missing.txt")],
                                 capture_output=True, text=True, timeout=10)
        self.assertNotEqual(run.returncode, 0)
        self.assertIn("Cannot open", run.stderr)
        run = subprocess.run([str(PROGRAM)], input="", capture_output=True,
                             text=True, timeout=10)
        self.assertNotEqual(run.returncode, 0)
        self.assertIn("No input filename", run.stderr)

    def test_whitespace_and_decimal_tokens(self):
        run = self.run_file("\t+1\n  03 \t +04\n\n", [0, 0, 0, 0, 0, 0, 5])
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(read_matrix(run.stdout, "A * B:", 1), [[12]])

    def test_wide_values_remain_separated(self):
        run = self.run_matrices([[2147483647, -2147483648], [0, 1]], [[1, 0], [0, 1]])
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(read_matrix(run.stdout, "A * B:", 2),
                         [[2147483647, -2147483648], [0, 1]])

    def test_product_overflow(self):
        for value in (2147483647, -2147483648):
            run = self.run_matrices([[value]*3 for _ in range(3)],
                                   [[2147483647]*3 for _ in range(3)])
            self.assertNotEqual(run.returncode, 0)
            self.assertIn("long long range", run.stderr)

    def test_invalid_command_line(self):
        for args in (["input.txt", "0"],
                     ["input.txt", "0", "2", "0", "2", "1", "2", "2.5"]):
            run = subprocess.run([str(PROGRAM), *args], cwd=ROOT,
                                 capture_output=True, text=True, timeout=10)
            self.assertNotEqual(run.returncode, 0)
            self.assertIn("Error:", run.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
