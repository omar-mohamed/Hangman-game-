// g4c.cpp : Defines the entry point for the application.
//

//#include "stdafx.h"
#include "g4c.h"
#include <windows.h>
#include <time.h>
#include <string>
#include <stdio.h>
using namespace std;
void error(char *x) 
{
}

std::wstring s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GWnd *gWnd = NULL;

/*  WinMain(), our entry point  */

COLORREF GWnd::colortable[16];
HPEN GWnd::pentable[16];
HBRUSH GWnd::brushtable[16];
SpriteProc GWnd::sprProc =NULL;
MouseProc GWnd::mouseProc = NULL;
KeyProc GWnd::keyDownProc = NULL;
KeyProc GWnd::keyUpProc = NULL;

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
  RECT rcClient, rcWindow;
  POINT ptDiff;
  GetClientRect(hWnd, &rcClient);
  GetWindowRect(hWnd, &rcWindow);
  ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
  ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
  MoveWindow(hWnd,rcWindow.left, rcWindow.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		   LPSTR szCmdLine, int iCmdShow) {
    static LPCWSTR szAppName = L"winhello";
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;
	gWnd =new GWnd;

    /*  Fill in WNDCLASSEX struct members  */

    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;

    
    /*  Register a new window class with Windows  */

    RegisterClassEx(&wndclass);


    /*  Create a window based on our new class  */

    hwnd = CreateWindow(szAppName, L"Application Running",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			winWidth, winHeight,
			NULL, NULL, hInstance, NULL);

	ClientResize(hwnd, winWidth, winHeight);
	
    /*  Show and update our window  */

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);


    /*  Retrieve and process messages until we get WM_QUIT  */

	while( TRUE )
    {
     

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // Check for a quit message
            if( msg.message == WM_QUIT )
                break;

            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else 
        {
			gWnd->OnIdle();
        }
    }


    /*  Exit with status specified in WM_QUIT message  */

    return (int )msg.wParam;
}

/*  Window procedure  */

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
	POINTS p;
//    HDC         hdc;
    switch ( iMsg ) {
	case WM_CREATE:
		gWnd->hWnd = hwnd;
		gWnd->OnCreate();
		return 0;
    case WM_PAINT:
		//hdc =GetDC(hwnd);
	//	gWnd->OnPaint(NULL,NULL);
	//	BeginPaint(hwnd,&ps);
	//	EndPaint(hwnd,&ps);
	//	ReleaseDC(hwnd,hdc);
	return 0;
	case WM_KEYUP:
		if(wParam==VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		else
		{
			gWnd->OnKeyUp((int)wParam);
		}

		return 0;
	case WM_KEYDOWN:
		gWnd->OnKeyDown((int)wParam);
		return 0;
	case WM_CHAR:
		gWnd->OnChar((int )wParam);
		return 0;
	case WM_SETFOCUS:
		gWnd->OnSetFocus();
		return 0;
	case WM_KILLFOCUS:
		gWnd->OnKillFocus();
		return 0;
	case WM_LBUTTONDOWN:
		gWnd->leftMouseDown=true;
		p=MAKEPOINTS(lParam);
		gWnd->OnMouseMove(p.x,p.y);
		return 0;
	case WM_LBUTTONUP:
		gWnd->leftMouseDown=false;
		p=MAKEPOINTS(lParam);
		gWnd->OnMouseMove(p.x,p.y);
		return 0;
	case WM_RBUTTONDOWN:
		gWnd->rightMouseDown=true;
		p=MAKEPOINTS(lParam);
		gWnd->OnMouseMove(p.x,p.y);
		return 0;
	case WM_RBUTTONUP:
		gWnd->rightMouseDown=false;
		p=MAKEPOINTS(lParam);
		gWnd->OnMouseMove(p.x,p.y);
		return 0;
	case WM_MOUSEMOVE:
		p=MAKEPOINTS(lParam);
		gWnd->OnMouseMove(p.x,p.y);
		return 0;
    case WM_DESTROY:
		gWnd->CleanUp();
		delete gWnd;
		PostQuitMessage(0);
	return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
DWORD WINAPI ThreadProc(
  LPVOID lpParameter
)
{
	
	main();
	gWnd->SetText(L"Application Finished");
	
	return 0;
}
DWORD WINAPI ThreadProc2(
  LPVOID lpParameter
)
{
	long frame=0;
	gWnd->timer.Update(clock());
	while(gWnd->appRunning)
	{
		gWnd->timer.Update(clock());
	
		gWnd->OnPaint(NULL,NULL);
		//char str[200];
		//sprintf(str,"Frame  %d",frame++);
		//gWnd->SetText(str);
	
		Sleep(gWnd->timer.timeTillNextFrame);


	}

	return 0;
}


void GWnd::SetText(LPCTSTR t)
{
	SetWindowText(hWnd,t);
}
void GWnd::OnIdle()
{
	
//	Invalidate();
}
void GWnd::UpdateCollission()
{
	if(sprProc==NULL)
		return;
	int collCount=0;
	static vector<int>collArr;
	collArr.resize(sprites.size());
	int i,j;
	for(i=0;i<sprites.size();i++)
	{
		if(sprites[i].visible&&sprites[i].collidable)
		{
			collArr[collCount]=i;
			collCount++;
		}
	}
	//now we have an array of only visible sprites
	for(i=0;i<collCount;i++)
	{
		for(j=i+1;j<collCount;j++)
		{
			if(IsCollission(collArr[i],collArr[j]))
			{
				sprProc(collArr[i],collArr[j],NULL);
			}
		}
	}
}
int GWnd::IsCollission(int i,int j)
{
	RECT d;
	RECT r=sprites[i].GetRect();
	RECT r2=sprites[j].GetRect();
	return IntersectRect(&d,&r,&r2);
}
void GWnd::OnCreate(void)
{
	appRunning=true;
	CreateThread(0,0,ThreadProc,0,0,0);
	CreateThread(0,0,ThreadProc2,0,0,0);


}
void GWnd::Resize(int cx,int cy)
{
	SetWindowPos(hWnd,0,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	width = cx;
	height = cy;
}
GWnd::GWnd()
{

	sprites.resize(30);
	canDraw=false;
	memDC= NULL;
	hWnd = NULL;
	bmp = NULL;
	width = winWidth;
	height = winHeight;
	
	evCanDraw = CreateEvent(0,TRUE,FALSE,L"evCanDraw");
	ResetEvent(evCanDraw);
	
	hOldBrush =NULL;
	hOldPen = NULL;
	mxUseDC=CreateMutex(0,FALSE ,L"mxUseDC");
	InitColorTable();
	InitPenTable();
	InitBrushTable();

	default_pen=0;
	default_brush=15;

	hEvInputState = CreateEvent(0,TRUE,FALSE,L"evInputState");
	ResetEvent(hEvInputState);
	
	keyPressed=false;
	textInputState=false;
	caretVisible=false;

	rightMouseDown=leftMouseDown=false;

}
void GWnd::OnKeyDown(int virtualCode)
{
	if(textInputState)
		return;
	if(keyDownProc)
		keyDownProc(virtualCode, NULL);

	keyPressed=true;
	pressedChar=virtualCode;
}
void GWnd::OnKeyUp(int virtualCode)
{
	if(textInputState)
		return;
	if(keyUpProc)
		keyUpProc(virtualCode, NULL);

	keyPressed=false;
}

void GWnd::OnMouseMove(int x,int y)
{
	if(mouseProc!=NULL)
	{
		mouseProc(x,y,leftMouseDown,rightMouseDown,NULL);
	}
}

void GWnd::ShowCaret()
{
	/*if(!caretVisible)
	{
		CreateCaret(hWnd, (HBITMAP) 1, 0, nCaretPosX); 
		SetCaretPos(nCaretPosX, nCaretPosY ); 
		::ShowCaret(hWnd);
		caretVisible=true;
	}*/
	

}
void GWnd::HideCaret()
{
	/*::HideCaret(hWnd); 
     DestroyCaret(); 
	 caretVisible=false;*/
}
void GWnd::OnSetFocus()
{
	if(textInputState)
	{
		ShowCaret();
	
	}

}
void GWnd::OnKillFocus()
{
	HideCaret();
	
}
void GWnd::OnPaint(HDC dc, PAINTSTRUCT * ps)
{
	dc=NULL;
	dc =GetDC(hWnd);
	WaitForSingleObject(mxUseDC,4000);
	if(memDC==NULL)
	{
		PreparememDC(dc);
	}
	
	UpdateCollission();
	BitBlt(spriteLayer.memDC,0,0,width-1,height-1,memDC,0,0,SRCCOPY);
	DrawSprites();
	DrawCurrentTextInput();
	BitBlt(dc,0,0,width-1,height-1,spriteLayer.memDC ,0,0,SRCCOPY);
	
	ReleaseDC(hWnd,dc);
	ReleaseMutex(mxUseDC);
}
void GWnd::DrawCurrentTextInput()
{
	if(!textInputState)
		return;
	TextOutA(spriteLayer.memDC,textInputStartX,textInputStartY,inputBuffer,inputBufferIndex);
	MoveToEx(spriteLayer.memDC,nCaretPosX,nCaretPosY,NULL);
	LineTo(spriteLayer.memDC,nCaretPosX,nCaretPosY+dwCharY);
	::ShowCaret(hWnd);
}

void GWnd::DrawSprites()
{
	//Rectangle(spriteLayer.memDC,0,0,width,height);
	for(int i=0;i<sprites.size();i++)
	{
		if(sprites[i].visible)
		{
			sprites[i].Draw(spriteLayer.memDC);
		}
	}
}
void GWnd::CleanUp()
{
	appRunning=false;
	CloseHandle(evCanDraw);
	CloseHandle(mxUseDC);

	if(memDC)
	{
		SelectObject(memDC,hOldBmp);
		DeleteObject(bmp);
		DeleteDC(memDC);
	}
	RestoreBrush();
	RestorePen();
	for(int i=0;i<16;i++)
	{
		DeleteObject(pentable[i]);
		DeleteObject(brushtable[i]);
	}
	spriteLayer.Destroy();
	
}
void GWnd::PreparememDC(HDC dc)
{
	spriteLayer.Init(width,height,dc);
	memDC = CreateCompatibleDC(spriteLayer.memDC);
	bmp = CreateCompatibleBitmap(dc,width,height);
	hOldBmp = (HBITMAP) SelectObject(memDC,bmp);
	SelectObject(memDC,GetStockObject(WHITE_BRUSH));
	SelectObject(memDC,GetStockObject(BLACK_PEN));
	Rectangle(memDC,0,0,width,height);
	
    TEXTMETRIC tm;
	GetTextMetrics(dc, &tm); 
	dwCharY = tm.tmHeight; 
 

	
	
	if(memDC && bmp) 
		SetEvent(evCanDraw);
	
}
void GWnd::Invalidate()
{
	InvalidateRect(hWnd,NULL,TRUE);
}
void GWnd::StopDrawing()
{
	ReleaseMutex(mxUseDC);
}
void GWnd::WaitTillCanDraw()
{
	WaitForSingleObject(evCanDraw,INFINITE);
	
	WaitForSingleObject(mxUseDC,INFINITE);
}
void GWnd::InitPenTable()
{
	for(int i=0;i<16;i++)
		pentable[i]=CreatePen(PS_SOLID,1,colortable[i]);
	
}
void GWnd::InitBrushTable()
{
	for(int i=0;i<16;i++)
		brushtable[i]=CreateSolidBrush(colortable[i]);
	
}
void GWnd::InitColorTable()
{
	colortable[0]=RGB(0,0,0);
	colortable[1]=RGB(255,145,72);
	colortable[2]=RGB(36,218,36);
    colortable[3]=RGB(109,255,109);

	colortable[4]=RGB(36,36,255);
	colortable[5]=RGB(72,109,255);
	colortable[6]=RGB(182,36,36);
	colortable[7]=RGB(72,182,255);

	colortable[8]=RGB(255,36,36);
	colortable[9]=RGB(255,109,109);
	colortable[10]=RGB(218,218,36);
	colortable[11]=RGB(218,218,145);

	colortable[12]=RGB(36,145,36);
	colortable[13]=RGB(218,72,182);
	colortable[14]=RGB(182,182,182);
	colortable[15]=RGB(255,255,255);

}
void GWnd::SelectPen(int clr)
{
	if(clr==-1)
		hOldPen= (HPEN)SelectObject(memDC,GetStockObject(NULL_PEN));
	hOldPen=(HPEN) SelectObject(memDC,pentable[clr]);
}
void GWnd::RestorePen()
{
	if(hOldPen)
		SelectObject(memDC,hOldPen);
}
void GWnd::SelectBrush(int clr)
{
	if(clr==-1)
		hOldBrush= (HBRUSH)SelectObject(memDC,GetStockObject(NULL_BRUSH));
	
	else
		hOldBrush=(HBRUSH) SelectObject(memDC,brushtable[clr]);
}

void GWnd::RestoreBrush()
{
	if(hOldBrush)
		SelectObject(memDC,hOldBrush);
}
void GWnd::SelectTextColor(int clr)
{
	if(clr==-1)
		SetTextColor(memDC,RGB(0,0,0));
	else
		SetTextColor(memDC,colortable[clr]);
	SetBkColor(memDC,RGB(255,255,255));
}
void text_out(char *txt,int x,int y,int clr)
{
	gWnd->WaitTillCanDraw();
	
	gWnd->SelectTextColor(clr);
	TextOutA(gWnd->memDC,x,y,txt,(int)strlen(txt));

	
	gWnd->StopDrawing();
}

void text_out(char *txt,int x,int y)
{
	text_out(txt,x,y,-1);
}

void text_out(LPCWSTR txt,int x,int y,int clr)
{
	gWnd->WaitTillCanDraw();
	
	gWnd->SelectTextColor(clr);
	TextOut(gWnd->memDC,x,y,txt,(int)wcslen(txt));

	
	gWnd->StopDrawing();
}

void text_out(LPCWSTR txt,int x,int y)
{
	text_out(txt,x,y,-1);
}

void draw_line(int x1,int y1,int x2,int y2)
{
	draw_line(x1,y1,x2,y2,gWnd->default_pen);
}
void draw_line(int x1,int y1,int x2,int y2,int clr)
{
	gWnd->WaitTillCanDraw();
	gWnd->SelectPen(clr);
	MoveToEx(gWnd->memDC,x1,y1,NULL);
	LineTo(gWnd->memDC,x2,y2);
	gWnd->RestorePen();
	gWnd->StopDrawing();
	

}
void draw_ellipse(int x1,int y1,int w,int h,int penClr,int brushClr)
{

	gWnd->WaitTillCanDraw();
	gWnd->SelectPen(penClr);
	gWnd->SelectBrush(-1);//null brush
	Ellipse(gWnd->memDC,x1,y1,x1+w,y1+h);
	gWnd->RestorePen();
	gWnd->RestoreBrush();
	gWnd->StopDrawing();

}
void draw_rect(int x1,int y1,int w,int h)
{
	draw_rect(x1,y1,w,h,gWnd->default_pen,gWnd->default_brush);

}
void draw_rect(int x1,int y1,int w,int h,int penClr,int brushClr)
{

	gWnd->WaitTillCanDraw();
	gWnd->SelectPen(penClr);
	gWnd->SelectBrush(-1);//null brush
	Rectangle(gWnd->memDC,x1,y1,x1+w,y1+h);
	gWnd->RestorePen();
	gWnd->RestoreBrush();
	gWnd->StopDrawing();

}
void draw_ellipse(int x1,int y1,int w,int h)
{
	draw_ellipse(x1,y1,w,h,gWnd->default_pen,gWnd->default_brush);

}
void fill_ellipse(int x1,int y1,int w,int h,int penClr,int brushClr)
{

	gWnd->WaitTillCanDraw();
	gWnd->SelectPen(penClr);
	gWnd->SelectBrush(brushClr);
	Ellipse(gWnd->memDC,x1,y1,x1+w,y1+h);
	gWnd->RestorePen();
	gWnd->RestoreBrush();
	gWnd->StopDrawing();

}
void fill_rect(int x1,int y1,int w,int h)
{
	fill_rect(x1,y1,w,h,gWnd->default_pen,gWnd->default_brush);

}
void fill_rect(int x1,int y1,int w,int h,int penClr,int brushClr)
{

	gWnd->WaitTillCanDraw();
	gWnd->SelectPen(penClr);
	gWnd->SelectBrush(brushClr);
	Rectangle(gWnd->memDC,x1,y1,x1+w,y1+h);
	gWnd->RestorePen();
	gWnd->RestoreBrush();
	gWnd->StopDrawing();

}
void fill_ellipse(int x1,int y1,int w,int h)
{
	fill_ellipse(x1,y1,w,h,gWnd->default_pen,gWnd->default_brush);

}
void color(int pen,int brush)
{
	gWnd->default_pen=pen;
	gWnd->default_brush=brush;
}
void pen_color(int pen)
{
	gWnd->default_pen=pen;
}
void brush_color(int brush)
{
	gWnd->default_brush=brush;
}

void wait(int millisecs)
{
	gWnd->WaitTillCanDraw();
	Sleep(millisecs);
	gWnd->StopDrawing();
}


Sprite::Sprite()
{
	bmp=NULL;
	bmpMask=NULL;
	pal=NULL;
	visible=0;
	collidable=1;
}
void Sprite::LoadBmp(LPCWSTR fname)
{
	LoadBitmapFromBMPFile(fname,&bmp,&pal,w,h);
	bmpMask=CreateBitmapMask(0,bmp,RGB(255,255,255));
}
void Sprite::CleanUp()
{
	if(bmp)
		DeleteObject(bmp);
	if(bmpMask)
		DeleteObject(bmpMask);
}
int Sprite::isHit(Sprite &spr2)
{
	return 0;
}
RECT Sprite::GetRect()
{
	RECT r;
	r.left=x;
	r.top=y;
	r.right=x+w;
	r.bottom=y+h;
	return r;
}
void Sprite::Draw(HDC dc)
{
	DrawBmp(dc,bmpMask,NULL,x,y,SRCAND);
	DrawBmp(dc,bmp,NULL,x,y,SRCPAINT);
}

void Sprite::DrawNoTransparency(HDC dc)
{
	
	DrawBmp(dc,bmp,NULL,x,y,SRCCOPY);
}
void Sprite::DrawNoTransparency(HDC dc,int newX,int newY)
{
	//DrawBmp(dc,bmp,NULL,newX,newY,SRCPAINT);
	DrawBmp(dc,bmpMask,NULL,newX,newY,SRCAND);
	DrawBmp(dc,bmp,NULL,newX,newY,SRCPAINT);
}
void load_sprite(char *fname,int spriteNum)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	string str(fname);
	wstring str2 = s2ws(str);
	gWnd->sprites[spriteNum].LoadBmp(str2.c_str());
}

void put_sprite(int spriteNum,int x,int y)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	gWnd->WaitTillCanDraw();
	
	gWnd->sprites[spriteNum].visible=1;
	gWnd->sprites[spriteNum].x=x;
	gWnd->sprites[spriteNum].y=y;

	gWnd->StopDrawing();


}
void copy_sprite_image(int spriteNum,int x,int y)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	gWnd->WaitTillCanDraw();
	gWnd->sprites[spriteNum].DrawNoTransparency(gWnd->memDC,x,y);
	gWnd->StopDrawing();

}
void get_sprite_pos(int spriteNum,int &x,int &y)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	
	x=gWnd->sprites[spriteNum].x;
	y=gWnd->sprites[spriteNum].y;

}
void get_sprite_size(int spriteNum,int &w,int &h)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	
	w=gWnd->sprites[spriteNum].w;
	h=gWnd->sprites[spriteNum].h;

}
void hide_sprite(int spriteNum,int x,int y)
{
	if(spriteNum<0||spriteNum>=gWnd->sprites.size())
		error("Bad Sprite Number");
	gWnd->sprites[spriteNum].visible=0;
}

int make_extra_sprite()
{
	gWnd->sprites.push_back(Sprite());
	return gWnd->sprites.size() - 1;
}

HBITMAP CreateBitmapMask(HDC dc,HBITMAP hbmColour, COLORREF crTransparent)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem = CreateCompatibleDC(dc);
    hdcMem2 = CreateCompatibleDC(dc);

    SelectObject(hdcMem, hbmColour);
    SelectObject(hdcMem2, hbmMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}
 BOOL LoadBitmapFromBMPFile( LPCWSTR szFileName, HBITMAP *phBitmap,
   HPALETTE *phPalette,int &w,int &h )
   {

   BITMAP  bm;

   *phBitmap = NULL;
   *phPalette = NULL;

   // Use LoadImage() to get the image loaded into a DIBSection
   *phBitmap = (HBITMAP)LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0,
               LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
   if( *phBitmap == NULL )
     return FALSE;

   // Get the color depth of the DIBSection
   GetObject(*phBitmap, sizeof(BITMAP), &bm );
   w=bm.bmWidth;
   h=bm.bmHeight;
   // If the DIBSection is 256 color or less, it has a color table
   if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
   {
   HDC           hMemDC;
   HBITMAP       hOldBitmap;
   RGBQUAD       rgb[256];
   LPLOGPALETTE  pLogPal;
   WORD          i;

   // Create a memory DC and select the DIBSection into it
   hMemDC = CreateCompatibleDC( NULL );
   hOldBitmap = (HBITMAP)SelectObject( hMemDC, *phBitmap );
   // Get the DIBSection's color table
   GetDIBColorTable( hMemDC, 0, 256, rgb );
   // Create a palette from the color tabl
   pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
   pLogPal->palVersion = 0x300;
   pLogPal->palNumEntries = 256;
   for(i=0;i<256;i++)
   {
     pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
     pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
     pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
     pLogPal->palPalEntry[i].peFlags = 0;
   }
   *phPalette = CreatePalette( pLogPal );
   // Clean up
   free( pLogPal );
   SelectObject( hMemDC, hOldBitmap );
   DeleteDC( hMemDC );
   }
   else   // It has no color table, so use a halftone palette
   {
   HDC    hRefDC;

   hRefDC = GetDC( NULL );
   *phPalette = CreateHalftonePalette( hRefDC );
   ReleaseDC( NULL, hRefDC );
   }
   return TRUE;

   }
   void DrawBmp(HDC hDC,HBITMAP hBitmap,HPALETTE hPalette,int x,int y,DWORD rop)
   {
	  BITMAP bm;
	  HDC hMemDC;
	  HBITMAP hOldBitmap;
	  HPALETTE hOldPalette;
	  GetObject( hBitmap, sizeof(BITMAP), &bm );
      hMemDC = CreateCompatibleDC( hDC );
      hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
      hOldPalette = SelectPalette( hDC, hPalette, FALSE );
      RealizePalette( hDC );

      BitBlt( hDC, x,y, bm.bmWidth, bm.bmHeight,
              hMemDC, 0, 0, rop );

      SelectObject( hMemDC, hOldBitmap );
      SelectPalette( hDC, hOldPalette, FALSE );
	  DeleteDC(hMemDC);

   
   }
void Buffer::Init(int _w,int _h,HDC initDc)
{
	memDC=CreateCompatibleDC(initDc);
	bmp=CreateCompatibleBitmap(initDc,_w,_h);
	oldbmp=(HBITMAP)SelectObject(memDC,bmp);
	SelectObject(memDC,GetStockObject(WHITE_BRUSH));
	SelectObject(memDC,GetStockObject(BLACK_PEN));
	w=_w;
	h=_h;
}
void Buffer::Destroy()
{
	SelectObject(memDC,oldbmp);
	DeleteObject(bmp);
	DeleteDC(memDC);
}
void Buffer::Blit(HDC dc,int x,int y)
{
}
void register_sprite_proc(SpriteProc p)
{
	gWnd->sprProc = p;
}
void disable_sprite_proc()
{
	register_sprite_proc(NULL);
}
void disable_sprite_proc(int spr)
{
	gWnd->sprites[spr].collidable=0;
}
void enable_sprite_proc(int spr)
{
	gWnd->sprites[spr].collidable=1;
}
int inkey()
{
	if(!gWnd->keyPressed)
	{
		return 0;
	}
	else
	{
		gWnd->keyPressed=false;	
		return gWnd->pressedChar;
	}
}

void register_mouse_proc(MouseProc proc)
{
	gWnd->mouseProc=proc;
}

void register_key_down_proc(KeyProc proc)
{
	gWnd->keyDownProc =proc;
}

void register_key_up_proc(KeyProc proc)
{
	gWnd->keyUpProc =proc;
}

void GWnd::EnterInputState()
{
	inputBufferIndex=0;
	ResetEvent(hEvInputState);
	
	textInputState=true;
	
}
void GWnd::WaitForEndInput()
{
	WaitForSingleObject(hEvInputState,INFINITE);
	textInputState=false;
	inputBuffer[inputBufferIndex]='\0';
}
void GWnd::OnChar(int ch)
{
	char lastChar;
	int nCharWidth;
	if(textInputState)
	{
		
		// check if current location is close enough to the
		// end of the buffer that a buffer overflow may
		// occur. If so, add null and display contents. 
		if (inputBufferIndex > textBuffersize-2)
		{
			MessageBeep(0);
			return;
		} 
		switch (ch) 
            { 
                case 0x08:  // backspace 
				
					 lastChar= inputBuffer[--inputBufferIndex];
					inputBuffer[inputBufferIndex]='\0';
					GetCharWidth32(memDC, (UINT) ch, (UINT) ch, 
                        &nCharWidth); 
					
                    nCaretPosX -= nCharWidth; 
					break;
                case 0x0A:  // linefeed 
                case 0x1B:  // escape 
                    MessageBeep((UINT) -1); 
                    return ; 
 
                case 0x09:  // tab 
 
                    // Convert tabs to four consecutive spaces. 
 
                    for (int i = 0; i < 4; i++) 
						SendMessage(hWnd, WM_CHAR, 0x20, 0); 
                    return ; 
 
                case 0x0D:  // carriage return 
 
					SetEvent(hEvInputState);
                    break; 
 
                default:    // displayable character 
 
                   
					::HideCaret(hWnd); 
				
                    // Retrieve the character's width and output 
                    // the character. 
 
                    
                    GetCharWidth32(memDC, (UINT) ch, (UINT) ch, 
                        &nCharWidth); 
				
 
                    // Store the character in the buffer.
 
					inputBuffer[inputBufferIndex++] = ch; 
 
                    // Calculate the new horizontal position of the 
                    // caret. If the position exceeds the maximum, 
                    // insert a carriage return and move the caret 
                    // to the beginning of the next line. 
 
                    nCaretPosX += nCharWidth; 
                  
                  
					::ShowCaret(hWnd); 
                    break; 
            } 
            SetCaretPos(nCaretPosX, nCaretPosY ); 
			OnPaint(NULL,NULL);

	
	}//if textInputState
}
int GWnd::GetStrWidth(char *str)
{
	int nCharWidth,ret=0;
	char *tmp=str;
	while(*tmp)
	{
		char ch=*tmp;
		GetCharWidth32(memDC, (UINT) ch, (UINT) ch, 
                        &nCharWidth); 

		ret+=nCharWidth;
		tmp++;
	}
	return ret;
	
}
char *input(char *prompt,int x,int y)
{
	text_out(prompt,x,y);
	x+=gWnd->GetStrWidth(prompt);
	gWnd->OnPaint(NULL,NULL);

	gWnd->nCaretPosX=x;
	gWnd->nCaretPosY=y;
	gWnd->textInputStartX=x;
	gWnd->textInputStartY=y;
	
	gWnd->EnterInputState();
	gWnd->WaitForEndInput();

	text_out(gWnd->inputBuffer,x,y);
	return gWnd->inputBuffer;
}
template<class T> void gswap(T &x,T &y)
{
	T tmp=x;
	x=y;
	y=tmp;

}
void swap_sprite_images(int spr1,int spr2)
{
	Sprite *s1=&(gWnd->sprites[spr1]);
	Sprite *s2=&(gWnd->sprites[spr2]);

	s1->SwapBitmapWith(s2);


}
void Sprite::SwapBitmapWith(Sprite *s)
{
	gswap(bmp,s->bmp);
	gswap(bmpMask,s->bmpMask);
	gswap(pal,s->pal);

	gswap(w,s->w);
	gswap(h,s->h);

}
Timer::Timer()
{
	sumDelays=0;
	elapsedFrames=0;
	lastTime=0;
}
void Timer::Update(long curTime)
{
	int delta=curTime-lastTime;
	lastTime= curTime;
	if(delta>minDelay)
		timeTillNextFrame=0;
	else
		timeTillNextFrame=minDelay-delta;
	elapsedFrames++;
	sumDelays+=delta;
	avgFrameRate=sumDelays/elapsedFrames;
	
	
}