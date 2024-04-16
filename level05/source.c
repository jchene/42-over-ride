#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char string[100];
  unsigned int i;

  i = 0;
  fgets(string, 100, stdin);
  for ( i = 0; i < strlen(string); ++i )
  {
    if ( string[i] >= 'A' && string[i] <= 'Z' )
      string[i] ^= 0b00100000;
  }
  printf(string);
  exit(0);
}

#BIG ENDIAN BASE
39617044 28293664 6d617045 64

#BIG ENDIAN BLOCK INVERTED
64 6d617045 28293664 39617044

#LITTLE ENDIAN BASE
44706139 64362928 4570616d 64

#LITTLE ENDIAN BLOCK INVERTED
64 4570616d 64362928 44706139