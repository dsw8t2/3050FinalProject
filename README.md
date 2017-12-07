# 3050FinalProject
Mizzou cs3050 final project
By: Kyle Deppe
Sahand Sadri
John Umstead
Daniel Weiss

To run you need a C compiler, use make to compile it
"./a.out room.txt" is the command to execute the program

Run in gdb to see error reporting
Error Reporting:
Exit code 1: incorrect number of command line arguments
Exit code 2: input file failed to open 
Exit code 3: input file failed to close
Exit code 4: parsing error invalid character encountered
Exit code 5: parsing error empty file

output should show the room with the individual paths. The path between 's' and 'e' has 1's and the path between 'f' and 'l' is 0's. 
Unless there is an error then GDB will show you the exit codes that we have listed above