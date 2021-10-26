#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0 || (strcmp("..", de.name) == 0) || (strcmp(".", de.name) == 0))
            continue;

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_FILE && (strcmp(target, de.name) == 0)){
            // printf("%s\n", fmtname(buf));
            printf("%s\n", buf);
        }

        if(st.type == T_DIR){
            find(buf, target);
        }
    }

    close(fd);
}

int main(int argc, char *argv[])
{

      if(argc < 3){
        printf("usage: find path target\n");
        exit(0);
      }
    find(argv[1], argv[2]);

    exit(0);
}
