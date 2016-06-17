#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SPHR.h"
#define pi 3.1415926535
#define N 51
#define M 30
VOID DrawSphere(HDC hDC, INT Xc, INT Yc)
{
  INT i, j;
  DOUBLE theta, phi, phase = clock() / 10000.0;
  DOUBLE x, y;
  INT R = 400;

  for (i = 0; i < N; i++)
  {
    theta = i * pi / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * pi / (M - 1) + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
     
      Ellipse(hDC, x- 5, y - 5, x + 5, y + 5);
    }
  }
  for (i = 0; i < N; i++)
  {
    theta = i * pi / (N - 1);
    x = Xc + R * sin(theta) * cos(phase);
    y = Yc - R * cos(theta); 
    MoveToEx(hDC, x, y, NULL);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * pi / (M - 1) + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }
  
  for(j = 0; j <= N; j++)
  {
    phi = j * 2 * pi / (M - 1) + phase;
    x = Xc;
    y = Yc - R;
    MoveToEx(hDC, x, y, NULL);
    for(i = 1; i < N; i++)
    {
      theta = i * pi / (N - 1);
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }
  
}
