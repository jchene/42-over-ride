# LEVEL03 - REVERSE 2 - ENCRYPTED PASSWORD

We have a binary named level03 at /home/users/level03/

It is owned by `level04` and has a `setuid bit`

The program is divided in three functions: `main` `test` and `decrypt`

```c
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
```

The program starts by printing some line and reading an integer from `stdin` and then pass it with another number `322424845` to `test`

```c
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
```

`test` will substract the user input from `322424845` in this case

If the result is contained between 1 and 21 `decrypt` will be called with the result as argument, else `decrypt` will be called on a random integer.

```c
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
```

`decrypt` takes the string "Q}|u`sfg~sf{}|a3" and applies a binary XOR operation on each character of the string, then compares the result with "Congratulations!", if the string are identical the program launch "/bin/sh"

We have to find a number contained between 0 and 255 such that when XORed with every character of the string the string becomes "Congratulations!"

Let's start with the first letters:
```
C (ASCII) = 67 (dec) =	01000011 (bin)
Q (ASCII) = 81 (dec) =	01010001 (bin)
```

XOR Table:
```
0 XOR 0 = 0
1 XOR 0 = 1
0 XOR 1 = 1
1 XOR 1 = 0
```

We can now easily determine the number
```
	Q					C

	0	 XOR	0	=	0
	1	 XOR	0	=	1
	0	 XOR	0	=	0
	1	 XOR	1	=	0
	0	 XOR	0	=	0
	0	 XOR	0	=	0
	0	 XOR	1	=	1
	1	 XOR	0	=	1
```

The number is: 00010010 (bin) = 18 (dec)

Now we know we have to pass `18` to `decrypt` to get the shell to run

We can see in `test` that 18 is in the 1-21 range now we have to find the passcode that substracted from `322424845` gives `18`

The passcode is 322424845 - 18 = `322424827`

Let's try it
```
level03@OverRide:~$ (echo "322424827"; cat) | ./level03
***********************************
*               level03         **
***********************************
whoami
level04
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```

The flag is
```
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```


