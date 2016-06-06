#include <windows.h>
#include <math.h>


VOID Draw(HDC hDC, INT X, INT Y, DOUBLE A)
{
  INT i;
  static INT w, h;
  POINT pts;
  DOUBLE rad = A * 3.14 / 180, si = sin(rad), co = cos(rad);  
  static POINT pt[] = 
  {
    {-10, 0}, {-10, 50}, {0, 150}, {10, 50}, {10, 0}
  };
  POINT pt1[sizeof(pt) / sizeof(pt[0])];
  GetCursorPos(&pts);
  ScreenToClient(hDC, &pts);
  MoveToEx(hDC, w / 2, h / 2, NULL);
  LineTo(hDC, pts.x, pts.y);
  si = (Y - pts.y) / sqrt((pts.x - X) * (pts.x - X) + (Y - pts.y) * (Y - pts.y));
  co = (pts.x - X) / sqrt((pts.x - X) * (pts.x - X) + (Y - pts.y) * (Y - pts.y));
  
  for(i = 0; i < sizeof(pt) / sizeof(pt[0]); i++)
  {
    pt1[i].x = X + pt[i].x * co - pt[i].y * si;
    pt1[i].y = Y - (pt[i].x * si + pt[i].y * co);
    
  }
 /* GetCursorPos(&pt1);
  ScreenToClient(hDC, &pt1);*/
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 220, 220));
  Polygon(hDC, pt1, sizeof(pt) / sizeof(pt[0]));
}



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  DOUBLE a, b, c, g;
  SYSTEMTIME t;
  CHAR s[30];
  POINT pt;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  HPEN hPen;
  HBRUSH hBr, hOldBr;
  
 

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
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY); 
    EndPaint(hWnd, &ps);

    return 0;

    
  case WM_CREATE: 
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "CLOCKFACE.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDCLogo = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDCLogo, hBmLogo);
    return 0;
    case WM_ERASEBKGND:
    return 0;  
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    BitBlt(hMemDC,(w - bm.bmWidth) / 2,(h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight,
           hMemDCLogo, 0, 0, SRCCOPY);
   

   /* GetLocalTime(&t);
    a = t.wSecond * 2 * 3.14159265358979 / 60;
    r = bm.bmWidth / 2.2;
    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
    SelectObject(hMemDC, hPen);
    
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);
    a = t.wMinute * 2 * 3.14159265358979 / 60;
    r = bm.bmWidth / 2.2;
    hPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 255));
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);
    a = t.wHour * 2 * 3.14159265358979 / 12;
    r = bm.bmWidth / 2.2;
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 255));
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);
    SetTextColor(hMemDC, RGB(130, 90, 30));
    TextOut(hMemDCLogo, 0, h / 2 + h / 2 * sin(clock()/2000.0), s,
            sprintf(s, "%02i.%02i.%i", t.wDay, t.wMonth, t.wMonth));*/ 
    Draw(hMemDC, w / 2, h / 2, 200);
   
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
    case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
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
