#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int decrypt(char codeDiff)
{
	char passphrase[29];

	passphrase[17] = '\0';
	strcpy(passphrase, "Q}|u`sfg~sf{}|a3");
	for (unsigned int i = 0; i < strlen(passphrase); ++i)
		passphrase[i] ^= codeDiff;
	if (!strcmp(passphrase, "Congratulations!"))
		return system("/bin/sh");
	else
		return puts("\nInvalid Password");
}

int test(int passcode, int secretCode)
{
	int result;

	switch (secretCode - passcode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		result = decrypt(secretCode - passcode);
		break;
	default:
		result = decrypt(rand());
		break;
	}
	return result;
}

int main(void)
{
	int passcode;

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &passcode);
	test(passcode, 322424845);
	return 0;
}