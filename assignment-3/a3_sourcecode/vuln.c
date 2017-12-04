#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void helper() {
    asm("pop %rdi; pop %rsi; pop %rdx; ret");
}

int vuln() {
    char buf[150];
    ssize_t b;

    printf("Enter input: ");
    b = read(0, buf, 400);

    printf("Recv: ");
    write(1, buf, b);

    system("/bin/false");
}

int main(int argc, char *argv[]){
    setbuf(stdout, 0);
    vuln();
    return 0;
}
