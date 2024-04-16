# LEVEL00 - REVERSE 1 - PLAIN PASSWORD

We have a binary named level00 at /home/users/level00/

It is owned by `level01` user and has a setuid bit

Here is the ASM dump of the main function:

```
Dump of assembler code for function main:
   0x08048494 <+0>:     push   %ebp
   0x08048495 <+1>:     mov    %esp,%ebp
   0x08048497 <+3>:     and    $0xfffffff0,%esp
   0x0804849a <+6>:     sub    $0x20,%esp
   0x0804849d <+9>:     movl   $0x80485f0,(%esp)
   0x080484a4 <+16>:    call   0x8048390 <puts@plt>
   0x080484a9 <+21>:    movl   $0x8048614,(%esp)
   0x080484b0 <+28>:    call   0x8048390 <puts@plt>
   0x080484b5 <+33>:    movl   $0x80485f0,(%esp)
   0x080484bc <+40>:    call   0x8048390 <puts@plt>
   0x080484c1 <+45>:    mov    $0x804862c,%eax
   0x080484c6 <+50>:    mov    %eax,(%esp)
   0x080484c9 <+53>:    call   0x8048380 <printf@plt>
   0x080484ce <+58>:    mov    $0x8048636,%eax
   0x080484d3 <+63>:    lea    0x1c(%esp),%edx
   0x080484d7 <+67>:    mov    %edx,0x4(%esp)
   0x080484db <+71>:    mov    %eax,(%esp)
   0x080484de <+74>:    call   0x80483d0 <__isoc99_scanf@plt>
   0x080484e3 <+79>:    mov    0x1c(%esp),%eax
   0x080484e7 <+83>:    cmp    $0x149c,%eax
   0x080484ec <+88>:    jne    0x804850d <main+121>
   0x080484ee <+90>:    movl   $0x8048639,(%esp)
   0x080484f5 <+97>:    call   0x8048390 <puts@plt>
   0x080484fa <+102>:   movl   $0x8048649,(%esp)
   0x08048501 <+109>:   call   0x80483a0 <system@plt>
   0x08048506 <+114>:   mov    $0x0,%eax
   0x0804850b <+119>:   jmp    0x804851e <main+138>
   0x0804850d <+121>:   movl   $0x8048651,(%esp)
   0x08048514 <+128>:   call   0x8048390 <puts@plt>
   0x08048519 <+133>:   mov    $0x1,%eax
   0x0804851e <+138>:   leave
   0x0804851f <+139>:   ret
End of assembler dump.
```

The program prints some lines and then calls scanf to ask for user input
```
   0x0804849a <+6>:     sub    $0x20,%esp
   0x0804849d <+9>:     movl   $0x80485f0,(%esp)
   0x080484a4 <+16>:    call   0x8048390 <puts@plt>
   0x080484a9 <+21>:    movl   $0x8048614,(%esp)
   0x080484b0 <+28>:    call   0x8048390 <puts@plt>
   0x080484b5 <+33>:    movl   $0x80485f0,(%esp)
   0x080484bc <+40>:    call   0x8048390 <puts@plt>
   0x080484c1 <+45>:    mov    $0x804862c,%eax
   0x080484c6 <+50>:    mov    %eax,(%esp)
   0x080484c9 <+53>:    call   0x8048380 <printf@plt>
   0x080484ce <+58>:    mov    $0x8048636,%eax
   0x080484d3 <+63>:    lea    0x1c(%esp),%edx
   0x080484d7 <+67>:    mov    %edx,0x4(%esp)
   0x080484db <+71>:    mov    %eax,(%esp)
   0x080484de <+74>:    call   0x80483d0 <__isoc99_scanf@plt>
```
```
(gdb) x/s 0x80485f0
0x80485f0:       '*' <repeats 35 times>
(gdb) x/s 0x8048390
0x8048390 <puts@plt>:    "\377%\004\240\004\bh\b"
(gdb) x/s 0x80485f0
0x80485f0:       '*' <repeats 35 times>
(gdb) x/s 0x8048614
0x8048614:       "* \t     -Level00 -\t\t  *"
(gdb) x/s 0x80485f0
0x80485f0:       '*' <repeats 35 times>
(gdb) x/s 0x804862c
0x804862c:       "Password:"
```

Then have a comparison between the value 0x149c (5276) and the input passed as parameter
```
0x080484e7 <+83>:    cmp    $0x149c,%eax
```
```
Starting program: /home/users/level00/level00
***********************************
*            -Level00 -           *
***********************************
Password:51

Breakpoint 1, 0x080484e7 in main ()
(gdb) info reg
eax            0x33     51
```

If the comparison returns `false` we jump to `main+121`
```
   0x080484ec <+88>:    jne    0x804850d <main+121>
```

Then we will display a line and leave the program
```
   0x0804850d <+121>:   movl   $0x8048651,(%esp)
   0x08048514 <+128>:   call   0x8048390 <puts@plt>
   0x08048519 <+133>:   mov    $0x1,%eax
   0x0804851e <+138>:   leave
   0x0804851f <+139>:   ret
```
```
(gdb) x/s 0x8048651
0x8048651:       "\nInvalid Password!"
```

If the comparison returns `true`, we will print a line and call the `system` function with some arguments and then leave.
```
   0x080484ee <+90>:    movl   $0x8048639,(%esp)
   0x080484f5 <+97>:    call   0x8048390 <puts@plt>
   0x080484fa <+102>:   movl   $0x8048649,(%esp)
   0x08048501 <+109>:   call   0x80483a0 <system@plt>
   0x08048506 <+114>:   mov    $0x0,%eax
   0x0804850b <+119>:   jmp    0x804851e <main+138>
```
```
(gdb) x/s 0x8048639
0x8048639:       "\nAuthenticated!"
```
```
(gdb) x/s 0x8048649
0x8048649:       "/bin/sh"
```

Let's try 5276 as password
```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ whoami
level01
```

We can cat the `.pass` file

```
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```