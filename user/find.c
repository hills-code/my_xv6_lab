#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path) {
    char *p;
    //找到“/”后面的第一个字符
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    return ++p;
}

void find(char *path, char *file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    fd = open(path, 0);

    fstat(fd, &st);

    switch(st.type){
        //如果文件类型是普通文件
        case T_FILE:
            //对比文件名是否符合，如果符合就输出路径
            if (strcmp(file_name, fmtname(path)) == 0) 
                printf("%s\n", path);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0) 
                    continue;
                //避免重复进入当前文件夹以及父文件夹
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
                    continue;
                //将文件名拼接到当前路径下
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                //递归查找
                find(buf, file_name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    find(argv[1], argv[2]); 
    exit(0);
}