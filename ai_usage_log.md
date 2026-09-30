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
[insert the "Canvas prompt.txt" here regarding the ai_usage_log.md]

### Assistant Summary

Read the Canvas prompt, AI logging instructions, companion PDF using the PDF skill, supplied input and expected output, and Lab 04 submission structure and corrected AI log format. Used Lab 04 only as a read-only reference. Read the KU Cycle C Test skill and the assignment's linked matrix multiplication reference. Generated main.cpp with separate functions for loading two dynamically sized matrices, addition, multiplication, diagonal sums, row swaps, column swaps, and element updates, plus reusable printing and validation. Each edit starts from the original Matrix A. Added filename prompting, optional command-line edit indices, aligned columns, malformed-file and index checks, and overflow detection using long long arithmetic for int-valued input. Created a g++ C++17 Makefile with all, clean, and test targets, retained the supplied input, generated output.txt from the executable, and added a standard-library Python test suite and expected-output fixture. The local build produced no warnings and all 12 test methods passed. Added README build and Cycle demo instructions and ignore rules excluding instructor handouts and compiled files. Created this log with the actual student request preserved verbatim, excluding automatic app and environment context as in the corrected Lab 04 log. Verified GitHub authentication and that the intended Lab 5 repository did not already exist. Initialized Git with separate commits for the program and supporting files. Used the KU Cycle skill to transfer a Git bundle into a unique temporary directory on cycle1.eecs.ku.edu, inspect tracked files and commit history, and verify clean Git status. With g++ 11.4.0, make clean all test passed all 12 test methods without warnings. The sample output matched output.txt byte for byte, invalid edits printed unchanged matrices, make clean removed the executable, and Git stayed clean. A cleanup trap removed the remote directory, and a separate read-only SSH check verified its absence. Recorded the Cycle results in the README. Created and pushed the public repository at https://github.com/MatthewBisbee/eecs-348-lab-5 and confirmed PUBLIC visibility. Verified an unauthenticated fresh HTTPS clone on Cycle 1: all 12 tests passed without compiler warnings, generated output matched the committed output, make clean removed the executable, and Git remained clean. Verified cleanup in a separate SSH check. The README provides instructions for the student to perform the required live demo; no live TA demo was performed.
