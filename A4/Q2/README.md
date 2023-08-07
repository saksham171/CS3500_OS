multi_pc.c
    contains the program for multiple producer-consumer problem.
Screenshot_TC.png
    contains a screenshot of the output produced by the program on the sample testcase given in the lab assignment pdf.
Report_2^2.pdf
    contains the report for the 2.2 question in the lab assignment pdf.
A Makefile has been made to execute the program.
make clean
    command is used to remove the existing objext files in the directory.
make
    command is used to compile the program.
So, do
make
./multi_pc -p 4 -pc 2 -c 2 -cc 2
    to run the program for the first time on a given input. Make sure that the input is in this order (p , pc , c , cc). And, for running it again, do
make clean
make
./multi_pc -p 4 -pc 2 -c 2 -cc 2
