#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        
        while (1) {
            int prime;
            if (read(p[0], &prime, sizeof(prime)) == 0) {
                close(p[0]);
                exit(0);
            }

            printf("prime %d\n", prime);

            int next_p[2];
            pipe(next_p);

            if (fork() == 0) {
                close(next_p[1]);
                close(p[0]);
                p[0] = next_p[0]; 
            } else {
                close(next_p[0]);

                int num;
                while (read(p[0], &num, sizeof(num)) != 0) {
                    if (num % prime != 0) {
                        write(next_p[1], &num, sizeof(num));
                    }
                }

                close(p[0]);
                close(next_p[1]);
                wait(0);
                exit(0);
            }
        }
    } else {
        close(p[0]);

        for (int i = 2; i < 281; i++) {
            write(p[1], &i, sizeof(i));
        }

        close(p[1]);
        wait(0);
        exit(0);
    }
}
