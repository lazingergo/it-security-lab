#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* not_used = "/bin/sh";

void not_called() {
    printf("System call is even missing here...\n");
}

void vulnerable_function(char* string) {
    char buffer[8];
    strcpy(buffer, string);
}

int main(int argc, char** argv) {
    if (argc != 2) {
		printf("Please specify an argument!\n");
		return 0;
    }

    vulnerable_function(argv[1]);
    return 0;
}
