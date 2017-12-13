nclude <unistd.h>
#include <stdio.h>
#include <string.h>
int main() {
    char hostname[100] = {0};
    int res = gethostname(hostname, sizeof(hostname));
    printf("Old host name: %s. (Return %d)\n", hostname, res);
    strcpy(hostname, "bambooshark.ics.cs.cmu.edu");
    res = sethostname(hostname, strlen(hostname));
    gethostname(hostname, sizeof(hostname));
    printf("New host name: %s. (Return %d)\n", hostname, res);
    return 0;
}
