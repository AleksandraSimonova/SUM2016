/* file name: anim.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")


sa2ANIM SA2_Anim;
static UNIT64;
  SA2_StartTime;
  SA2_OldTime;
  SA2_OldTimeFPS;
  SA2_PauseTime;
  SA2_TimePerSec;
  SA2_FrameCounter;

VOID  SA2_AnimInit(hWnd)
{
  HDC hDC;
  LARGE_INTEGER t;
  memset(&SA2_Anim, 0, sizeof(sa2ANIM));
  SA2_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  SA2_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);    
}
VOID  SA2_AnimClose(VOID)
{
  INT i;
  for (i = 0; i < SA2_Anim.NumOfUnits; i++)
  {
    SA2_Anim.Units[i]->Close(SA2_Anim.Units[i], &SA2_Anim);
    free(SA2_Anim.Units[i]);
  }
  SA2_Anim.NumOfUnits = 0;
  DeleteDC(SA2_Anim.hDC);
  DeleteObject(SA2_Anim.hFrame);
  memset(&SA2_Anim, 0, sizeof(sa2ANIM));  
   

}
VOID  SA2_AnimResize(W, H)
{
  HDC hDC;
  SA2_Anim.W = W;
  SA2_Anim.H = H;
 if (SA2_Anim.hFrame != NULL)
      DeleteObject(SA2_Anim.hFrame);
    hDC = GetDC(SA2_Anim.hWnd);
    ReleaseDC(SA2_Anim.hWnd, hDC);
    SelectObject(SA2_Anim.hDC, SA2_Anim.hFrame); 
    
}
VOID  SA2_AnimCopyFrame(HDC hDC)
{
  BitBlt(hDC, 0, 0, SA2_Anim.W, SA2_Anim.H, SA2_Anim.hDC, 0, 0, SRCCOPY);
}
VOID  SA2_AnimRender(VOID)
{
 
  INT i;
  LARGE_INTEGER t;
  SA2_FrameCounter++;
  QueryPerformanceCounter(&t);
  SA2_Anim.GlobalTime = (DBL)(t.QuadPart - SA2_StartTime) / SA2_TimePerSec;
  SA2_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - SA2_OldTime) / SA2_TimePerSec;
  if(SA2_Anim.IsPause)
  {
    SA2_Anim.DeltaTime = 0;
    SA2_PauseTime += t.QuadPart - SA2_OldTime;
  }
  else
  {
     SA2_Anim.DeltaTime = SA2_Anim.GlobalDeltaTime;
     SA2_Anim.Time = (DBL)(t.QuadPart - SA2_PauseTime - SA2_StartTime) / SA2_TimePerSec;
  }
  if(t.QuadPart - SA2_OldTimeFPS > SA2_TimePerSec )
  {
    CHAR str[100];
    SA2_Anim.FPS = SA2_FrameCounter * SA2_TimePerSec / (DBL)(t.QuadPart - SA2_OldTimeFPS);
    SA2_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5F", SA2_Anim.FPS);
    SetWindowText(SA2_Anim.hWnd, str);
    SA2_FrameCounter = 0;
  }
  SA2_OldTime = t.QuadPart;

  for (i = 0; i < SA2_Anim.NumOfUnits; i++)
  SA2_Anim.Units[i]->Response(SA2_Anim.Units[i], &SA2_Anim);
  SelectObject(SA2_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(SA2_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(SA2_Anim.hDC, RGB(100, 155, 220));
  Rectangle(SA2_Anim.hDC, 0, 0, SA2_Anim.W + 1, SA2_Anim.H + 1);

  
  for (i = 0; i < SA2_Anim.NumOfUnits; i++)
  {
    SelectObject(SA2_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(SA2_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(SA2_Anim.hDC, RGB(0, 0, 0));
    SetDCBrushColor(SA2_Anim.hDC, RGB(255,255, 255));

    SA2_Anim.Units[i]->Render(SA2_Anim.Units[i], &SA2_Anim);
  }                        
 
}                          
VOID  SA2_AnimAddUnit(sa2UNIT *Uni) 
{
 
  if (SA2_Anim.NumOfUnits < MAX_UNITS)
  {
    SA2_Anim.Units[SA2_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &SA2_Anim);
    
  }
}


