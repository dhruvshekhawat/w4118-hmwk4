#include <stdio.h>
#include <unistd.h>
#include <syscall.h>

int main(int argc, char **argv)
{
	int m, n;
	int ret;

	if (argc == 3) {
		n = atoi(argv[1]);
		m = atoi(argv[2]);
	} else {
		printf("Usage: %s <num_cores> <group>\n", *argv);
		return -1;
	}

	ret = syscall(378, n, m);
	printf("status = %d\n", ret);

	return 0;
}
