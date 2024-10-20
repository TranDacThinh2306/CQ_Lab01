#include<kernel/types.h>
#include<user/user.h>

int
main(int argc,char* argv[])
{
    if (argc != 1)
    {
        fprintf(2,"Usage: pingpong\n");
        exit(1);
    }
    
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        // child
        char buf[1];
        read(p[0],buf,1);
        printf("%d: received %sing\n",getpid(),buf);
        write(p[1],"p",1);
        exit(0);

    }
    else if (getpid() > 0)
    {
        // parent
        write(p[1],"p",1);
        wait(0);
    
        char buf[1];
        read(p[0],buf,1);
        printf("%d: received %song\n",getpid(),buf);
        exit(0);
    }
}