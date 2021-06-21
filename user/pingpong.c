#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
	int p2c[2], p2f[2], pid;
	char buf;
	pipe(p2c);
	pipe(p2f);
	if (fork() == 0) { // son
		pid = getpid();
		read(p2c[0], &buf, 1);
		fprintf(1, "%d: received ping\n", pid);
		write(p2f[1], &buf, 1);
	} else { // parent
		pid = getpid();
		write(p2c[1], &buf, 1);
		read(p2f[0], &buf, 1);
		fprintf(1, "%d: received pong\n", pid);
	}
	exit(0);
}
