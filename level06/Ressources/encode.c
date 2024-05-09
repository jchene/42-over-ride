#include <stdio.h>
#include <string.h>

int encode(char *login)
{
	int loginValue;
	
	loginValue = (login[3] ^ 0x1337) + 6221293;
	for (int i = 0; i < strnlen(login, 7); ++i)
	{
		if (login[i] <= 31)
				return 1;
		loginValue += (loginValue ^ (unsigned int)login[i]) % 1337;
	}
	return loginValue;
}

int main()
{
	char s[7] = "AAAAAA";
	printf("%d\n", encode(s));
}