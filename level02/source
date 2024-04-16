#include <stdio.h>

int main(void)
{
	char password[96];
	char ptr[48];
	char username[96];
	int passwordSize;
	FILE *stream;

	memset(username, 0, sizeof(username));
	memset(ptr, 0, 41);
	memset(password, 0, sizeof(password));
	stream = 0;
	passwordSize = 0;

	stream = fopen("/home/users/level03/.pass", "r");
	if (!stream)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}

	passwordSize = fread(ptr, 1, 41, stream);
	ptr[strcspn(ptr, "\n")] = 0;
	if (passwordSize != 41)
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		exit(1);
	}
	fclose(stream);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");

	printf("--[ Username: ");
	fgets(username, 100, stdin);
	username[strcspn(username, "\n")] = 0;

	printf("--[ Password: ");
	fgets(password, 100, stdin);
	password[strcspn(password, "\n")] = 0;
	
	puts("*****************************************");

	if (strncmp(ptr, password, 41))
	{
		printf(username);
		puts(" does not have access!");
		exit(1);
	}

	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return 0;
}