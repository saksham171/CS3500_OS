phil.c
    contains the program for the dining philosopher's problem.
Screenshot_TC.png
    contains a screenshot of the output produced by the program on a sample testcase given in the lab assignment pdf.
A Makefile has been made to execute the program.
make clean
    command is used to remove the existing objext files in the directory.
make
    command is used to compile the program
So, do
make
./phil n
    to run the program to solve the dining philosophers problem with n philosophers for the first time. And, for running it again, do
make clean
make
./phil n
