#include <windows.h>
#include <math.h>

/*#define WND_CLASS_NAME "My Window Class"   */


void DrawEye(HWND hWnd, INT x, INT y, INT R, INT R1)
{
  HDC hDC;
  POINT pt;
  INT dx, dy;
  DOUBLE t;
  
  
  GetCursorPos(&pt);

  ScreenToClient(hWnd, &pt);
  dx = pt.x - x;
  dy = pt.y - y;
  t = (R - R1)/ sqrt( dx * dx + dy * dy);
  
  if( t < 1)
  {
    dx *= t;
    dy *= t ;
  }
  Ellipse(hDC, x - R1, y + R1, x + R1, y + R1);
  Ellipse(hDC, x + dx - R1, y + dy - R1, x + dx + R1, y + dy + R1);
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt; 
  static INT w, h;  

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEMOVE:
    DrawEye(hWnd, 100, 50, 15, 5);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  case WM_CREATE: 
    SetTimer(hWnd, 30, 10, NULL);
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

