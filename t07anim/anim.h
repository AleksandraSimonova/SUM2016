/* file name: anim.h
 *progremmer: sa2
 *date: 08.06.2016
 */
#ifndef  __ANIM_H_
#define  __ANIM_H_
#include "def.h"

#define MAX_UNITS 1000

typedef struct tagsa2UNIT sa2UNIT;

 
typedef struct tagsa2ANIM
{
  HWND hWnd; 
  HDC hDC;   
  INT W, H;
  HBITMAP hFrame; 
  sa2UNIT *Units[MAX_UNITS]; 
  INT NumOfUnits; 
  
} sa2ANIM;
struct tagsa2UNIT
{
  VOID (* Init)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Close)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Response)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Render)(sa2UNIT *Uni, sa2ANIM *Ani);
} ;
#endif
    

