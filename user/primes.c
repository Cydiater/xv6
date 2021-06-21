#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
	int last_pipe[2], next_pipe[2] = {0}, is_end = 1;
	pipe(last_pipe);
	if (fork() != 0) {
		close(last_pipe[0]);
		for (int i = 2; i <= 35; i++)
			write(last_pipe[1], &i, sizeof i);
		close(last_pipe[1]);
		wait((int *) 0);
	} else {
		int prime = 2, next_prime;
		fprintf(1, "prime 2\n");
		close(last_pipe[1]);
		while (1) {
			int number;
			int cnt = read(last_pipe[0], &number, sizeof number);
			if (cnt == 0) {
				if (is_end == 0)
					close(next_pipe[1]);
				close(last_pipe[0]);
				wait((int *) 0);
				exit(0);
			}
			if (is_end == 0) {
				if (number % prime != 0)
					write(next_pipe[1], &number, sizeof number);
			} else {
				if (number % prime != 0) {
					next_prime = number;
					fprintf(1, "prime %d\n", number);
					pipe(next_pipe);
					if (fork() == 0) {
						last_pipe[0] = next_pipe[0];
						prime = next_prime;
						close(next_pipe[1]);
						is_end = 1;
					} else {
						close(next_pipe[0]);
						is_end = 0;
					}
				}
			}
		}
	}
	exit(0);
}
