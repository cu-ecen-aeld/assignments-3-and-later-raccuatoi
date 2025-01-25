#include "systemcalls.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
    int status = system(cmd);
    return status == 0; 
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
    int status;
    pid_t pid;
    pid = fork();
    if (pid == -1) 
      return false;
    // Child process
    else if (pid == 0) {
      va_list args;
      va_start(args, count);
      char* command[count+2];
      command[0] = va_arg(args, char*);
      int i = strlen(command[0]) - 1;
      while (i >= 0 && command[0][i] != '/') i -= 1; 
      i = i + 1;
      int j = 0;
      char buff[128];
      while (i < strlen(command[0])) {
        buff[j] = command[0][i];
        i = i + 1;
        j = j + 1;
      }
      buff[j] = 0;
      command[1] = buff;
      for (int i = 1; i < count; ++i) {
        command[i+1] = va_arg(args, char*);
      }
      command[count+1] = NULL;
      va_end(args);
      execv(command[0], &command[1]);
      exit(-1); // If child process is in error, return here.
    } 
    // Parent proc
    int ret = waitpid(pid, &status, 0);
    if (ret == -1) {
      return false;
    } else if (WIFEXITED(status)) {
      return WEXITSTATUS(status) == 0;
    }
    return false;
}



/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd < 0) {
      return false;
    }
    int status;
    pid_t pid;
    pid = fork();
    if (pid == -1) 
      return false;
    // Child process
    else if (pid == 0) {
      if (dup2(fd, 1) < 0) {
          perror("Error duplicating the file descriptor.");
          return false;
      }
      close(fd);
      va_list args;
      va_start(args, count);
      char* command[count+2];
      command[0] = va_arg(args, char*);
      int i = strlen(command[0]) - 1;
      while (i >= 0 && command[0][i] != '/') i -= 1; 
      i = i + 1;
      int j = 0;
      char buff[128];
      while (i < strlen(command[0])) {
        buff[j] = command[0][i];
        i = i + 1;
        j = j + 1;
      }
      buff[j] = 0;
      command[1] = buff;
      for (int i = 1; i < count; ++i) {
        command[i+1] = va_arg(args, char*);
      }
      command[count+1] = NULL;
      va_end(args);
      execv(command[0], &command[1]);
      exit(-1); // If child process is in error, return here.
    } 
    // Parent proc
    close(fd);
    int ret = waitpid(pid, &status, 0);
    if (ret == -1) {
      return false;
    } else if (WIFEXITED(status)) {
      return WEXITSTATUS(status) == 0;
    }
    return false;
}
