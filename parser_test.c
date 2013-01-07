struct thing {
  int i, j;
} t;

double thing(int i, int j);

signed int main()
{
  thing(1, 2, 43);
  int i, j = 1;

  char a = 'a';

  for(i = (1 + 2) * 2; i < 10; i++)
  {

    if(i % 2 == 0)
    {
      int j = 0;
      while(j < 10) break;
      j += i;
      i += j + 1 + 2 + 3;
    }
    else a += 1;

    for(;;)
    {
      continue;
    }
  }

  i = j+++a+2;

  return 0;
}