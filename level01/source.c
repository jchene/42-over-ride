#include <string.h>
#include <stdio.h>

char username[100];

int verify_user_name(char *username)
{
	puts("verifying username....\n");
	return memcmp(username, "dat_wil", 7) != 0;
}

int verify_user_pass(char *password)
{
	return memcmp(password, "admin", 5) != 0;
}

int main(void)
{
	char password[64];

	memset(password, 0, sizeof(password));
	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(username, 256, 0);
	if (verify_user_name(username))
	{
		puts("nope, incorrect username...\n");
	}
	else
	{
		puts("Enter Password: ");
		fgets(password, 100, 0);
		verify_user_pass(password);
		puts("nope, incorrect password...\n");
	}
	return 1;
}