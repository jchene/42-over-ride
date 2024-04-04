int __cdecl __noreturn main(int argc, const char **argv, const char **envp)
{
  char s[100]; // [esp+28h] [ebp-70h] BYREF
  unsigned int i; // [esp+8Ch] [ebp-Ch]

  i = 0;
  fgets(s, 100, stdin);
  for ( i = 0; i < strlen(s); ++i )
  {
    if ( s[i] > 64 && s[i] <= 90 )
      s[i] ^= 0x20u;
  }
  printf(s);
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