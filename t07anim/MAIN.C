/* file name: main.c
 *progremmer: sa2
 *date: 11.06.2016
 */
 
#include "anim.h"
#include "units.h"
#define SA2_WND_CLASS_NAME "My window class"




LRESULT CALLBACK SA2_MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *Cmdline, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd; 
  MSG msg;

  SetDbgMemHooks();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = SA2_WND_CLASS_NAME;              
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = SA2_MyWindowFunc;
  if (!RegisterClass(&wc) )
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }
  
  hWnd = CreateWindow(SA2_WND_CLASS_NAME,
    "Animation System",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  SA2_AnimAddUnit(SA2_UnitCreateBall());

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
}
LRESULT CALLBACK SA2_MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  static INT W, H;
  HDC hDC;
  static HDC hMemDC;
  PAINTSTRUCT ps; 
  MINMAXINFO *MinMax;

  switch (Msg)
  {
  case  WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax -> ptMaxTrackSize.y = 
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    SA2_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    SA2_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  
  case WM_MOUSEWHEEL:
/*    SA2_MouseWheel += (SHORT)HIWORD(wParam);    */
    return 0;
  
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    SA2_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    SA2_AnimCopyFrame(hDC);   
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    SA2_AnimClose();
    KillTimer(hWnd, 30); 
    PostQuitMessage(0);
    return 0;
  
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} 
