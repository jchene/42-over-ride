# Override - Exploiting ELF-32/64 binaries on x86_64 architecture

This project consists of multiple levels, each representing a unique challenge. The goal is to exploit vulnerabilities in each level to gain access to the next one.

## Structure

Each level in the project has the following structure:

- `flag`: A file containing the password for the next level.
- `Ressources/`: A directory containing all resources needed to solve the challenge.
- `README.md`: A file explaining how to solve the challenge for the current level.
- `source`: A human readable source code version of the binary for the current level.

## Levels

Here is a list of the topics covered by each level:

- [Level 00 - REVERSE 1 - PLAIN PASSWORD](./level00/)
- [Level 01 - BUFFER OVERFLOW 1 - SHELLCODE](./level01/)
- [Level 02 - FORMAT STRING 1 - READ MEMORY](./level02/)
- [Level 03 - REVERSE 2 - ENCRYPTED PASSWORD](./level03/)
- [Level 04 - BUFFER OVERFLOW 2 - SYSCALL BAN](./level04/)
- [Level 05 - FORMAT STRING 2 - PLT OVERRIDE](./level05/)
- [Level 06 - REVERSE 3 - ENCRYPTED STRING](./level06/)
- [Level 07 - INT OVERFLOW - NO BOUNDARIES](./level07/)
- [Level 08 - REVERSE 4 - RELATIVE PATH](./level08/)
- [Level 09 - BUFFER OVERFLOW 3 - EXTRA BYTE](./level09/)
