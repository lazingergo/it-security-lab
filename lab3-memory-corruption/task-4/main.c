#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char string[100];

void exec_string() {
    system(string);
}

void add_bin(int magic) {
    if (magic == 0xdeadbeef) {
        strcat(string, "/bin");
    }
}

void add_sh(int magic1, int magic2) {
    if (magic1 == 0xcafebabe && magic2 == 0x0badf00d) {
        strcat(string, "/sh");
    }
}

void vulnerable_function(char* string) {
    char buffer[8];
    strcpy(buffer, string);
}

int main(int argc, char** argv) {

    if (argc != 2) {
	   printf("Please specify one argument!\n");
	   return 0;
    }

    string[0] = 0;

    vulnerable_function(argv[1]);
    
    return 0;
}
