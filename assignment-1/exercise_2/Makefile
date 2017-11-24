CFLAGS += -shared -fpic

all: pam_quiz_roulette install

clean:

	rm -f *.so

install: pam_quiz_roulette.so

	sudo install -o root -g root $< /lib/security

uninstall:

	sudo rm -f /lib/security/pam_quiz_roulette.so

%: %.c

	$(CC) $(CFLAGS) -o $@.so $<