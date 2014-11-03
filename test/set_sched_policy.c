#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc, char *argv[] ) {

	int i, error;
	if ( argc != 2 ) {
		printf("USAGE: set_sched_policy <pid>\n");
		return 0;
	}
	i = atoi(argv[1]);

	if (i < 4) {
		 printf("set other pid than %d\n",i);
		 return 0;
	}

	struct sched_param param;
	param.sched_priority = 0;
	error = sched_setscheduler(i, 6, & param);
	if (error == -1)
		perror("sched");
//	printf("error: %d",error);
	i = 1;
	while (i < 32767) {
		printf("%d\n",i);
		i++;
	}
	return 0;

}
