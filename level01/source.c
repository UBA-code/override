
int verify_user_name(char *username)
{
  bool c;
  bool sameChar;
  puts("verifying username....\n");
  int i = 7;
  const char* realPass = "dat_wil";
  
  while (i)
  {
    char userNameChar = *(uint8_t*)userName;
    char const realPassChar = *(uint8_t*)realPass;
    c = userNameChar < realPassChar;
    sameChar = userNameChar == realPassChar;
    userName += 1;
    realPass = &realPass[1];
    i -= 1;
    
    if (!sameChar)
    break;
}

return (int)((!sameChar && !c) - c);
}

int verify_user_pass(char *password)

{
  int i;
  char *realPassword;
  bool isCharLower;
  bool isCharSame;
  
  i = 5;
  realPassword = "admin";
  do {
     if (i == 0) break;
     i = i + -1;
     isCharLower = (byte)*password < (byte)*realPassword;
     isCharSame = *password == *realPassword;
     password = (char *)((byte *)password + 1);
     realPassword = realPassword + 1;
  } while (isCharSame);
  return (int)(char)((!isCharLower && !isCharSame) - isCharLower);
}

int main(int argc, char** argv, char** envp)
{
    void buf;
    memset(&buf, 0, 64);
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(&a_user_name, 256, stdin);
    
    if (verify_user_name(a_user_name))
    {
        puts("nope, incorrect username...\n");
        return 1;
    }
    
    puts("Enter Password: ");
    fgets(&buf, 100, stdin);
    int isDiff = verify_user_pass(&buf);

    if (isDiff && !isDiff)
    {
      puts("nope, incorrect password...\n");
      return 1;
    }
}
