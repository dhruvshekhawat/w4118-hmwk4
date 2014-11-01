#include <stdio.h>
#include <unistd.h>
#include <syscall.h>

int main(int argc, char **argv)
{
	int ret;

	ret = syscall(378, 1, 1);
	printf("%d\n", ret);

	return 0;
}
