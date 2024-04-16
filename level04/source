#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>

#define EXECVE_SYSCALL 11
#define EIP_OFFSET 44

int main(void)
{
	int childStatus;
	char shellcode[128];
	int syscallNumber;
	__pid_t childPid;

	childPid = fork();
	memset(shellcode, 0, sizeof(shellcode));
	syscallNumber = 0;
	childStatus = 0;
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
	else
	{
		// child
		prctl(1, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(shellcode);
	}
	return 0;
}