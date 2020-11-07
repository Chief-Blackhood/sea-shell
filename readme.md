To compile my code you have to run make command in your terminal and then type "./shell". This will run the main.c which loops until ctrl c interrupt or q option.

andothercommands.c is used to implement background process. I have allowed only 20 background process to be active at one point of a time in my shell.

cd.c is used to implement the shell in-built cd command.

curdir.c finds what is the current directory in which the user is in and also checks if any of the background process have exited/terminated and prints the signal with which they exited.

echo.c mimics echo shell command by doesn't take into consideration the different flags or special characters.

header.h find include all the header files that I myself have not implemented eg: <stdio.h>.

history.c file mimics history command and stores the last 20 commands in a file named history.txt which will be created where the exectutable in executed.

hostname.c prints the current user and system name i.e. prompt of the shell.

jobs.h is used to make my own data type which is a struct so that I can use in multiple .c files.

ls.c mimics the ls command. I have implemented -l,-a,-la,-al flags.

main.c is where most of the initilization occurs the the shell is run from. It is a infinite loop function but can be exited by writing q as a command.

makefile is used to complie all the .c files efficiently.

othercommands.c is used to implement all the foreground process using execvp, fork and waitpid.

parse.c is used to tokenize the commands passed into words.

pinfo.c is used to print the status, virtual memory used and the absoulte path of the executable of a given pid. If no pid is given the pid of the process calling the pinfo command will be used.

pwd prints the absolute path of where the user is in the terminal just like pwd inbuilt-command in shell.

All the remaining files are .h files of the above given .c files so that they can be included in the main.c file.

The piping is implemented in the commandHandle.c file. Each command in the pipe doesn't create a subshell process as piping with background process was not a requirement. After piping redirection is checked in the same file as redirection has a tighter bound than piping. Then all the other commands are executed.

Setting and deleting a enviroment variable happens in manageEnviro.c. These enviroment variables are current shell specific only and doesn't carry over to other shells.

Printing and killing(kjob) of jobs is achieved in printjobs.c which only prints the background processes started by the current shell.

fg and bg commands are implemented in the bgfg.c file. The bg commands continues the background process which were stopped and the fg commands brings the background process to the foreground.

BONUS Specification 1 and 2 has been implemented in the shell.

Assumptions:

1. The size of the names of various commands/ arguments are taken to be of resonable size an not exceeding 1024 characters.

2. & will be detected if it is written anywhere in the command except the first position as that is reserved for the command name. Writing & in the first position will just thrown an error syaing it is not a file/directory.

3. No special chacters will be given as inputs in echo or in cd command.

4. Atmax only 20 background process are allowed to run at the same time.

5. No more that 64 arguments will be given in a single command. Note: comma separation is counted as two individual commands and ecah can have 64 arguments.
