#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "proxy.hpp"

#define PORT "12345" // proxy port
int main(int argc, char *argv[])
{
    // FILE *fp = NULL;
    pid_t process_id = 0;
    pid_t sid = 0;
    // stdp1: Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0)
    {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // PARENT PROCESS. Need to kill it.
    if (process_id > 0)
    {
        printf("process_id of child process %d \n", process_id);
        // return success in exit status
        exit(0);
    }

    // catch, ignore, and handle singals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // int pid;
    // pid = fork();
    // if (pid < 0)
    // {
    //     exit(1);
    // }
    // if (pid > 0)
    // {
    //     printf("parent pid = %d \n", getpid());
    //     exit(0);
    // }

    // unmask the file mode
    umask(0);
    // set new session
    sid = setsid();
    if (sid < 0)
    {
        // Return failure
        exit(1);
    }
    // Change the current working directory to root.
    chdir("/");

    int x_fd;
    for (x_fd = sysconf(_SC_OPEN_MAX); x_fd > 0; x_fd--)
    {
        close(x_fd);
    }
    // redirect stdin, stderr, stdout to /dev/null
    int dev_null_fd = open("/dev/null", O_RDWR);
    if (dev_null_fd == -1)
    {
        perror("Error:");
        exit(EXIT_FAILURE);
    }

    int ret_in = dup2(dev_null_fd, STDIN_FILENO);
    int ret_out = dup2(dev_null_fd, STDOUT_FILENO);
    int ret_err = dup2(dev_null_fd, STDERR_FILENO);
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if (ret_in == -1 || ret_err == -1 || ret_out == -1)
    {
        perror("Error:");
        exit(1);
    }
    // Open a log file in write mode.
    // fp = fopen("Log.txt", "w+");
    while (1)
    {
        // Dont block context switches, let the process sleep for some time
        sleep(1);
        // fprintf(fp, "Logging info...\n");
        // fflush(fp);
        // Implement and call some function that does core work for this daemon.
        Proxy *proxy;
        int port = atoi(PORT);
        proxy = new Proxy(port);

        if (proxy->run())
        {
            std::cerr << "Error starting HTTPProxy" << std::endl;
        }

        delete proxy;
    }
    // fclose(fp);
    return (0);
}