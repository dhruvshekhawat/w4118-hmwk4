#include <stdio.h>
#include <syscalls.h>

int main(int argc, char **argv)
{
	int ret;

	ret = syscall(378, 5, 5);
	printf("%d\n", ret);

	return 0;
}
