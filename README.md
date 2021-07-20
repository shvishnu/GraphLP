# GraphLP
Finding a Stable Matching with flexible quotas, given a graph where every vertex has a strict order of preference order of its neighbours using Linear Programming.

## Dependencies:
+ clang compiler, cmake, ninja, cplex
+ Python dependencies are given in solver/requirements.txt


## Installation
    $ mkdir build; cd build
    $ cmake -G "Ninja" ../
    $ ninja

You can replace Ninja with "Unix Makefiles" in the above command.
Then type make to build.

This should build an executable named getlp and rev inside the build directory.


## Usage
The executable takes a set of parameters to compute the desired matching:

	-i -- /path/to/preferencelist
	-q -- /path/to/store/the/quadruplelp (ending with .lp)
    -t -- /path/to/store/the/tripletlp (ending with .lp)
To use the lpsolver to compute the matching for the generated LPs, use `lpsolver.sh`

For e.g., to compute the matching for input input.txt, use the following command:

	$ bash lpsolver.sh quadruple.lp triplet.lp input.txt
The output will be stored in solution.txt
