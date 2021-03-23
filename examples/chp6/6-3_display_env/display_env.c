#include<stdio.h>

int main(
    int argc, 
    char *argv[], 
    char *envp[]
    ) 
{
    char **env_ptr;
    for(env_ptr=envp; *env_ptr!=NULL; env_ptr++) {
        printf("%s\n", *env_ptr);
    }

    return 0;
}