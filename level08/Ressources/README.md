# LEVEL 08 - FORMAT STRING 3 - NO ARG CHECK

We have a binary named level08 at /home/users/level08/

It is owned by `level09`, has a `setuid bit` and hax `NX` disabled

This program is composed of 2 functions: `main` and `log_wrapper`

Let's start with the `main` munction, the program first define two `FILE` streams, an `int` to store a file descriptor, a buffer of 1 `char` and a buffer of 104 `char`

```c
int main(int argc, const char **argv)
{
	FILE *logFile;
	FILE *fileToBackup;
	int backupFd;
	char buf;
	char dest[104];
```

Then the program checks for arguments and print a message in case of an error which tells us the program is expecting a filename as first and only argument

```c
if (argc != 2)
	printf("Usage: %s filename\n", *argv);
```

Then it tries to open a log file located at `./backups/.log` in `write` mode

If it failed the program prints an error message and exits or else it calls the `log_wrapper` function with a debug message and the first argument as parameters

```c
	logFile = fopen("./backups/.log", "w");
	if (!logFile)
	{
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}

	log_wrapper(logFile, "Starting back up: ", argv[1]);
```

Next, the program tries to open the file to backup which is the filename passed as argument, prints an error and exits if the file could not be opened.

If no error was encountered the program proceeds to concatenate the filename with "./backups/" and tries to open this new file.

```c
	fileToBackup = fopen(argv[1], "r");
	if (!fileToBackup)
	{
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	strcpy(dest, "./backups/");
	strncat(dest, argv[1], 99 - strlen(dest));
	backupFd = open(dest, O_WRONLY | O_CREAT | O_EXCL, 00660);
	if (backupFd < 0)
	{
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}
```

Finally, the file is copied character by character using the `buf` defined at the top, the program calls the `log_wrapper` function and closes all remaining opened fd's before returning

```c
	buf = -1;
	while (1)
	{
		buf = fgetc(fileToBackup);
		if (buf == -1)
			break;
		write(backupFd, &buf, 1uLL);
	}

	log_wrapper(logFile, "Finished back up ", argv[1]);

	fclose(fileToBackup);
	close(backupFd);
	return 0;
}
```

Next, let's break down the `log_wrapper` function

This function is pretty straighforward, the arguments are in order:
- The `FILE` stream of the log file `./backups/.log`
- The debug message to write to the log file
- The filename passed as argument of the program

The function starts bu defining a buffer of 264 char and copies the debug message into it

```c
void log_wrapper(FILE *logFile, const char *msg, const char *filename)
{
	char dest[264];

	strcpy(dest, msg);
```

Then it uses `snprintf` to append the filename to the `dest` buffer and replaces the "\n" with a "\0" 

```c
	snprintf(&dest[strlen(dest)], 254 - strlen(dest), filename);
	dest[strcspn(dest, "\n")] = 0;
```

Finally it finishes by writing the `dest` buffer into the log file using `fprintf` before returning

```c
	fprintf(logFile, "LOG: %s\n", dest);
	return;
}
```

We can see a format string vulnerability in this use of `snprintf`

Following `snprintf`'s manual we can see it's arguments

```c
int snprintf(char *str, size_t size, const char *format, ...);
```

The thirs argument is the format string but in this case the string is the argument passed as parameter to the program without parsing

```c
//argv[1] passed without parsing from main function
log_wrapper(logFile, "Starting back up: ", argv[1]);
```

```c
//filename is argv[1]
void log_wrapper(FILE *logFile, const char *msg, const char *filename)
```

```c
//filename is passed as format string to snprintf without parsing
snprintf(&dest[strlen(dest)], 254 - strlen(dest), filename);
```

Now that we have found the vulnerability let's try to exploit it

We can start by using it to display the memory of the program





0x400ab3