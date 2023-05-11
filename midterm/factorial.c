#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n) {
    if (n == 0 || n == 1) {
	return 1;
    } else {
	return n * factorial(n-1);
    }
}

void call_factorial() {
    char buf[5]; // allocate space to read user input

    printf("Enter a number: ");
    fgets(buf, 5, stdin); // read input from the terminal

    int start;
    sscanf(buf, "%d", &start); // convert string to an integer

    int sum = factorial(start);
    printf("sum = %d\n", sum);
}

int main() {
    call_factorial();
}
