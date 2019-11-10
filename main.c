#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "pipelib.h"

int main() {
  int fds[2]; 
  /*
   - fd : file descriptor
   - fds[0] for using read end
   - fds[1] for using write end 
  */
  int pipe_creator_status = 0; 
  
  __pid_t pid;
  /*
    - __pid_t is datatype stands for process identification
    - pid represent process ids
  */

  pipe_creator_status = pipe(fds);
  if(pipe_creator_status == -1) {
    perror("@Create Pipe Error");
    exit(0);
  }
  
  pid = fork(); // create child_process
  /*
    fork() returns 
    - < 0 : fail to create child process
    - = 0 : for child_process
    - > 0 : + process ID of the child process to the parent process. 
            + When >0 parent process will execute.
  */
  if(pid < 0) {
    perror("@Create Pipe Error");
    exit(0);
  } else if(pid > 0) {
    parentProcessing(fds);
  } else {
    childProcessing(fds);
  }
}