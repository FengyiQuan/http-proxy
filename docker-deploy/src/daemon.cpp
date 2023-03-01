// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <string.h>
// #include <signal.h>
#include "proxy.hpp"

#define PORT "12345" // proxy port
// int main(int argc, char *argv[])
// {
//     // FILE *fp = NULL;
//     pid_t process_id = 0;
//     pid_t sid = 0;
//     // stdp1: Create child process
//     process_id = fork();
//     // Indication of fork() failure
//     if (process_id < 0)
//     {
//         printf("fork failed!\n");
//         // Return failure in exit status
//         exit(1);
//     }
//     // PARENT PROCESS. Need to kill it.
//     if (process_id > 0)
//     {
//         printf("process_id of child process %d \n", process_id);
//         // return success in exit status
//         exit(0);
//     }

//     // catch, ignore, and handle singals
//     signal(SIGCHLD, SIG_IGN);
//     signal(SIGHUP, SIG_IGN);

//     // int pid;
//     // pid = fork();
//     // if (pid < 0)
//     // {
//     //     exit(1);
//     // }
//     // if (pid > 0)
//     // {
//     //     printf("parent pid = %d \n", getpid());
//     //     exit(0);
//     // }

//     // unmask the file mode
//     umask(0);
//     // set new session
//     sid = setsid();
//     if (sid < 0)
//     {
//         // Return failure
//         exit(1);
//     }
//     // Change the current working directory to root.
//     chdir("/");

//     // int x_fd;
//     // for (x_fd = sysconf(_SC_OPEN_MAX); x_fd > 0; x_fd--)
//     // {
//     //     close(x_fd);
//     // }
//     // redirect stdin, stderr, stdout to /dev/null
//     int dev_null_fd = open("/dev/null", O_RDWR);
//     if (dev_null_fd == -1)
//     {
//         perror("Error:");
//         exit(EXIT_FAILURE);
//     }

//     int ret_in = dup2(dev_null_fd, STDIN_FILENO);
//     int ret_out = dup2(dev_null_fd, STDOUT_FILENO);
//     int ret_err = dup2(dev_null_fd, STDERR_FILENO);
//     // Close stdin. stdout and stderr
//     close(STDIN_FILENO);
//     close(STDOUT_FILENO);
//     close(STDERR_FILENO);

//     if (ret_in == -1 || ret_err == -1 || ret_out == -1)
//     {
//         perror("Error:");
//         exit(1);
//     }
//     // Open a log file in write mode.
//     // fp = fopen("Log.txt", "w+");
//     while (1)
//     {
//         // Dont block context switches, let the process sleep for some time
//         sleep(1);
//         printf("Creating proxy...\n");
//         // fflush(fp);
//         // Implement and call some function that does core work for this daemon.
//         Proxy *proxy;
//         int port = atoi(PORT);
//         proxy = new Proxy(port);

//         printf("Running proxy...\n");
//         if (proxy->run())
//         {
//             std::cerr << "Error starting HTTPProxy" << std::endl;
//         }

//         delete proxy;
//     }
//     // fclose(fp);
//     return (0);
// }

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

void create_daemon()
{
  // Step 1：使用 fork() 创建子进程，并让父进程退出
  pid_t cpid = fork(); // spawn children process
  if (cpid < 0)
  {
    exit(EXIT_FAILURE);
  }
  if (cpid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  // Step 2：脱离控制 tty
   // 离开它原来的进程组，避免
   // 向 tty 发送更多与终端操作相关的信号

  // setsid() create new ID for child process
  pid_t spid = setsid();
  if (spid < 0)
  {
    exit(EXIT_FAILURE);
  }

  // Step 3 point stdin/stdout/stderr at /dev/null
  // To dump all output to void that son't be useful, we direct them all to null

  int fd = open("/dev/null", O_RDWR, 0); // open file with Read and Write permission
  // Now fd represent file descriptor of /dev/null
  if (fd != -1)
  {
    // dup2(int oldfd, int newfd)
    // point newfd to oldfd
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2)
    {
      close(fd);
    }
  }
  // Step 4 Change working directory to "/"
  if (chdir("/") < 0)
  {
    exit(EXIT_FAILURE);
  }
  // Step 5 Set umask to 0
  // change permission mask to mode&0777
  // The following change to 0 for security reason
  umask(0);

  // Step 6 Use fork() again to make the process not a session leader
  cpid = fork();
  if (cpid < 0)
  {
    exit(EXIT_FAILURE);
  }
  if (cpid > 0)
  {
    exit(EXIT_SUCCESS); // have left parent process
  }

  Proxy *proxy;
  int port = atoi(PORT);
  proxy = new Proxy(port);

  printf("Running proxy...\n");
  if (proxy->run())
  {
    std::cerr << "Error starting HTTPProxy" << std::endl;
  }
}

int main()
{
  printf("Creating daemon\n");
  create_daemon();
  printf("Successful create daemon\n");

  // while (1)
  // {
  //   sleep(2);
  // }
}