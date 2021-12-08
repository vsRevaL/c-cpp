# Unix Processes in C

## Main Reference: *CodeVault*
 - https://youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY

## The fork() function in C

`#include <unistd.h>`

`number of processes = 2^(num of fork() calls)` <br>
`1 fork -> 2 process` <br>
`2 fork -> 4 process` <br>
`3 fork -> 8 process` <br>
`4 fork -> 16` <br>
`10 fork -> 1024` <br>
`20 fork -> 1048576` <br>

It's not recommended to use fork() in a loop - 2^n fork() calls

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main()
{
    int id = fork();
    if (id == 0) {
        printf("Hello from child process\n");
    }
    else {
        printf("Hello from the parent process\n");
    }

    return 0;
}
```

```cs
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/**
 * Task: Create 3 processes only.
**/
int main()
{
    int id = fork();
    if (id != 0)
    {
        fork();
    }
    printf("Hi\n"); // prints "Hi" 3x
    return 0;
}
```
<br>

## Waiting for processes to finish (using the wait function) in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/**
 * Task: Print nums from 1 to 10
 * The child process prints 1-5
 * and the parent prints 6-10
**/
int main() {
    pid_t id = fork();
    int n;
    if (id == 0) { //Child process
        n = 1;
    }
    else {
        wait(NULL); //KEY! waits for the child process to finish printing 1-5
        n = 6;
    }
    
    for (int i = n; i <= n + 4; i++) {
        printf("%d ", i);
        fflush(stdout);
    }

    if (id > 0) {
        printf("\n");
    }

    return 0;
}
```

<br>

## Process IDs in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t id = fork();
    // getpid() - current process ID
    // getppid() - parent process ID
    printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());
    return 0;
}
```

<br>

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t id = fork();
    if (id == 0) {
        sleep(1);
    }
    // getpid() - current process ID
    // getppid() - parent process ID
    printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());

    int res = wait(NULL);
    if (res == -1) {
        printf("No children to wait for\n");
    }
    else {
        printf("%d finished execution\n", res);
    }

    return 0;
}
```

```cs
Current ID: 154, parent ID: 40
Current ID: 155, parent ID: 154
No children to wait for
155 finished execution
```

<br>

## Calling fork multiple times

![img.png](img.png)

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t id1 = fork();
    pid_t id2 = fork();
    if (id1 == 0) { //We're in the child of the parent process
        if (id2 == 0) { //We're in the child of that child process
            printf("We're in process [y]\n");
        }
        else { 
            printf("We're in process [x]\n");
        }
    }
    else {
        if (id2 == 0) {
            printf("We are process [z]\n");
        }
        else {
            printf("We are the parent process!\n");
        }
    }

    // To wait for all the processes we should wait for:
    while (wait(NULL) != -1 || errno != ECHILD) {
        printf("Waited for a child to finish\n"); // gets printed 3x
    }

    return 0;
}
```

<br>

## Communicating between processes (using pipes) in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * Getting a number input from user int the child process,
 * and send that number to the main (parent) process. 
**/
int main() {
    int fd[2];
    // fd[0] - read
    // fd[1] - write

    if (pipe(fd) == -1) {
        printf("[ERROR]: An error occurred with opening the pipe\n");
        return 1;
    }

    // Be better fork() after creating the pipe!
    int id = fork();
    if (id == -1) {
        printf("[ERROR]: An error occurred while forking\n");
        return 2;
    }

    if (id == 0) {
        sleep(1); // wasting a bit of time
        close(fd[0]);
        int x;
        printf("Input a number: ");
        scanf("%d", &x);
        if (write(fd[1], &x, sizeof(int)) == -1) {
            printf("An error occurred with writing to the pipe\n");
            close(fd[1]); 
            return 3;
        }
        close(fd[1]); 
    }
    else {
        printf("Waited for child\n");
        close(fd[1]);
        int y;
        if (read(fd[0], &y, sizeof(int)) == -1) {
            printf("An error occurred with reading from the pipe\n");
            close(fd[0]); 
            return 4;
        }
        close(fd[0]);
        y *= 3;
        printf("Got from child process: %d\n", y);
        wait(NULL);
    }

    return 0;
}
```

<br>

## Practical use case for fork and pipe in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * We want the sum of a array
 * one process calculates the first half another the second
 * and send the partial sum to the main process 
**/
int main() {
    int arr[] = {1, 2, 3, 4, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int start, end;
    int fd[2];
    if (pipe(fd) == -1) {
        printf("[ERROR]: An error occurred with opening a pipe\n");
        return 1;
    }

    pid_t id = fork();
    if (id == -1) {
        printf("[ERROR]: Fork failed\n");
        return 2;
    }

    if (id == 0) { // Child process
        start = 0;
        end = n / 2;
    }
    else {
        start = n / 2;
        end = n;
    }

    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    printf("Calculated partial sum: %d\n", sum);

    return 0;
}
```

So far so good. <br>
Now we just need to send the information from one process to the another.

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * We want the sum of a array
 * one process calculates the first half another the second
 * and send the partial sum to the main process 
**/
int main() {
    int arr[] = {1, 2, 3, 4, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int start, end;
    int fd[2];
    if (pipe(fd) == -1) {
        printf("[ERROR]: An error occurred with opening a pipe\n");
        return 1;
    }

    pid_t id = fork();
    if (id == -1) {
        printf("[ERROR]: Fork failed\n");
        return 2;
    }

    if (id == 0) { // Child process
        start = 0;
        end = n / 2;
    }
    else {
        start = n / 2;
        end = n;
    }

    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    printf("Calculated partial sum: %d\n", sum);

    // If we're in the child, we send to information to the parent process
    if (id == 0) { 
        close(fd[0]);
        if (write(fd[1], &sum, sizeof(sum)) == -1) {
            printf("[ERROR]: Error occurred with writing the pipe\n");
            close(fd[1]);
            return 2;
        }
        close(fd[1]);
    }
    else {
        close(fd[1]);
        int sumFromChild = 0;
        if (read(fd[0], &sumFromChild, sizeof(int)) == -1) {
            printf("[ERROR]: Error occurred with reading the pipe\n");
            close(fd[0]);
            return 3;
        }
        close(fd[0]);

        int totalSum = sum + sumFromChild;
        printf("Total sum if %d\n", totalSum);
        wait(NULL);
    }

    return 0;
}
```

<br>

## Introduction to FIFOs (aka named pipes) in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    char pipename[20];
    sprintf(pipename, "/tmp/%d", getpid());
    if (mkfifo(pipename, S_IRUSR | S_IWUSR) == -1) {
        if (errno != EEXIST) {
            printf("[ERROR]: with opening the fifo pipe\n");
            return 1;
        }
    }

    // FIFOs:
    // Opening the read or write end of a FIFO blocks until the other end is also opened
    // (by another process or thread)
    int fd = open(pipename, O_WRONLY); // so the terminal stops.
    int x = 97;
    if (write(fd, &x, sizeof(x)) == -1) {
        printf("[ERROR]: Error with writing the fifo pipe\n");
        return 2;
    }
    close(fd);

    return 0;
}

```

<br>

## How to use FIFO files to communicate between processes in C

```cs

```

<br>

## Two way communication between processes (using pipes) in C

Take a look at this code, looks nice. But..

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * Generate a number [1-5], send to the other process
 * who multiplies the number by 4, and sends back
 * 5 =>
 * 5 * 4 = 20
 * => prints 20
**/ 
int main(int argc, char *argv[])
{
    int p1[2];
    if (pipe(p1) == -1) {
        printf("[ERROR]: Creating pipe\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("[ERROR]: forking\n");
        return 2;
    }

    if (pid == 0) {//Child
        int x;
        if (read(p1[0], &x, sizeof(x)) == -1) {
            printf("[ERROR]: child - reading pipe\n");
            return 3;
        }
        printf("Received %d\n", x);

        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1) {
            printf("[ERROR]: child - writing pipe\n");
            return 4;
        }
        printf("Wrote: %d\n", x);
    }
    else { //Parent
        int x = 5;
        if (write(p1[1], &x, sizeof(x)) == -1) {
            printf("[ERROR]: parent - writing pipe\n");
            return 5;
        }
        printf("Wrote: %d\n", x); 

        if (read(p1[0], &x, sizeof(x)) == -1) {
            printf("[ERROR]: parent - reading pipe\n");
            return 6;
        }
        printf("Result: %d\n", x);
        wait(NULL);
    }
    close(p1[0]);
    close(p1[1]);
    
    return 0;
}
```

Well.. this pretty code will not work properly.

**We cannot have a pipe that both sends and receives data!!!**

**We have to have 2 pipes in order to achieve this solution.**


Let's see:

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * Generate a number [1-5], send to the other process
 * who multiplies the number by 4, and sends back
 * 5 =>
 * 5 * 4 = 20
 * => prints 20
**/ 
int main(int argc, char *argv[])
{
    int p1[2]; //child -> parent
    int p2[2]; //parent -> child

    // 0 - read ; 1 - write
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        printf("[ERROR]: Creating pipe\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("[ERROR]: forking\n");
        return 2;
    }

    if (pid == 0) { //Child
        close(p1[0]); //We don't need to read from the first pipe on the child process.
        close(p2[1]); //We don't need to write anything with the 2nd pipe
        int x;
        if (read(p2[0], &x, sizeof(x)) == -1) {
            printf("[ERROR]: child - reading pipe\n");
            return 3;
        }
        printf("Received %d\n", x);

        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1) {
            printf("[ERROR]: child - writing pipe\n");
            return 4;
        }
        printf("Wrote: %d\n", x);

        close(p1[1]);
        close(p2[0]);
    }
    else { //Parent
        close(p1[1]);
        close(p2[0]);
        int x = 5;
        if (write(p2[1], &x, sizeof(x)) == -1) {
            printf("[ERROR]: parent - writing pipe\n");
            return 5;
        }
        printf("Wrote: %d\n", x); 

        if (read(p1[0], &x, sizeof(x)) == -1) {
            printf("[ERROR]: parent - reading pipe\n");
            return 6;
        }
        printf("Result: %d\n", x);

        close(p1[0]);
        close(p2[1]);
        wait(NULL);
    }

    return 0;
}
```

This guaranteed to always work.

## How to send an array through a pipe

Spoiler: We need to send the array size (n) as well

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * 2 processes
 * 1) Child proc should generate random numbers
 * and send them to the parent
 * 2) parent is will sum them and print the result 
**/ 
int main() {

    int fd[2]; //child -> parent
    if (pipe(fd) == -1) {
        printf("[ERROR]: creating pipe\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("[ERROR]: forking\n");
        return 2;
    }

    if (pid == 0) { //Child
        close(fd[0]);
        int n;
        int arr[10];
        srand(pid);
        n = rand() % 10 + 1;
        printf("Generated: ");
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 11;
            printf("%d ", arr[i]);
        }
        printf("\n");
        if (write(fd[1], &n, sizeof(int)) < 0) {
            printf("[ERRO]: writing arr size\n");
            return 3;
        }
        printf("Sent n= %d\n", n);
        if (write(fd[1], arr, sizeof(int) * n) < 0) {
            printf("[ERROR]: writing pipe\n");
            return 4;
        }
        close(fd[1]);
    }
    else { //Parent
        close(fd[1]);
        int arr[10];
        int n;
        if (read(fd[0], &n, sizeof(int)) < 0) {
            printf("[ERROR]: reading arr size\n");
            return 5;
        }
        if (read(fd[0], arr, sizeof(int) * n) < 0) {
            printf("[ERROR]: reading arr size\n");
            return 6;
        }

        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += arr[i];
        }
        printf("Result is: %d\n", sum);

        close(fd[0]);
        wait(NULL);
    }
    
    return 0;
}
```

## How to send a string through a pipe

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/** Task:
 * 2 processes
 * 1) Child proc should generate random numbers
 * and send them to the parent
 * 2) parent is will sum them and print the result 
**/ 
int main() {

    int fd[2]; //child -> parent
    if (pipe(fd) == -1) {
        printf("[ERROR]: creating pipe\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("[ERROR]: forking\n");
        return 2;
    }

    if (pid == 0) { //Child proc
        close(fd[0]);
        char str[200];
        printf("Input a string: ");
        fgets(str, 200, stdin);
        str[strlen(str) - 1] = '\0';
        int n = strlen(str) + 1;
        if (write(fd[1], &n, sizeof(int)) == -1) {
            printf("[ERROR]: writing string size\n");
            return 3;
        }

        if (write(fd[1], str, sizeof(char) * strlen(str) + 1) == -1) {
            printf("[ERROR]: writing pipe\n");
            return 4;
        }

        close(fd[1]);
    }
    else { //Parent proc
        close(fd[1]);
        char str[200];
        int n;
        if (read(fd[0], &n, sizeof(int)) == -1) {
            printf("[ERROR]: reading string size\n");
            return 4;
        }

        if (read(fd[0], str, sizeof(char) * n) == -1) {
            printf("[ERROR]: reading pipe\n");
            return 5;
        }
        printf("Received: %s\n", str);
        close(fd[0]);
        wait(NULL);
    }
    
    return 0;
}
```

<br>

## Short introduction to signals in C

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        printf("[ERROR]: with fork\n");
        return 1;
    }

    if (pid == 0) { //Child
        while (1) {
            printf("Some text goes here\n");
            usleep(50000);
        }
    }
    else { //Parent
        sleep(1);
        kill(pid, SIGKILL);

        wait(NULL);
    }

    return 0;
}
```

<br>

## Stopping and continuing the execution of a process (using SIGCONT and SIGSTOP)

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (pid == 0) {
        while (1) {
            printf("Valami\n");
            usleep(50000);
        }
    }
    else {
        kill(pid, SIGSTOP);
        int t;
        do {
            printf("Time in seconds for execution: ");
            scanf("%d", &t);

            if (t > 0) {
                kill(pid, SIGCONT);
                sleep(t);
                kill(pid, SIGSTOP);
            }

        } while (t > 0);

        kill(pid, SIGKILL);
        wait(NULL);
    }
    
    return 0;
}
```

## Handling signals

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handle_sigstp(int sig) {
    printf("Stop not allowed\n");
}

int main(int argc, char* argv[]) {
    struct sigaction sa;
    sa.sa_handler = &handle_sigstp;
    sa.sa_flags = SA_RESTART;

    sigaction(SIGTSTP, &sa, NULL);

    int x;
    printf("Input a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);

    return 0;
}
```

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handle_sigtstp(int sig) {
    printf("Stop not allowed\n");
}

int main(int argc, char* argv[]) {
    //struct sigaction sa;
    //sa.sa_handler = &handle_sigstp;
    //sa.sa_flags = SA_RESTART;
    //sigaction(SIGTSTP, &sa, NULL);

    signal(SIGTSTP, &handle_sigtstp);

    int x;
    printf("Input a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);

    return 0;
}
```

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handle_sigtstp(int sig) {
    printf("Stop not allowed\n");
}

void handle_sigcont(int sig) {
    printf("Input number: \n");
    fflush(stdout);
}

int main(int argc, char* argv[]) {
    //signal(SIGTSTP, &handle_sigtstp);
    signal(SIGCONT, &handle_sigcont);

    int x;
    printf("Input a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);

    return 0;
}
```

## Communicating between processes using signals

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int x = 0;

void handle_sigusr1() {
    if (x == 0) {
        printf("\n(HINT) Its 15\n"); // Not recommended to do 
        // basically anything in signal handler
    }
}

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (pid == 0) { //Child
        sleep(5);
        kill(getppid(), SIGUSR1);
    }
    else { //Parent
        signal(SIGUSR1, &handle_sigusr1);
        printf("What is the result of 3 x 5: ");
        scanf("%d", &x);
        if (x == 15) {
            printf("Right!\n");
        }
        else {
            printf("Wrong!\n");
        }

        wait(NULL);
    }

    return 0;
}
```

<br>

## How to execute another program in C (using exec)

```cs

```

<br>

## What is waitpid()?

Waits for a specific child to finnish execution.

```cs
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    pid_t pid1 = fork();
    if (pid1 == -1) {
        printf("[ERROR]: creating process\n");
        return 1;
    }
    if (pid1 == 0) {
        sleep(4);
        printf("Finished execution (%d)\n", getpid());
        return 0;
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        printf("[ERRO] creating 2nd process\n");
        return 2;
    }
    if (pid2 == 0) {
        sleep(1);
        printf("Finished execution (%d)\n", getpid());
        return 0;
    }

    pid_t pid1_res = waitpid(pid1, NULL, 0);
    printf("Waited for %d\n", pid1_res);
    pid_t pid2_res = waitpid(pid2, NULL, 0);
    printf("Waited for %d\n", pid2_res);

    return 0;
}
```

<br>
<br>
<br>
<br>
<br>

<br>
<br>
<br>
<br>
<br>

<br>
<br>
<br>
<br>
<br>


# Feladatok

## Hasznos függvények

```cs

/***************** Üzenet *******************/
struct uzenet { 
    long mtype;
    char mtext [ 1024 ]; 
};

/*void handler(int sign){
    printf("%d folyamat %d jelzést kapott.\n",getpid(), sign);
}*/

int getRandomNumber(int start, int end) {
    //srand(time(NULL));
    return rand () % (end - start) + start;
}

int kuld( int uzenetsor , char message[]) 
{ 
    struct uzenet uz;
    uz.mtype = 5,
    strcpy(uz.mtext, message);
    int status; 
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    return 0; 
}

int fogad( int uzenetsor ) 
{ 
    struct uzenet uz; 
    int status; 
    status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    else
        printf( "%d folyamat %ld kódú üzenetet kapott: %s\n",getpid(), uz.mtype, uz.mtext ); 
    return 0; 
} 

/*****************  Szemafor *******************/
#define MEMSIZE 1024

sem_t *szemafor_letrehozas(char *nev, int szemafor_ertek)
{
	sem_t *semid = sem_open(nev, O_CREAT, S_IRUSR | S_IWUSR, szemafor_ertek);
	if (semid == SEM_FAILED)
		perror("sem_open");

	return semid;
}

void szemafor_torles(char *nev)
{
	sem_unlink(nev);
}



```

## Példa üzenetsor használatára Signállal

```cs
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "functions.h"

int h = 0;
void handler(int signum) {
    h++;
}

/**
 *  Task: Van 2 folyamat, parent küldjön üzenetet a childnek
 *  child válaszoljon vissza neki
 * 
**/
int main(int argc, char* argv[]) {
    signal(SIGUSR1, handler);

    key_t kulcs = ftok(argv[0], 1);
    printf("A kulcs: %d\n", kulcs);
    int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        perror("msgget");
        return 1;
    }

    pid_t child = fork();
    if (child == 0) { //Child
        fogad(uzenetsor);
        kuld(uzenetsor, "Szia neked is Parnet process!");
        kill(getppid(), SIGUSR1);
        while (h == 0) { usleep(20); }
        h = 0;
        fogad(uzenetsor);
        kuld(uzenetsor, "Köszönöm jól!");
        kill(getppid(), SIGUSR1);
        while (h == 0 ) { usleep(20); }
        fogad(uzenetsor);
        fogad(uzenetsor);
        kuld(uzenetsor, "Köszönöm szépen! Neked is további szép napot Parent process!");
        kill(getppid(), SIGUSR1);
    }
    else { //Parent
        kuld(uzenetsor, "Szia child process!");
        while (h == 0) { usleep(20); }
        h = 0;
        fogad(uzenetsor);
        kuld(uzenetsor, "Hogy vagyunk ma Child process?");
        kill(child, SIGUSR1);
        while (h == 0) { usleep(20); }
        h = 0;
        fogad(uzenetsor);
        kuld(uzenetsor, "Ennek bizony örülök!");
        kuld(uzenetsor, "További szép napot neked Child process!");
        kill(child, SIGUSR1);
        while (h == 0) { usleep(20); }
        fogad(uzenetsor);

        /*******************Fontos****************/
        waitpid(child, NULL, 0);
        int status = msgctl(uzenetsor, IPC_RMID, NULL);
        if (status < 0) {
            printf("[ERROR]: parent - msgctl\n");
            return 2;
        }
        /*******************************************/
    }

    return 0;
}
```

## Példa oszotott memória használatára Signállal

```cs
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "functions.h"

int h = 0;
void handler(int signum) {
    h++;
}

int main(int argc, char* argv[]) { // gcc -pthread [...]
    signal(SIGUSR1, handler);

    /*********Osztott memória létrehozása*************/
    key_t kulcs = ftok(argv[0], 1); //kulcs generálás
    int oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR); //osztott memória létrehozása
    char* s = shmat(oszt_mem_id, NULL, 0); //csatlakozás az osztott memóriához
    /************************************************/

    pid_t child = fork();
    if (child > 0) { //Parent
        strcpy(s, "Szia Child Process!"); //beírunk a memóriába
        //shmdt(s); //lecsatlakozunk a memóriáról, nem kell már az adat
        kill(child, SIGUSR1); //szólunk, hogy beírtuk a memóriába az adatot
        
        while (h == 0) { usleep(20); }
        h = 0;
        printf("A parnet ezt olvasta ki: %s\n", s);
        strcpy(s, "Hogy vagyunk ma Child process?");
        kill(child, SIGUSR1);

        while (h == 0) { usleep(20); }
        printf("A parnet ezt olvasta ki: %s\n", s);
        strcpy(s, "Ennek igazán örülök!");
        kill(child, SIGUSR1);

        shmdt(s); //lecsatlakozunk a memóriáról
        waitpid(child, NULL, 0);
        shmctl(oszt_mem_id, IPC_RMID, NULL); //töröljük az osztott memóriát
    }
    else { //Child
        while (h == 0) { usleep(20); } //várunk még a parent nem szól, hogy beírta a memóba az adatot
        h = 0;
        printf("A gyerek ezt olvasta a memóriából: %s\n", s);
        //shmdt(s); //lecsatlakozunk a memóriáról, nem kell már az adat
        strcpy(s, "Szia Parent processz!");
        kill(getppid(), SIGUSR1);

        while (h == 0) { usleep(20); }
        h = 0;
        printf("A gyerek ezt olvasta a memóriából: %s\n", s);
        strcpy(s, "Köszönöm, én jól vagyok!");
        kill(getppid(), SIGUSR1);

        while (h == 0) { usleep(20); }
        printf("A gyerek ezt olvasta a memóriából: %s\n", s);

        shmdt(s); //lecsatlakozunk a memóriáról
    }
    
    return 0;
}
```

## Osztott memória szemaforral

```cs
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MEMSIZE 1024

sem_t *szemafor_letrehozas(char *nev, int szemafor_ertek)
{
	sem_t *semid = sem_open(nev, O_CREAT, S_IRUSR | S_IWUSR, szemafor_ertek);
	if (semid == SEM_FAILED)
		perror("sem_open");

	return semid;
}

void szemafor_torles(char *nev)
{
	sem_unlink(nev);
}

int main(int argc, char *argv[]) // gcc -pthread [...]
{
	char *sem_nev = "/w4e4v5";

	key_t kulcs = ftok(argv[0], 1);
	int sh_mem_id = shmget(kulcs, MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
	char* s = shmat(sh_mem_id, NULL, 0);

	sem_t* semid = szemafor_letrehozas(sem_nev, 0);

	pid_t child = fork();
	if (child > 0) // parent
	{
		char buffer[] = "Hajra Fradi!\n";
		printf("Szulo indul, kozos memoriaba irja: %s\n", buffer);
		sleep(4);		   // wait for a few seconds
		strcpy(s, buffer); // copy data to shared memory
		printf("Szulo, szemafor up!\n");
		sem_post(semid); // semaphore up
		shmdt(s);		 // release shared memory
		wait(NULL);
		szemafor_torles(sem_nev);
		shmctl(sh_mem_id, IPC_RMID, NULL);
	}
	else if (child == 0) //child
	{
		/*  kritikus szakasz kezdete   */
		printf("Gyerek: Indul a posix szemafor down!\n");
		sem_wait(semid); // semaphore down
		printf("Gyerek, down rendben, eredmeny: %s", s);
		sem_post(semid);
		/*  kritikus szakasz vége  */
		shmdt(s);
	}

	return 0;
}
```

## Zh feladat web

```cs
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "functions.h"

int h = 0;
void handler(int sign){
    //printf("[Signal1]>>>>>>>>>: %d folyamat %d jelzést kapott.\n",getpid(), sign);
    h++;
}

int h2 = 0;
void handler2(int sign){
    //printf("[Signal2]>>>>>>>>>: %d folyamat %d jelzést kapott.\n",getpid(), sign);
    h2++;
}

int main(int argc, char* argv[]) {
    int lista_size = 5;
    const char lista[5][32] = {
        "Kifli",
        "Bambi",
        "Zsömle",
        "Kenyér",
        "Cukor"
    };

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler2);
    int p1[2]; //parent -> child1
    int p2[2]; //parent -> child2

    key_t kulcs = ftok(argv[0], 1);
    //printf("A kulcs: %d\n", kulcs);
    int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        printf("[ERROR]: üzenetsor létrehozás\n");
        return 1;
    }

    int oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR); //osztott memória létrehozása
    char* s = shmat(oszt_mem_id, NULL, 0); //csatlakozás az osztott memóriához

    char* sem_nev1 = "/w4e4v5_sem1";
    sem_t* semid1 = szemafor_letrehozas(sem_nev1, 0); //szemafor létrehozás
    
    char* sem_nev2 = "/w4e4v5_sem2";
    sem_t* semid2 = szemafor_letrehozas(sem_nev2, 0); //szemafor létrehozás
    
    char* sem_nev3 = "/w4e4v5_sem3";
    sem_t* semid3 = szemafor_letrehozas(sem_nev3, 0); //szemafor létrehozás

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        printf("[ERROR]: opening pipes\n");
        return 1;
    }
    
    pid_t child1 = fork();
    pid_t child2;
    if (child1 == 0) { //Child1
        close(p2[0]); close(p2[1]); close(p1[1]);
        //printf("[Child1]: pid számom = %d\n", getpid());
        printf("[Child1]: Bemehetek a boltba?\n");
        kill(getppid(), SIGUSR1);
        printf("[Child1]: Jelzést küldtem\n");
        char buffer1[256];
        if (read(p1[0], buffer1, sizeof(buffer1)) == -1) {
            printf("[ERROR]: Reading p1 pipe\n");
            return 1;
        }
        printf("[Child1]: ezt olvasta csőből: %s\n", buffer1);

        fogad(uzenetsor);
        printf("[Child1]: viszont kívánom\n");

        srand(getpid());
        int randnum = rand() % 5;
        printf("[Child1]: Válaszott áru= %s\n", lista[randnum]);
        strcpy(s, lista[randnum]);
        printf("[Child1]: beírta az árut a memóriába\n");
        sem_post(semid1);

        close(p1[0]);
        shmdt(s);
        printf("Child1 végzett\n");
    }
    else {
        child2 = fork();
        if (child2 == 0) { //Child2
            close(p1[0]); close(p1[1]); close(p2[1]);
            //printf("[Child2]: pid számom = %d\n", getpid());
            printf("[Child2]: Bemehetek a boltba?\n");
            kill(getppid(), SIGUSR2);
            printf("[Child2]: Jelzést küldtem\n");
            char buffer2[256];
            if (read(p2[0], buffer2, sizeof(buffer2)) == -1) {
                printf("[ERROR]: Reading p2 pipe\n");
                return 1;
            }
            printf("[Child2]: ezt olvasta csőből: %s\n", buffer2);

            fogad(uzenetsor);
            printf("[Child2]: viszont kívánom\n");

            srand(getpid());
            int randnum = rand() % 5;
            
            sem_wait(semid2);
            printf("[Child2]: Válaszott áru= %s\n", lista[randnum]);
            strcpy(s, lista[randnum]);
            printf("[Child2]: beleírt az memóriába\n");
            sem_post(semid2);
            sem_post(semid3);

            close(p2[0]);
            shmdt(s);
            printf("Child2 végzett\n");
        }
        else { //Parent
            close(p1[0]); close(p2[0]); //Csövek lezárása
            while (h == 0 || h2 == 0) { usleep(20); }
            h = 0; h2 = 0;
            printf("[Parent]: Megkapta a jelzéseket\n");

            char buffer[256];
            strcpy(buffer, "Gyere be Child1");
            if (write(p1[1], buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: Writing p1 pipe\n");
                return 2;
            }
            printf("[Parent]: küldtem csövön Child1-nek\n");

            strcpy(buffer, "Gyere be Child2!");
            if (write(p2[1],  buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: Writing p2 pipe\n");
                return 3;
            }
            printf("[Parent]: küldtem csövön Child2-nek\n");

            kuld(uzenetsor, "Üdvözöllek!");
            kuld(uzenetsor, "Üdvözöllek!");

            

            sem_wait(semid1);
            printf("[Parent]: Child1 gyerek terméke: %s\n", s);
            sem_post(semid1);
            
            sem_post(semid2);

            sem_wait(semid3);
            printf("[Parent]: Child2 gyerek terméke: %s\n", s);
            sem_post(semid3);
            
            shmdt(s);
            close(p1[1]); close(p2[1]); //Csövek lezárása
            waitpid(child1, NULL, 0);
            waitpid(child2, NULL, 0);
            int status = msgctl(uzenetsor, IPC_RMID, NULL);
            if (status < 0) {
                printf("[ERROR]: parent - msgctl\n");
                return 2;
            }
            shmctl(oszt_mem_id, IPC_RMID, NULL); //töröljük az osztott memóriát
            szemafor_torles(sem_nev1); //szemafor törlés
            szemafor_torles(sem_nev2); //szemafor törlés
            szemafor_torles(sem_nev3); //szemafor törlés
            printf("Parent végzett\n");
        }
    }
    
    return 0;
}
```

## zh feladat 05.10

```cs
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "functions.h"

int h1 = 0;
void handler1(int signum) {
    h1++;
}

int h2 = 0;
void handler2(int signum) {
    h2++;
}

int main(int argc, char* argv[]) {
    //Megoldott feladatok: 1. 2.

    int db = 0;
    if (argc < 2) {
        db = 5;
    }
    else {
        db = atoi(argv[1]);
        if (db < 1) {
            db = 5;
        }
        printf("Db: %d\n", db);
    }

    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);
    int p1[2]; //parent -> child1;
    int p2[2]; //parent -> child2
    
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        printf("[ERROR]: creating pipes\n");
        return 1;
    }

    key_t kulcs = ftok(argv[0], 1);
    //printf("A kulcs: %d\n", kulcs);
    int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        printf("[ERROR]: üzenetsor létrehozás\n");
        return 1;
    }
    
    pid_t child1 = fork();
    pid_t child2;
    if (child1 == 0) { //Child1
        close(p1[1]); close(p2[0]); close(p2[1]);
        kill(getppid(), SIGUSR1);
        
        int db = 0;
        read(p1[0], &db, sizeof(int));
        close(p1[0]);
        printf("[Child1]: páciensek száma= %d\n", db);

        srand(getpid() + time(NULL));
        int betegek = 0;
        for (int i = 0; i < db; i++) {
            if (rand() % 100 <= 80) { betegek++; }
        }

        char str[5];
        sprintf(str, "%d", betegek);
        kuld(uzenetsor, str);

        printf("Child1 végzett\n");
    }
    else { 
        child2 = fork();
        if (child2 == 0) { //Child2
            close(p2[1]); close(p1[0]); close(p1[1]);
            kill(getppid(), SIGUSR2);

            int db = 0;
            read(p2[0], &db, sizeof(int));
            close(p2[0]);
            printf("[Child2]: páciensek száma = %d\n", db);

            srand(getpid() + time(NULL));
            int betegek = 0;
            for (int i = 0; i < db; i++) {
                if (rand() % 100 <= 20) { betegek++; }
            }

            char str[5];
            sprintf(str, "%d", betegek);
            kuld(uzenetsor, str);

            printf("Child2 végzett\n");
        }
        else { //Parent
            close(p1[0]); close(p2[0]);
            while (h1 == 0 || h2 == 0) { usleep(20); }
            h1 = 0; h2 = 0;
            printf("[Parent]: Oltás elkezdődik!\n");

            int child1_db = db / 2;
            int child2_db = db / 2;
            if (db % 2 == 1) {
                child2_db = db / 2 + 1;
            }
            write(p1[1], &child1_db, sizeof(int));
            write(p2[1], &child2_db, sizeof(int));
            close(p1[1]); close(p2[1]);

            int child1_betegek = fogad(uzenetsor);
            printf("[Parent]: Child1 betegek száma = %d\n", child1_betegek);
            int child2_betegek = fogad(uzenetsor);
            printf("[Parent]: Child2 betegek száma = %d\n", child2_betegek);

            
            FILE* file = fopen("betegek.txt", "w");
            if (file == NULL) {
                printf("\n[ERROR]: Writing the file failed\n");
                return 2;
            }

            const char* DB_FORMAT_OUT = "%d\n";
            fseek(file, 0, SEEK_SET);
            fprintf(file, DB_FORMAT_OUT, child1_betegek);
            fprintf(file, DB_FORMAT_OUT, child2_betegek);
            

            fclose(file);

            waitpid(child1, NULL, 0);
            waitpid(child2, NULL, 0);
            int status = msgctl(uzenetsor, IPC_RMID, NULL);
            if (status < 0) {
                printf("[ERROR]: parent - msgctl\n");
                return 2;
            }
            printf("Parent végzett\n");
        }
    }

    return 0;
}
```

## zh feladat 05.27

```cs
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "functions.h"

int h = 0;
void handler(int sign){
    //printf("[Signal1]>>>>>>>>>: %d folyamat %d jelzést kapott.\n",getpid(), sign);
    h++;
}

int h2 = 0;
void handler2(int sign){
    //printf("[Signal2]>>>>>>>>>: %d folyamat %d jelzést kapott.\n",getpid(), sign);
    h2++;
}

int main(int argc, char* argv[]) {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler2);

    key_t kulcs = ftok(argv[0], 1);
    //printf("A kulcs: %d\n", kulcs);
    int uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        printf("[ERROR]: üzenetsor létrehozás\n");
        return 1;
    }

    //int oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR); //osztott memória létrehozása
    //char* s = shmat(oszt_mem_id, NULL, 0); //csatlakozás az osztott memóriához

    char* sem_nev1 = "/w4e4v5_sem1";
    sem_t* semid1 = szemafor_letrehozas(sem_nev1, 0); //szemafor létrehozás
    
    char* sem_nev2 = "/w4e4v5_sem2";
    sem_t* semid2 = szemafor_letrehozas(sem_nev2, 0); //szemafor létrehozás
    
    char* sem_nev3 = "/w4e4v5_sem3";
    sem_t* semid3 = szemafor_letrehozas(sem_nev3, 0); //szemafor létrehozás
    
    int p1[2]; //child1 -> parent
    int p2[2]; //child2-> parent
    int p3[2]; //parent -> child1
    if (pipe(p1) < 0 || pipe(p2) || pipe(p3) < 0) {
        printf("[ERROR]: opening pipes\n");
        return 1;
    }
    
    pid_t child1 = fork();
    pid_t child2;
    if (child1 == 0) { //Child1 - Bütyök
        close(p1[0]); close(p2[0]); close(p2[1]); close(p3[1]);
        //printf("[Child1]: pid számom = %d\n", getpid());
        kill(getppid(), SIGUSR1);

        int fa_db = fogad(uzenetsor);
        printf("[Child1]: %d db fát kell gyűjtenem\n", fa_db);
        /**** 1. Feladat vége ****/

        char buffer[256];
        strcpy(buffer, "Vizes a fa!");
        write(p1[1], buffer, sizeof(buffer));
        /**** 2. feladat vége ****/

        if (read(p3[0], buffer, sizeof(buffer)) == -1) {
            printf("[ERROR]: child1 -  reading pipe3\n");
        }
        printf("[Child1]: parent ezt küldte csövön: %s\n", buffer);
        fflush(stdout);
        /**** 3. Feladat vége ***/

        /**** Takarítás ****/
        //shmdt(s);
        close(p1[1]);
        close(p3[1]);
        //printf("Child1 végzett\n");
    }
    else {
        child2 = fork();
        if (child2 == 0) { //Child2 - Tutajos
            close(p1[0]); close(p1[1]); close(p2[0]); close(p3[0]); close(p3[1]);
            kill(getppid(), SIGUSR2);

            while (h2 == 0) { usleep(20); }
            int hal_db = fogad(uzenetsor);
            printf("[Child2]: %d db halat kell fognom\n", hal_db);
            /**** 1. Feladat vége ****/

            srand(time(NULL) + getpid());
            int fogott_halak = rand() % 3;
            char buffer[256] = "A csuka megfogott!";
            write(p2[1], buffer, sizeof(buffer));
            strcpy(buffer, "Segítség!");
            write(p2[1], buffer, sizeof(buffer));
            /**** 2. feladat vége ****/

            /**** Takarítás ****/
            //shmdt(s);
            close(p2[1]);
            //printf("Child2 végzett\n");
        }
        else { //Parent - Matula
            close(p1[1]); close(p2[1]); close(p3[0]);//Csövek lezárása
            while (h == 0 || h2 == 0) { usleep(20); }
            h = 0; h2 = 0;
            printf("[Parent]: Megérkeztek a jelzések\n");
            
            kuld(uzenetsor, "2");
            usleep(20);
            kill(child2, SIGUSR2);
            kuld(uzenetsor, "3");
            /**** 1. Feladat vége ****/

            char buffer[256];
            if (read(p1[0], buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: parent - reading pipe1\n");
            }
            printf("[Parent]: Child1 ezt küldte csövön: %s\n", buffer);
            if (read(p2[0], buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: parent - reading pipe1\n");
            }
            printf("[Parent]: Child2 ezt küldte csövön: %s\n", buffer);
            if (read(p2[0], buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: parent - reading pipe1\n");
            }
            printf("[Parent]: Child2 ezt küldte csövön: %s\n", buffer);
            /**** 2. feladat vége ****/

            strcpy(buffer, "Hadja a fizes fát, menjen Tutajosnak segíteni!");
            if (write(p3[1], buffer, sizeof(buffer)) == -1) {
                printf("[ERROR]: parent - writing pipe3\n");
                return 1;
            }
            /**** 3. Feladat vége ***/

            /**** Takarítás ****/
            close(p1[0]); close(p2[0]); close(p3[1]);
            //shmdt(s);
            waitpid(child1, NULL, 0);
            waitpid(child2, NULL, 0);
            int status = msgctl(uzenetsor, IPC_RMID, NULL);
            if (status < 0) {
                printf("[ERROR]: parent - msgctl\n");
                return 2;
            }
            //shmctl(oszt_mem_id, IPC_RMID, NULL); //töröljük az osztott memóriát
            szemafor_torles(sem_nev1); //szemafor törlés
            szemafor_torles(sem_nev2); //szemafor törlés
            szemafor_torles(sem_nev3); //szemafor törlés
            printf("Parent végzett\n");    
        }
    }
    
    return 0;
}


```

<br>
<br>
<br>
<br>
