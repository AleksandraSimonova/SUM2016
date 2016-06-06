/*
 *
 */
#include <stdio.h>

#define MAX 30

double A[MAX][MAX];
int N;
double sum = 0;

int Parity = 0, P[MAX];

void Swap( int *A, int *B )
{
  int tmp = *A; 

  *A = *B;
  *B = tmp;
}

void LoadMatrix( char *FileName )
{
  int i, j;
  FILE *F;

  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &N);
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
}

void Go(int Pos)
{
  int i;

  if (Pos == N)
  {
    double prod = 1;

    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];
    if (Parity == 0)
      sum += prod;
    else 
      sum -= prod;
    return;
  }
  else 
  {
    /*Go(Pos + 1);
      SaveParity = Parity;
    for(i = 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
    }
    x = P[Pos];
    for(i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;
    Parity = SaveParity;  */
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
    }  
  }
}  

double EvalDeterminant( char *FileName )
{
  int i;

  LoadMatrix(FileName);
  sum = 0;
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void main( void )
{
  int i;
  char *M[] = 
  {
    "mat1.txt"
  };

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
}

