#include "kernel/types.h"
#include "user/user.h"

void func(int read_end) {
    int prime, num;
    int p[2]; 
    if (read(read_end, (void *)&prime, 4) != 0) {
        pipe(p);
        printf("prime %d\n", prime);
        if (fork() != 0) {
            close(p[0]);
            while (read(read_end, (char *)&num, 4) != 0) {
                if (num % prime != 0) 
                    write(p[1], (void *)&num, 4);
            }
            close(p[1]);  
            close(read_end); 
            wait(0);
        } else {
            close(p[1]); 
            func(p[0]);
        }
    } else close(read_end);
}
int main() {
    int i, p[2]; 
    pipe(p);
    if (fork() != 0) {
        close(p[0]);
        for (i = 2; i <= 35; i++) 
            write(p[1], (void *)&i, 4);
        close(p[1]);  
        wait(0);
    } else {
        close(p[1]); 
        func(p[0]);
    }
    exit(0);
}      