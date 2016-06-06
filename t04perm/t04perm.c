/* FILE NAME: T04PERM.C
   PROGRAMM: sa2
   DATE: 02.06.2016
*/
#include <stdio.h>
#define N 5
int P[N] = {0,1, 2, 3, 4, 5};
int Parity = 0;


void Swap(int *A, int *B)
{
    int tmp = *A; 
    *A = *B;
    *B = tmp;
}
void SavePerm( void )
{
  int i;
  FILE *F;

  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for(i = 0; i < N - 1; i++)
    fprintf(F, "%i", P[i]);
  if(Parity)
    fprintf(F,"odd\n");
  else
    fprintf(F, "even\n");
  
  fclose(F);
}
void Go(int Pos)
{
  int i;

  if (Pos == N)
  {
    SavePerm();
    return;
  }
  else 
  {
    for( i=Pos; i < N; i++)
    {
      if(Pos != 1)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
      Go(Pos + 1);
    
    }
  }
}

void main( void )
{
  int i;
  for(i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
}