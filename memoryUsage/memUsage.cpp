#include <bits/stdc++.h>
#include <unistd.h>

#define M 1000000
#define B 10000000000

struct FooB {
    char text[B];
};


int main(int argc, char* argv[])
{
    // Main
    std::cout << "In main, sleeping 10 seconds" << std::endl;
    sleep(10);

    // Malloc
    struct FooB* billionFoo;
    billionFoo = (struct FooB*) malloc(sizeof(struct FooB));
    if (billionFoo == nullptr) {
        std::cerr << "Malloc failure" << std::endl;
        exit(1);
    } else {
        std::cout << "Malloc success" << std::endl;
    }
    std::cout << "After malloc, sleeping 10 seconds" << std::endl;
    sleep(10);

    // Fill it.
    /* for (int ii = 0; ii < B; ii++) { */
    /*     billionFoo->text[ii] = 'A'; */
    /* } */
    /* std::cout << "After fill, sleeping 20 seconds" << std::endl; */
    /* sleep(20); */
    
    // Fork, checking for shared memory between pids.
    int numChildren = 100;
        
    pid_t pid = fork();
    if (pid > 0) {
        std::cout << "I am parent, my child: " << pid << std::endl;
        std::cout << "Parent sleeping for 60" << std::endl;
        for (int ii = 0; ii < numChildren; ii++) {
            pid_t pidn = fork();
            if (pidn == 0) {
                break;
            } else {
                std::cout << "PARENT forked kid: " << pidn << std::endl;
            }
        }
        sleep(60);
    }
    else if (pid == 0) {
        pid_t myPid = getpid();
        std::cout << "I am child: " << myPid << std::endl;
        std::cout << "Child sleeping for 20" << std::endl;
        sleep(20);

        std::cout << "CHILD: Writing one char" << std::endl;
        billionFoo->text[1] = 'T';
        std::cout << "CHILD: wrote one char - sleeping 20" << std::endl;
        sleep(20);

        std::cout << "CHILD: writing all chars" << std::endl;
        for (int ii = 0; ii < B; ii++) billionFoo->text[ii] = 'T';
        std::cout << "CHILD: done writing all chars, sleeping 20" << std::endl;
        sleep(20);
    } else {
        std::cerr << "Error forking" << std::endl;
        exit(1);
    }

    // free
    free(billionFoo);
    std::cout << "After free, sleeping 20 seconds" << std::endl;
    sleep(20);
    return 0;
}
