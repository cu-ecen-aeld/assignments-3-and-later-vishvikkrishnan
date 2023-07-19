#include "systemcalls.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int status = 0;
    if (cmd == NULL) {
        return true;
    }
    status = system(cmd);
    if (status == -1) {
        perror("system() failed");
        return false;
    }
    if (status == 127) {
        perror("Could not execute a shell in the child process");
        return false;
    }
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    int child_status;
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork() failed");
        return false;
    }

    // Run execv() inside the child process
    else if (child_pid == 0) {
        if (execv(command[0], command) == -1) {
            perror("Failed to run exec()");
            exit(EXIT_FAILURE);
        }
    }

    // Wait on child process
    else if (waitpid(child_pid, &child_status, 0) == -1) {
            perror("waitpid() failed!");
            return false;
    }

    // Check exit status from the child process 
    else if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0) {
        return true;
    }

    va_end(args);
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    int fd = open(outputfile, O_WRONLY|O_CREAT, 0644);

    if (fd == -1) {
        perror("Error opening file!");        
        return false;
    }
    int child_pid = fork();
    int child_status;

    if (child_pid == -1) {
        perror("fork() failed");
        close(fd);
        return false;
    }

    // Run execv() inside the child process
    else if (child_pid == 0) {
        if (dup2(fd, 1) < 0) {
            perror("Unable to duplicate file descriptor");
            return false;
        }
        close(fd);
        child_status = execvp(command[0], command);
        if (child_status == -1) {
            perror("Failed to run exec()");
            return false;
        }
    }

    // Wait on child process
    else if (waitpid(child_pid, &child_status, 0) == -1) {
        perror("waitpid() error");
        return false;
    }

    // Check exit status from the child process 
    else if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0) {
        return true;
    }

    va_end(args);
    return false;
}
