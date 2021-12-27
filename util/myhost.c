#include <unistd.h>
#include <stdio.h>

int main()
{
	char name[256];
	if(!gethostname(name, 256)) {
		printf("%s\n", name);
	}
	return 0;
}


