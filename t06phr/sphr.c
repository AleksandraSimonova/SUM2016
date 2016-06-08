#include <stdio.h>
#include <math.h>
#include <time.h>
#include "sphere.h"
#define pi 3.1415926535
#define N 51
#define M 30
typedef double DBL; 
typedef struct
{
  DBL X, Y, Z;
} VEC;
VOID DrawSphere(HDC hDC, INT Xc, INT Yc)
{
  INT i, j;
  DOUBLE theta, phi, phase = clock() / 10000.0;
  DOUBLE x, y, z;
  INT R = 400;
  static VEC G[N][M];

  for (i = 0; i < N; i++)
  {
    theta = i * pi / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * pi / (M - 1) + phase;
      G[i][j].X =  R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);
     
    /*  G[i][j] = Rot(G[i][j], A, 30 * sin(clock() / 5000.0)); */   
    }
  }
  for (i = 0; i < N; i++)
  {
    theta = i * pi / (N - 1);
    for (j = 0; j <= M; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Y;
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
  }

  for (i = 0; i < N; i++)
  {
    theta = i * pi / (N - 1);
    MoveToEx(hDC, x, y,NULL);
    for (j = 0; j <= M; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Y;
     LineTo(hDC, x, y);
    }
  }
  for (j = 0; i <= M; j++)
  {
    phi = j * 2 * pi / (M - 1) + phase;
    MoveToEx(hDC, x, y,NULL);
    for (i = 1; i <= N; i++)
    {
      theta = i * pi / (N - 1);
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Y;
      LineTo(hDC, x, y);
    }
  }

  
}