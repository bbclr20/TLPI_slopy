#include<pwd.h>
#include<grp.h>
#include<ctype.h>
#include<stdlib.h>
#include<stdio.h>

char* userNameFromId(uid_t uid) {
    struct passwd *pwd;
    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name; 
}

uid_t userIdFromName(const char *name) {
    struct passwd *pwd;
    char *endptr;

    if (name == NULL || *name == '\0') {
        return -1;
    }
    
    // uid_t u;
    // u = strtol(name, &endptr, 10);
    // if (*endptr == '\0') {
    //     return u;
    // }

    pwd = getpwnam(name);
    if (pwd == NULL) {
        return -1;
    }
    return pwd->pw_uid;
}

char * groupNameFromId(gid_t gid) {
    struct group *grp;
    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

int main(int argc, char* argv[]) {
    char *userName = argv[1];
    uid_t userId = userIdFromName(userName);
    printf("User ID: %d\n", userId);

    userName = userNameFromId(userId);
    printf("User name: %s\n", userName);

    char *groupName = groupNameFromId(userId);
    printf("Group name: %s\n", groupName); 
    return 0;
}