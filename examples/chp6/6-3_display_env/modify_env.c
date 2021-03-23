#include<stdlib.h>
#include<stdio.h>

int main(
    int argc, 
    char *argv[],
    char *envp[]
    )
{
    // clearenv(); /* Erase entire environment */
    
    for (int j = 1; j < argc; j++) {
        if (putenv(argv[j]) != 0) {
            return -1;
        }
    }

    if (setenv("GREET", "Hello world", 0) == -1) { 
        return -1;
    }
    
    char *env_ptr = getenv("GREET");
    printf("The value of GREET is %s\n", env_ptr);

    unsetenv("BYE");
        
    char **ep;
    for (ep=envp; *ep!=NULL; ep++) {
        printf("%s\n", *ep);
    }
    exit(EXIT_SUCCESS);
}