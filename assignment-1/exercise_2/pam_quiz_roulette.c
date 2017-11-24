#define PAM_SM_AUTH

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

void string_to_lowercase(char *input) {
	for ( ; *input; ++input) *input = tolower(*input);
}

bool question_1() {
	char answer[128];
	printf("%s", "What is the answer to life, the universe and everything? ");
	scanf("%s", &answer);
	string_to_lowercase(answer);
	return strcmp(answer, "42") == 0;
}

bool question_2() {
	char answer[128];
	printf("%s", "What is half man, half dog and is its own best friend? ");
	scanf("%s", &answer);
	string_to_lowercase(answer);
	return strcmp(answer, "mog") == 0;
}

bool question_3() {
	char answer[128];
	printf("%s", "What's the secret combination for the luggage? ");
	scanf("%s", &answer);
	string_to_lowercase(answer);
	return strcmp(answer, "12345") == 0;
}

bool question_4() {
	char answer[128];
	printf("%s", "What's the name of Doc Brown's best friend? ");
	scanf("%s", &answer);
	string_to_lowercase(answer);
	return strcmp(answer, "marty mcfly") == 0;
}

bool question_5() {
	char answer[128];
	printf("%s", "What's the name of the depressed robot in the book The Hitchhiker's Guide To The Galaxy? ");
	scanf("%s", &answer);
	string_to_lowercase(answer);
	return strcmp(answer, "marvin") == 0;
}

int pam_sm_authenticate(
	pam_handle_t *pamh
	, int flags
	, int argc
	, const char **argv
) {
	bool (*questions[])() = {
		&question_1
		, &question_2
		, &question_3
		, &question_4
		, &question_5
	};
	srand(time(NULL));
	if (questions[rand() % 5]()) {
		return PAM_SUCCESS;
	}
	return PAM_AUTH_ERR;
}

int pam_sm_setcred(
	pam_handle_t *pamh
	, int flags
	, int argc
	, const char **argv
) {
	return PAM_IGNORE;
}