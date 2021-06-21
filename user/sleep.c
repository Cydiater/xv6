#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		const char *error_msg = "usage: sleep <number of ticks>\n";
		write(2, error_msg, sizeof error_msg);
		exit(1);
	}
	int tick_count = atoi(argv[1]);
	sleep(tick_count);
	exit(0);
}
