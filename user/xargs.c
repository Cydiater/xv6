#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char *xargv[MAXARG];

int readline(char *line) {
	int index = 0;
	char ch;
	while (read(0, &ch, 1) != 0) {
		if (ch == '\n') {
			line[index] = 0;
			return index;
		}
		line[index++] = ch;
	}
	return -1;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("usage: xargs <command>\n");
		exit(1);
	}

	char line[512];
	char *bin = argv[1];
	for (int i = 1; i < argc; i++)
		xargv[i - 1] = argv[i];

	while (1) {
		int length = readline(line), begin = 0, index_of_argv = argc - 1;
		if (length == -1) break;
		for (int i = 0; i <= length; i++) {
			if (index_of_argv == MAXARG) {
				fprintf(2, "xargs: too many arguments\n");
				exit(1);
			}
			if (line[i] == ' ' || line[i] == 0) {
				xargv[index_of_argv++] = line + begin;
				begin = i + 1;
			}
		}
		xargv[index_of_argv] = 0;
		//printf("bin = %s xargv[0] = %s\n", bin, xargv[0]);
		if (fork() == 0) {
			exec(bin, xargv);
		} else {
			wait((int *) 0);
		}
	}

	exit(0);
}
