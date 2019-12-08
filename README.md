# The 2019 ICPC Asia Jakarta Regional Contest - Prolem Repository

## Repository Structure

Each problem (from A to L) has their own folder. The folder name is the `slug` of the problem, which is the codename we used when we internally prepare the problems. The following table describes the slug for each problem

| Problem Code | Problem Slug | Problem Title                     |
| ------------ | ------------ | --------------------------------- |
| A            | homework     | Copying Homework                  |
| B            | cleaning     | Cleaning Robots                   |
| C            | even         | Even Path                         |
| D            | find         | Find String in a Grid             |
| E            | song         | Songwriter                        |
| F            | forest       | Regular Forestation               |
| G            | perf         | Performance Review                |
| H            | twin         | Twin Buildings                    |
| I            | mission      | Mission Possible                  |
| J            | terrace      | Tiling Terrace                    |
| K            | robot        | Addition Robot                    |
| L            | construction | Road Construction                 |

In the problem folder, there should be exactly the following file/folder:

- `description.pdf`. The problem statement distributed to the contestants during the contest.
- `data/`. A folder consisting of all testcases. The sample inputs given in the problem statement will be in the form of `sample/<slug>_sample_<testcase number>.in/ans`, while the hidden testcases will be in the form of `secret/<slug>_1_<testcase number>.in/ans`.
- `solution.cpp`. The official solution used to generate the outputs of the testcases
- `scorer.cpp`. A validator used to validate contestants' output. The code accepts three arguments.
  - The first parameter will be used as the filename of the input file
  - The second parameter will be used as the filename of the judge's output file
  - The third parameter will be used as the filename of the contestant's output file
  - This code will print `WA` to stdout if the contestant's output is intended to be judged `WRONG ANSWER`, while it will print `AC` to stdout if the contestants's output is intended to be judged `CORRECT`.
- `verifier.py`. A verifier used to validate the input of the testcases. The code will crash if the input violates the problem constraint, while it will exit gracefully and will not output anything if the input satisfies the problem constraint.

## Limitations

The following table describes the time limit for each problem, as broadcasted to all teams at the beginning of the contest:

| Problem Code | Problem Slug | Time Limit |
| ------------ | ------------ | ---------- |
| A            | homework     | 1 sec      |
| B            | cleaning     | 1 sec      |
| C            | even         | 1 sec      |
| D            | find         | 5 sec      |
| E            | song         | 1 sec      |
| F            | forest       | 1 sec      |
| G            | perf         | 2 sec      |
| H            | twin         | 1 sec      |
| I            | mission      | 1 sec      |
| J            | terrace      | 1 sec      |
| K            | robot        | 3 sec      |
| L            | construction | 2 sec      |

The memory limit for all problems is 256MB, as broadcasted to all teams at the beginning of the contest.

## Solutions

Note that while there is only one official published solution, we have a lot of other solutions prepared for our internal testing. These other solutions include, but not limited to:

- Solutions in several other languages
- Solutions that is intended to be incorrect (e.g. WA, TLE, etc.)
- Other correct solutions (especially on problems with more than one correct outputs for each input)

## Errata

None.
