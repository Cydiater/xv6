#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int foo(int x) {
	return x + 3;
}

int bar(int x) {
	return foo(x * 2);
}

int main() {
	for (int i = 0; i < 5; i++) {
		printf("%d\n", bar(i));
	}

	exit(0);
}
