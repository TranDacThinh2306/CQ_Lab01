//
//  xargs_demo
//
//  Created by NGUYỄN THỊ ANH THI on 15/10/2024.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    int start_arg_idx = 1;
    
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        start_arg_idx = 3;
    }

    int m_argc = 0;
    char *m_argv[MAXARG];

    for (int i = start_arg_idx; i < argc; i++) {
        m_argv[m_argc] = argv[i];
        m_argc++;
    }

    char buf;
    char temp_arg[256];
    int i = 0;

    while (read(0, &buf, 1)) {
        if (buf == '\n') {

            temp_arg[i] = 0;

            m_argv[m_argc] = temp_arg;
            m_argv[m_argc + 1] = 0;

            int pid = fork();
            if (pid < 0) {
                printf("Error starting child!\n");
                exit(1);
            } else if (pid == 0) {

                exec(m_argv[0], m_argv);
                exit(0);
            } else {
             
                wait(0);
                i = 0;
            }
        } else {
            temp_arg[i] = buf;
            i++;
        }
    }
    
    exit(0);
}

