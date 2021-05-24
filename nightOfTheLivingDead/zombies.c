#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int makeZombie(pid_t *pid) {
    int childPid;

    childPid = fork();
    if (childPid > 0) {
        // in parent
        *pid = childPid;
        return 0;
    } else {
        if(childPid == 0) {
            // in child
            exit(0);
        } else {
            // error
            return 1;
        }
    }
}

int makeZombies(int numZombies, pid_t *pids) {

    for (int ii = 0; ii < numZombies; ii++) {

        int ret = makeZombie(&pids[ii]);

        if (ret != 0) {
            fprintf(stderr, "Failed to spawn Zombie[%d]\n", ii);
            return 1;
        }
    }
    return 0;
}

void usage() {
    printf(
            "Purpose:\n"
            " Spawn zombies for a number of seconds\n\n"
            "Usage:\n"
            " ./zombies <num_zombies> <num_seconds>\n\n"
          );
}

int main(int argc, char *argv[]) {
    // handle args
    if (argc != 3) {
        usage();
        fprintf(stderr, "Incorrect number of args.\n");
        exit(1);
    }

    int numZombies = atoi(argv[1]);
    int numSeconds = atoi(argv[2]);

    if (numZombies == 0 || numSeconds == 0) {
        usage();
        fprintf(stderr, "Neither argument can be 0 or non integer\n");
        exit(1);
    }

    // Information
    printf("Will spawn %d zombies for %d seconds\n", numZombies, numSeconds);

    // Alloc memory for return pid list
    pid_t *pids = (pid_t *) malloc(numZombies * sizeof(pid_t));
    if (!pids) {
        fprintf(stderr, "Failed to malloc pids memory\n");
        exit(1);
    }

    // Make zombies
    int ret = makeZombies(numZombies, pids);
    if (ret == 0) {
        printf("Zombie List:\n");
        for (int ii = 0; ii < numZombies; ii++) {
            printf("Zombie Pid: %d\n", pids[ii]);
        }
    } else {
        fprintf(stderr, "Failed to spawn zombies\n");
    }

    free(pids);

    // Sleep - which really is what makes the zombies.
    printf("Sleeping %d seconds making our children zombies\n", numSeconds);

    sleep(numSeconds);
    return ret;
}

