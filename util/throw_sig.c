#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if (strcmp(argv[1], "sigterm") != 0) {
		if(kill(atoi(argv[2]), SIGTERM) != -1) {
			puts("nice");
		}
	} else if(strcmp(argv[1], "sigup") != 0) {
		if(kill(atoi(argv[2]), SIGHUP) != -1) {
			puts("nice");
		}
	} else {
		puts("wrong argument");
	}
	return 0;
}
