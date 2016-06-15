Начало реализации:

MAIN.C - WinMain + WinFunc (работа с WinAPI)
ANIM.C - VG4_Anim***
  VG4_AnimInit(hWnd)
  VG4_AnimClose()
  VG4_AnimResize(W, H)
  VG4_AnimCopyFrame(HDC)
  VG4_AnimRender()
  VG4_AnimAddUnit(Uni)
+ VG4_Anim - глобальная переменная - структура контекста анимации
vg4ANIM:
typedef struct
{
  HWND hWnd; - окно
  HDC hDC;   - контекст в памяти
  INT W, H;  - размер окна
  HBITMAP hFrame; - картинка кадра
  vg4UNIT *Units[VG4_MAX_UNITS]; - массив объектов анимации
  INT NumOfUnits; - текущее количество объектов анимации
  . . .
} vg4ANIM;

UNITS.C - вся базовая работа с объектами анимации
typedef struct tagvg4UNIT vg4UNIT;
struct tagvg4UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( vg4UNIT *Uni, vg4ANIM *Ani );
};

--- функции - "заглушки" - нужны для начальной инициализации любого объекта анимации по умолчанию

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;                                            
 * RETURNS: None.                                                
 */                                                               
static VOID VG4_UnitInit( vg4UNIT *Uni, vg4ANIM *Ani )            
{                                                                 
} /* End of 'VG4_UnitInit' function */                            
                                                                  
/* Unit deinitialization function.                                
 * ARGUMENTS:                                                     
 *   - self-pointer to unit object:                               
 *       vg4UNIT *Uni;                                             
 *   - animation context:                                           
 *       vg4ANIM *Ani;                                              
 * RETURNS: None.                                                   
 */
static VOID VG4_UnitClose( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT *Uni, vg4ANIM *Ani );
{
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitRender' function */

- основная функция создания объекта анимации по умолчанию:

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size )
{
  vg4UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(vg4UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = VG4_UnitInit;
  Uni->Close = VG4_UnitClose;
  Uni->Response = VG4_UnitResponse;
  Uni->Render = VG4_UnitRender;
  return Uni;
} /* End of 'VG4_AnimUnitCreate' function */

пример своего объекта анимации:
BBALL.C
#include "anim.h"

typedef struct
{
  vg4UNIT;
  VEC Pos;
} vg4UNIT_BALL;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(0, 1, 0);
} /* End of 'VG4_UnitInit' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  DrawSphere(VecAddVec(Uni->Pos,
                       VecMulNum(VecSet(0, 1, 0), sin(Ani->Time)),
             1); 
} /* End of 'VG4_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateBall( VOID )
{
  vg4UNIT *Uni;

  if ((Uni = VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = VG4_UnitInit;
  Uni->Render = VG4_UnitRender;
  return Uni;
} /* End of 'VG4_UnitCreateBall' function */


!!! общий для всего проекта файл объявлений:

DEF.H:

#ifndef __DEF_H_
#define __DEF_H_


#include <windows.h>

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */

#include <stdlib.h>
/* !!! в начале WinMain: SetDbgMemHooks(); */

typedef FLOAT FLT;
typedef DOUBLE DBL;

#include "vec.h"
...

#endif /* __DEF_H_ */


!!! включаем "def.h" в любой заголовке проекта.

ANIM.H

прототипы всего от системы анимации:
все типы!!! 

typedef struct tagvg4UNIT vg4UNIT;
typedef struct
{
  . . . vg4UNIT * ...
} vg4ANIM;
struct tagvg4UNIT
{
  . . . vg4ANIM . . .
};


... VG4_Anim ...
... VG4_Unit ...




VOID VG4_AnimClose( VOID )
{
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
  }
  VG4_Anim.NumOfUnits = 0;
  . . .
}

VOID VG4_AnimRender( VOID )
{
  . . . опросили все (kbd, mouse, joystick)
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  . . . очищаем кадр
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    . . . можно сбросить все кисти и перья
    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
  . . .
}

