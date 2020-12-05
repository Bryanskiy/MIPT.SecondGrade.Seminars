#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void GenerateString(int n, char* string) {
    int len = strlen(string);
    strcpy(string + len + 1, string);
    string[len] = n + 'a';
}

int main() {
    size_t num = 10;
    char* str = (char*)calloc(pow(2, num) - 1, sizeof(char));
    for(int i = 1; i < num; ++i) {
        GenerateString(i, str);
        printf("%s\n", str);
    }
}
