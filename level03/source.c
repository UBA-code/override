int decrypt(char a1)
{
  unsigned int i;
  unsigned int v3;
  char v4[17];

  strcpy((char *)v4, "Q}|u`sfg~sf{}|a3");
  v3 = strlen((const char *)v4);
  for ( i = 0; i < v3; ++i )
    *((char *)v4 + i) ^= a1;
  if ( !strcmp((const char *)v4, "Congratulations!") )
    return system("/bin/sh");
  else
    return puts("\nInvalid Password");
}

int32_t test(int32_t arg1, int32_t arg2)
{
    int32_t ecx_1 = arg2 - arg1;
    
    switch (ecx_1)
    {
        case 1:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 2:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 3:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 4:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 5:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 6:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 7:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 8:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 9:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 16:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 17:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 18:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 19:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 20:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
        case 21:
        {
            return decrypt((uint8_t)ecx_1);
            break;
        }
    }
    
    return decrypt(rand());
}



int main()
{
  uint __seed;
  int n;
  
  __seed = time((time_t *)0x0);
  srand(__seed);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &n);
  test(n,322424845);
  return 0;
}
