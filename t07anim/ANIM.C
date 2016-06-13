/* file name: anim.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#define SA2_GET_JOYSTIC_AXIS(A) \ (2.0 * (ji.dw##A##pos - jc.dw##A##min) / (ji.dw##A##pos - jc.dw##A##min) - 1) - 1)

INT SA2_MouseWheel;
sa2ANIM SA2_Anim;
static UNIT64;
  SA2_StartTime;
  SA2_OldTime;
  SA2_OldTimeFPS;
  SA2_PauseTime;
  SA2_TimePerSec;
  SA2_FrameCounter;

VOID SA2_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;
  memset(&SA2_Anim, 0, sizeof(sa2ANIM));
  SA2_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  SA2_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  QueryPerformanceFrequency(&t);
  SA2_TimePerSec = t. QuadPart;
  QueryPerformanceCounter(&t);
  SA2_StartTime = SA2_OldTime = SA2_OldTimeFPS = t. QuadPart;
  SA2_PauseTime = 0;
}
VOID  SA2_AnimClose( VOID )
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
VOID  SA2_AnimResize( INT W, INT H )
{
  HDC hDC;
  SA2_Anim.W = W;
  SA2_Anim.H = H;
  if (SA2_Anim.hFrame != NULL)
      DeleteObject(SA2_Anim.hFrame);
  hDC = GetDC(SA2_Anim.hWnd);
  SA2_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(SA2_Anim.hWnd, hDC);
  SelectObject(SA2_Anim.hDC, SA2_Anim.hFrame); 
}
VOID  SA2_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, SA2_Anim.W, SA2_Anim.H, SA2_Anim.hDC, 0, 0, SRCCOPY);
}
VOID  SA2_AnimRender(VOID)
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;

  SA2_FrameCounter++;

  /* Update timer */
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

  /* Update keyboard */
  GetKeyboardState(SA2_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    SA2_Anim.Keys[i] >>= 7;
    if(!SA2_Anim.OldKeys[i] && SA2_Anim.Keys[i])
      SA2_Anim.KeysClick[i] = TRUE;
    else
      SA2_Anim.KeysClick[i] = FALSE;
  }
  memcpy(SA2_Anim.OldKeys, SA2_Anim.Keys, 256);

  /* Update mouse */
  GetCursorPos(&pt);
  ScreenToClient(SA2_Anim.hWnd, &pt);
  SA2_Anim.Mdx = pt.x - SA2_Anim.Mx;
  SA2_Anim.Mdy = pt.y - SA2_Anim.My;
  SA2_Anim.Mx = pt.x;
  SA2_Anim.My = pt.y;

  /* Update joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      
      JOYINFOEX ji;
      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
         /*buttons*/
        for (i = 0; i < 32; i++)
          SA2_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          

        /*axes*/
       /*  SA2_Anim.JX = SA2_GET_JOYSTIC_AXIS(X);
         SA2_Anim.JY = SA2_GET_JOYSTIC_AXIS(Y);    
         SA2_Anim.JZ = SA2_GET_JOYSTIC_AXIS(Z);
         SA2_Anim.JR = SA2_GET_JOYSTIC_AXIS(R);
         SA2_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;         */
      }
    }
  }
  SA2_RndMatrWorld  = MatrIdentity();
  SA2_RndMatrView = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 0, 0));
  SA2_RndMatrProj = MatrFrustum;


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
VOID SA2_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;
  if(IsExit)
    return;
  IsExit = TRUE;
  PostMessage(SA2_Anim.hWnd, WM_CLOSE, 0, 0);
}



