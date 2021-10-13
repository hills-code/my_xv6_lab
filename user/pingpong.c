#include <kernel/types.h>
#include <user/user.h>

#define BUFFER_SIZE 25

int main(){
    int parent_fd[2], child_fd[2];
	char buf[16];
	pipe(parent_fd);
	pipe(child_fd);
	if(fork() == 0) {
		close(parent_fd[1]);
		read(parent_fd[0], buf, 4);
		buf[4] = '\0';
		printf("%d: received %s\n", getpid(), buf);
		close(child_fd[0]);
		write(child_fd[1], "pong", 4);
	} else {
		close(parent_fd[0]);
		write(parent_fd[1], "ping", 4);
		close(child_fd[1]);
		read(child_fd[0], buf, 4);
		buf[4] = '\0';
		printf("%d: received %s\n", getpid(), buf);
	}
	exit(0);
}