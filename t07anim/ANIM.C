/* file name: anim.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include <stdio.h>
#include "anim.h"


sa2ANIM SA2_Anim;
VOID  SA2_AnimInit(hWnd)
{
  HDC hDC;
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
    SetDCBrushColor(SA2_Anim.hDC, RGB(255, 0, 0));

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


