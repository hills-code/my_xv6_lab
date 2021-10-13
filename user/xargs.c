#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char flatten[512], *p[MAXARG], ch;
    int flatten_length = 0, line_n;
    int p_length = 0, p_n;
    int i;
    for (i = 0; i < argc - 1; i++) {
        p[p_length++] = flatten + flatten_length;
        strcpy(flatten+flatten_length, argv[i+1]);
        flatten_length += strlen(argv[i+1]) + 1;
    }
    p_n = p_length; 
    p[p_n++] = flatten + flatten_length;
    line_n = flatten_length; 
    while (read(0, &ch, 1) > 0) {
        if (ch == '\n') {
            flatten[line_n++] = '\0'; 
            p[p_n++] = 0;
            if (fork() == 0) 
                exec(argv[1], p);
            else {
                wait(0); 
                line_n = flatten_length; 
                p_n = p_length; 
                p[p_n++] = flatten + line_n;
            }
        } else if (ch == ' ') {
            flatten[line_n++] = '\0'; 
            p[p_n++] = flatten + line_n;
        } else flatten[line_n++] = ch;
    }
    exit(0);
}