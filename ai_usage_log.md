# AI Usage Log

**Platform:** Codex desktop  
**Model:** GPT-6

---

## Interaction 1

### User

See the files pointed at in your current directory within Lab 05 folder.

Follow all instructions as asked within these. The final result is a github repo link. Utilize the KU cycles skill for testing.

Once you have complete the exercises, to prepare for submission and github push, see the Lab 04 next to this Lab 05 directory (read only on any of the other labs than 5). You will see its structure, AI usage log formatting, and which files should be omitted and which should make the final push. You can also reference these at any point to address any vagueness in your own process.

Assignment description:
bjective: Get familiar with C++ programming and practice Git and make again. During the C++ programming, you will practice the basic file operations, if-statement, loop, function calls, and output format control.

What to turn in: Please provide a URL to your GitHub repository. If you are unable to push your code to GitHub, you may instead use KU GitLab, which works in a very similar way. Make sure the repository is public. Otherwise, the TAs won't be able to grade it. 

Grading:

    Each of the first two questions is worth 10 points, and each of the remaining questions is worth 15 points. [95 points]
    A valid Makefile must be included in the repository (similar to the Makefile for C, but replace the compiler gcc with g++). [5 points]

Programming problem: Matrix Operations

1. Read values from a file into the matrix:
Implement a function to load matrix data from a user-specified file (you can use fstream/ifstream). The first line of the file should contain an integer N indicating the size of the matrices, followed by two N × N matrices. After reading the file, print the matrices with proper formatting, such as aligned columns. Note that N can be any valid positive integer.

2. Add two matrices and display the result:
Implement a function that adds two matrices (you can use vector, i.e., the dynamic array, to store the matrix) and displays the resulting matrix.

3. Multiply two matrices and display the result:
Implement a function that multiplies two matrices and displays the resulting matrix.

Please refer to the link for the matrix multiplication: https://www.mathsisfun.com/algebra/matrix-multiplying.html

4. Get the sums of matrix diagonal elements:
Implement a function that calculates and displays, separately, the sum of the main diagonal elements and the sum of the secondary diagonal elements of a matrix.

5. Swap matrix rows and display the result:
Implement a function that takes a matrix and two row indices as input (with indexing starting at 0). If both indices are within valid bounds, swap the specified rows and output the resulting matrix.

6. Swap matrix columns and display the result:
Implement a function that takes a matrix and two column indices as input (with indexing starting at 0). If both indices are within valid bounds, swap the specified columns and output the resulting matrix.

7. Update a matrix element and display the result:
Implement a function that accepts a matrix, a row index, a column index, and a new value. If both indices are valid (with indexing starting at 0), update the element at the specified position and display the resulting matrix.

Sample Input

4 // The size of the square matrix (N) and the input file should not include this comment

01 02 03 04
05 06 07 08
09 10 11 12
13 14 15 16
13 14 15 16
09 10 11 12
05 06 07 08
01 02 03 04

### Assistant Summary

Read the Canvas prompt, AI logging instructions, companion PDF using the PDF skill, supplied input and expected output, and Lab 04 submission structure and corrected AI log format. Used Lab 04 only as a read-only reference. Read the KU Cycle C Test skill and the assignment's linked matrix multiplication reference. Generated main.cpp with separate functions for loading two dynamically sized matrices, addition, multiplication, diagonal sums, row swaps, column swaps, and element updates, plus reusable printing and validation. Each edit starts from the original Matrix A. Added filename prompting, optional command-line edit indices, aligned columns, malformed-file and index checks, and overflow detection using long long arithmetic for int-valued input. Created a g++ C++17 Makefile with all, clean, and test targets, retained the supplied input, generated output.txt from the executable, and added a standard-library Python test suite and expected-output fixture. The local build produced no warnings and all 12 test methods passed. Added README build and Cycle demo instructions and ignore rules excluding instructor handouts and compiled files. Created this log with the actual student request preserved verbatim, excluding automatic app and environment context as in the corrected Lab 04 log. Verified GitHub authentication and that the intended Lab 5 repository did not already exist. Initialized Git with separate commits for the program and supporting files. Used the KU Cycle skill to transfer a Git bundle into a unique temporary directory on cycle1.eecs.ku.edu, inspect tracked files and commit history, and verify clean Git status. With g++ 11.4.0, make clean all test passed all 12 test methods without warnings. The sample output matched output.txt byte for byte, invalid edits printed unchanged matrices, make clean removed the executable, and Git stayed clean. A cleanup trap removed the remote directory, and a separate read-only SSH check verified its absence. Recorded the Cycle results in the README. Created and pushed the public repository at https://github.com/MatthewBisbee/eecs-348-lab-5 and confirmed PUBLIC visibility. Verified an unauthenticated fresh HTTPS clone on Cycle 1: all 12 tests passed without compiler warnings, generated output matched the committed output, make clean removed the executable, and Git remained clean. Verified cleanup in a separate SSH check. The README provides instructions for the student to perform the required live demo; no live TA demo was performed.
