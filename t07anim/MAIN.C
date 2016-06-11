/* file name: main.c
 *progremmer: sa2
 *date: 11.06.2016
 */
 
#include "def.h"



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  static INT W, H;
  HDC hDC;
  static HDC hMemDC;
  PAINTSTRUCT ps;
  SetDbgMemHooks();
  switch (Msg)
  {
    case  WM_GETMINMAXINFO:
      
      return 0;
    case WM_CREATE:
     SetTimer(hWnd, 30, 10, NULL);
     SA2_AnimInit(hWnd);
      return 0;
    case WM_SIZE:
     SA2_AnimResize(LOWORD(lParam), HIWORD(lParam));
     SendMessage(hWnd, WM_TIMER, 0, 0);
      return 0;
    case WM_TIMER:
     SA2_AnimRender();
     InvalidateRect(hWnd, NULL, FALSE);
      return 0;
    case WM_ERASEBKGND:
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

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *Cmdline, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd; 
  MSG msg;

  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = "My Window Class";              
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWindowFunc;
  if (!RegisterClass(&wc) )
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }
  
  hWnd = CreateWindow("My Window Class",
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 0;
}
