#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define null 0
#define max 100
#define WINDING 2
#define MAX_LOADSTRING 100
#define ID_INPUTEDIT 1001
#define ID_OK 1002

bool closing = false;
HWND dialogHandle = 0;

typedef struct date /*定义字符栈*/
{
char sk[max];
int top;
}seqstack;

typedef struct da /*定义数据栈*/
{
int sk[max];
int top;
}stack;

struct PIECE{
	int num;  //序号
	int color;//0红1蓝
	POINT point;//位置
};

POINT points[] = {{25 , 375}, {125, 325}, {125, 425}, {225, 275}, {225, 375}, {225, 475},
          {325, 225}, {325, 325}, {325, 425}, {325, 525}, {425, 175}, {425, 275},
          {425, 375}, {425, 475}, {425, 575}, {525, 125}, {525, 225}, {525, 325},
          {525, 425}, {525, 525}, {525, 625}, {625,  75}, {625, 175}, {625, 275},
          {625, 375}, {625, 475}, {625, 575}, {625, 675}, {725,  25}, {725, 125},
          {725, 225}, {725, 325}, {725, 425}, {725, 525}, {725, 625}, {725, 725},
          {825,  75}, {825, 175}, {825, 275}, {825, 375}, {825, 475}, {825, 575},
          {825, 675}, {925, 125}, {925, 225}, {925, 325}, {925, 425}, {925, 525},
          {925, 625}, {1025, 175}, {1025, 275}, {1025, 375}, {1025, 475}, {1025, 575},
          {1125, 225}, {1125, 325}, {1125, 425}, {1125, 525},{1225, 275}, {1225, 375},
          {1225, 475}, {1325, 325}, {1325, 425}, {1425, 375}};
POINT apt1[] = {{25, 375},{325, 225}, {325, 525}};
POINT apt2[] = {{1425, 375},{1125, 225}, {1125, 525}};

int score[2] = {0, 0};


//判断是否结束
bool working = true;
//判断顺序
bool order = true;
//下棋按键
bool running = true;

//初始位置
PIECE red_pieces[] = {{0, 0, {25, 375}}, {1, 0, {325, 525}}, {2, 0, {325, 425}}, {3, 0, {325, 325}},
{4, 0, {325, 225}}, {5, 0, {225, 275}}, {6, 0, {225, 375}}, {7, 0, {225, 475}}, {8, 0, {125, 425}}, 
{9, 0, {125, 325}}};
PIECE o_red_pieces[] = {{0, 0, {25, 375}}, {1, 0, {325, 525}}, {2, 0, {325, 425}}, {3, 0, {325, 325}},
{4, 0, {325, 225}}, {5, 0, {225, 275}}, {6, 0, {225, 375}}, {7, 0, {225, 475}}, {8, 0, {125, 425}}, 
{9, 0, {125, 325}}};
PIECE blue_pieces[] = {{0, 1, {1425, 375}}, {1, 1, {1125, 225}}, {2, 1, {1125, 325}}, {3, 1, {1125, 425}},
{4, 1, {1125, 525}}, {5, 1, {1225, 475}}, {6, 1, {1225, 375}}, {7, 1, {1225, 275}}, {8, 1, {1325, 325}}, 
{9, 1, {1325, 425}}};
PIECE o_blue_pieces[] = {{0, 1, {1425, 375}}, {1, 1, {1125, 225}}, {2, 1, {1125, 325}}, {3, 1, {1125, 425}},
{4, 1, {1125, 525}}, {5, 1, {1225, 475}}, {6, 1, {1225, 375}}, {7, 1, {1225, 275}}, {8, 1, {1325, 325}}, 
{9, 1, {1325, 425}}};
POINT red_points[] = {{25, 375}, {325, 525}, {325, 425}, {325, 325}, {325, 225}, {225, 275}, {225, 375}, 
{225, 475}, {125, 425}, {125, 325}};
POINT blue_points[] = {{1425, 375}, {1125, 225}, {1125, 325}, {1125, 425}, {1125, 525}, {1225, 475}, 
{1225, 375}, {1225, 275}, {1325, 325}, {1325, 425}};

//存储当前棋子状态
PIECE state1 = {0, 0, {0, 0}};
PIECE state2 = {0, 1, {0, 0}};
PIECE state3 = {0, 0, {0, 0}};
PIECE state4 = {0, 1, {0, 0}};

int f = 1;
int back_move = 0;//悔棋标识
int illegal = 0;  //非法标记

char *inputbox(char *hint);
LRESULT CALLBACK inputBoxWndProc(HWND wndHandle, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Draw_chessboard(HDC hdc, HBRUSH hBrush, HPEN hPen);
void Init_number(HDC hdc);
void Draw_a_piece(HDC hdc, HBRUSH hBrush, int color, int num, int x, int y);
void Draw_board_with_pieces(HDC hdc, HBRUSH hBrush, HPEN hPen);
int find(int x, int y);
bool Judge_position(int x, int y);
bool move_rule(int cur_x, int cur_y, int x, int y);
bool adjacent_rule(int cur_x, int cur_y, int x, int y);
bool single_span(int value, int cur_x, int cut_y, int x, int y);
void spush(stack *s,int x);
void push(seqstack *s,char x);
int spop(stack *s ,int c);
char pop(seqstack *s ,char c);
int pp(char c);
int yunsuan(int a,char b,int c);
char bijiao(seqstack *f,char c);
int calculate(char * str);
void BubbleSort(int *a, int len);
bool whowin();
int find_score(int x, int y);
void reset();
void scores();
void tip(HDC hdc, HBRUSH hBrush, HPEN hPen);
char *remove_blank(char * str); 

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR szCmdLine,
    int iCmdShow
){
    static TCHAR szClassName[] = TEXT("HelloWin");  //窗口类名
    HWND     hwnd;      //窗口句柄
    MSG      msg;       //消息
    WNDCLASS wndclass;  //窗口类

    /**********第①步：注册窗口类**********/
    //为窗口类的各个字段赋值
    wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
    wndclass.lpfnWndProc  = WndProc;  //窗口过程
    wndclass.cbClsExtra   = 0;  //暂时不需要理解
    wndclass.cbWndExtra   = 0;  //暂时不需要理解
    wndclass.hInstance    = hInstance;  //当前窗口句柄
    wndclass.hIcon        = LoadIcon (NULL, IDI_APPLICATION);  //窗口图标
    wndclass.hCursor      = LoadCursor (NULL, IDC_ARROW);  //鼠标样式
    wndclass.hbrBackground= NULL;  //窗口背景画刷
    wndclass.lpszMenuName = NULL ;  //窗口菜单
    wndclass.lpszClassName= szClassName;  //窗口类名

    //注册窗口
    RegisterClass(&wndclass);

    /*****第②步：创建窗口(并让窗口显示出来)*****/
    hwnd = CreateWindow(
        szClassName,  //窗口类的名字
        TEXT("国际数棋"),  //窗口标题（出现在标题栏）
        WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,  //窗口风格
        CW_USEDEFAULT,  //初始化时x轴的位置
        CW_USEDEFAULT,  //初始化时y轴的位置
        1469,  //窗口宽度
        800,  //窗口高度
        NULL,  //父窗口句柄
        NULL,  //窗口菜单句柄
        hInstance,  //当前窗口的句柄
        NULL  //不使用该值
    );
	
    //显示窗口
    ShowWindow (hwnd, iCmdShow);
    //更新（绘制）窗口
    UpdateWindow (hwnd);

    /**********第③步：消息循环**********/
    while( GetMessage(&msg, NULL, 0, 0) ){
        TranslateMessage(&msg);  //翻译消息
        DispatchMessage (&msg);  //分派消息
    }

    return msg.wParam;
}

/**********第④步：窗口过程**********/


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    HDC         hdc,    dcMem;  //设备环境句柄
    PAINTSTRUCT ps;
    RECT        rect;
	HBITMAP     bmpMem;
	static HWND hwndInput;
    switch (message){
        //窗口绘制消息
        case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			//依附窗口DC创建兼容内存
			dcMem = CreateCompatibleDC(hdc);
			//创建兼容位图
			bmpMem = CreateCompatibleBitmap(hdc, 1469, 800);
			//将位图选择进内存DC
			SelectObject(dcMem, bmpMem);	
			HBRUSH hBrush;
			HPEN hPen;
			//白色填充内存DC客户区
			hBrush = CreateSolidBrush(RGB(255,255,255));
			SelectObject(hdc,hBrush);
			Rectangle(dcMem, 0, 0, 1469, 800);

			//绘图
			HBITMAP bmp;
			bmp = (HBITMAP)LoadImage(NULL, TEXT("background.bmp"), IMAGE_BITMAP, 1469, 800, LR_LOADFROMFILE);
			HDC hbuf;
			hbuf = CreateCompatibleDC(NULL);
			SelectObject(dcMem, bmp);
			BitBlt(dcMem, 0, 0, 1469, 800, hbuf, 0, 0,SRCCOPY);
			DeleteObject(bmp);
			DeleteObject(hbuf);


			hBrush = CreateSolidBrush(RGB(0,0,0));
			hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));

			SelectObject(dcMem,hBrush);
			SelectObject(dcMem,hPen);
			
			Draw_board_with_pieces(dcMem, hBrush, hPen);
			DeleteObject(hBrush);
			DeleteObject(hPen);
			if(0 != state1.point.x && 0 != state1.point.y){
				//绿色框，透明画刷
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 255));
				SelectObject(dcMem, hBrush);
				SelectObject(dcMem, hPen);
				Rectangle(dcMem, state1.point.x - 27, state1.point.y - 27, state1.point.x + 27, state1.point.y + 27);
				DeleteObject(hBrush);
				DeleteObject(hPen);
			}
			if(0 != state2.point.x && 0 != state2.point.y){
				//黄色框，透明画刷
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 255));
				SelectObject(dcMem, hBrush);
				SelectObject(dcMem, hPen);
				Rectangle(dcMem, state2.point.x - 27, state2.point.y - 27, state2.point.x + 27, state2.point.y + 27);
				DeleteObject(hBrush);
				DeleteObject(hPen);
			}

			if(working){
				HFONT hFont;
				hFont = CreateFont(42, 0, 0, 0, 800, false, false, false, 
						ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, 
						PROOF_QUALITY, FF_MODERN, "Arial");
				SelectObject(dcMem, hFont);
				SetBkMode(dcMem, TRANSPARENT);
				if(order){
					SetTextColor(dcMem, RGB(255,0,0));
					TextOut(dcMem, 1200, 600, "红棋落子", 8);
					tip(dcMem, hBrush, hPen);
				}
				else{
					SetTextColor(dcMem, RGB(0,0,255));
					TextOut(dcMem, 1200, 600, "蓝棋落子", 8);
					tip(dcMem, hBrush, hPen);
				}
				DeleteObject(hFont);
			}
			//将内存DC  dcMem上的图像拷贝到前台DC hdc上
			BitBlt(hdc, 0, 0, 1469, 800, dcMem, 0, 0, SRCCOPY);
			DeleteObject(bmpMem);
			DeleteObject(dcMem);
			DeleteObject(hBrush);
			DeleteObject(hPen);

			EndPaint (hwnd, &ps) ;
			return 0;

		//鼠标点击消息
		case WM_LBUTTONDOWN:
			//获取当前鼠标位置
			POINT pt;//鼠标点击位置
			int px, py;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			px = pt.x;
			py = pt.y;
			if(working){
				
				if(running){	
					for(int i = 0; i <= 9; i++){
						if(pow(px - red_pieces[i].point.x, 2) + pow(py - red_pieces[i].point.y, 2) < pow(25, 2)){
							PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
							state1.num = i;
							state1.point = red_pieces[i].point;
							state1.color = red_pieces[i].color;
						}
					}
					for(i = 0; i <= 9; i++){
						if(pow(px - blue_pieces[i].point.x, 2) + pow(py - blue_pieces[i].point.y, 2) < pow(25, 2)){
							PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
							state2.num = i;
							state2.point = blue_pieces[i].point;
							state2.color = blue_pieces[i].color;
						}
					}
					if(state1.point.x != 0 && state1.point.y != 0){
						state3 = state1;
						running = !running;
					}
					else if(state2.point.x != 0 && state2.point.y != 0){
						state4 = state2;
						running = !running;
					}
				}
				//第二次点击
				else{
					px = pt.x / 50 * 50 + 25; 
					py = pt.y / 50 * 50 + 25;
					int i;
					for(i = 0; i < 64; i++){
						if(px == points[i].x && py == points[i].y){
							//红
							if(state1.point.x != 0 || state1.point.y != 0){
								//InvalidateRect(hwnd, NULL, true);
								if(Judge_position(px, py)){
									PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
									red_pieces[state3.num].point = state3.point;
									red_pieces[state3.num].color = state3.color;
								}
								else{
									if((move_rule(state1.point.x, state1.point.y, px, py) || 
									   adjacent_rule(state1.point.x, state1.point.y, px, py)) && 
									   order == true){
										PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
										red_pieces[state1.num].point.x = px; 
										red_pieces[state1.num].point.y = py;
										
										order = !order;
										back_move = 1;
										
									}
									//单跨
									else if(order == true && single_span(state1.num, state1.point.x, state1.point.y, px, py) && 0 == illegal){

										PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
										red_pieces[state1.num].point.x = px; 
										red_pieces[state1.num].point.y = py;
										order = !order;
										back_move = 1;
									}
									if(1 == illegal){
										MessageBox(hwnd, TEXT("非法输入"), TEXT("警告"), MB_OK|MB_ICONWARNING);
										illegal = 0;
									}
									//else
								}
								state1.color = 0;
								state1.num = 0;
								state1.point.x = 0;
								state1.point.y = 0;
								running = !running;
							}
							//蓝
							else if(state2.point.x != 0 || state2.point.y != 0){
								if(Judge_position(px, py)){
									PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
									blue_pieces[state4.num].point = state4.point;
									blue_pieces[state4.num].color = state4.color;
								}
								else{
									if((move_rule(state2.point.x, state2.point.y, px, py) || 
									   adjacent_rule(state2.point.x, state2.point.y, px, py)) && 
									   order == false){
					
										PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
										blue_pieces[state2.num].point.x = px; 
										blue_pieces[state2.num].point.y = py;
										order = !order;
										back_move = 1;
									}
									else if(order == false && single_span(state2.num, state2.point.x, state2.point.y, px, py) && 0 == illegal){

										PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
										blue_pieces[state2.num].point.x = px; 
										blue_pieces[state2.num].point.y = py;
										order = !order;
										back_move = 1;
									}
									if(1 == illegal){
										MessageBox(hwnd, TEXT("非法输入"), TEXT("警告"), MB_OK|MB_ICONWARNING);
										illegal = 0;
									}
									//else
								}
								state2.color = 1;
								state2.num = 0;
								state2.point.x = 0;
								state2.point.y = 0;
								running = !running;
							}
							break;
						}
					}
					if (i >= 64){
						if(state1.point.x != 0 && state1.point.y != 0){
							red_pieces[state1.num].color = state3.color;
							red_pieces[state1.num].point = state3.point;
							state1.color = 0;
							state1.num = 0;
							state1.point.x = 0;
							state1.point.y = 0;
							running = !running;
						}
						else if(state2.point.x != 0 && state2.point.y != 0){
							blue_pieces[state2.num].color = state4.color;
							blue_pieces[state2.num].point = state4.point;
							state2.color = 1;
							state2.num = 0;
							state2.point.x = 0;
							state2.point.y = 0;
							running = !running;
						}
					}
				}
				//重新开始
				if(px > 50 && px < 200 && py > 50 && py < 100){
					PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
					int select = MessageBox(hwnd, TEXT("确定重新开始?"), TEXT("询问"),
							MB_YESNO);
					if(IDYES == select){
						reset();
					}
				}
				//结束游戏
				if(px > 50 && px < 200 && py > 100 && py < 150){
					PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
					if(!whowin()){
						int select = MessageBox(hwnd, TEXT("还未下完，确定结束?"), TEXT("询问"),
							MB_YESNO);
						if(IDYES == select){
							exit(0);
						}
					}
					//********************
					else{
						scores();
						if(score[0] > score[1]){
							MessageBox(hwnd, TEXT("恭喜红方胜利"), TEXT("结束"), MB_OK);
							reset();
						}
						else{
							MessageBox(hwnd, TEXT("恭喜蓝方胜利"), TEXT("结束"), MB_OK);
							reset();
						}
					}
				}
				//认输
				else if(px > 1200 && px < 1350 && py > 100 && py < 150){
					PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
					if(order == true){
						MessageBox(hwnd, TEXT("恭喜蓝方胜利"), TEXT("结束"), MB_OK);
						reset();
					}
					else{
						MessageBox(hwnd, TEXT("恭喜红方胜利"), TEXT("结束"), MB_OK);
						reset();
					}
				}
				//悔棋
				else if(px > 1200 && px < 1350 && py > 50 && py < 100){
					PlaySound(TEXT("1.wav"), NULL, SND_FILENAME|SND_ASYNC);
					if(1 == back_move){
						if (!order){
							red_pieces[state3.num].point = state3.point;
						}
						else{
							blue_pieces[state4.num].point = state4.point;
						}
						order = !order;
						back_move = 2;
					}
					else{
						MessageBox(hwnd, TEXT("不能连续悔棋"), TEXT("提示"), MB_OK);
					}
				}
			}
			InvalidateRect(hwnd, NULL, true);
			
			return 0;
        //窗口销毁消息
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam) ;
}

//绘制棋盘
void Draw_chessboard(HDC hdc, HBRUSH hBrush, HPEN hPen)
{
	SetPolyFillMode(hdc, WINDING);
	hBrush = CreateSolidBrush(RGB(255,0,0));//红色

	SelectObject(hdc,hBrush);
	Polygon(hdc, apt1, 3);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0,0,255));//蓝色
	SelectObject(hdc,hBrush);
	Polygon(hdc, apt2, 3);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdc,hBrush);
	for(int i = 0; i <= 15; i++){
		if (i <= 7){
			MoveToEx(hdc, i * 100 + 25, 350 - i * 50 + 25, NULL);
			LineTo(hdc, i * 100 + 25, 350 + i * 50 + 25);

			MoveToEx(hdc, i * 100 + 25, 350 + i * 50 + 25, NULL);
			LineTo(hdc, i * 100 + 725, i * 50 + 25);

			MoveToEx(hdc, i * 100 + 25, 350 - i * 50 + 25, NULL);
			LineTo(hdc, i * 100 + 725, 700 - i * 50 + 25);

			
			for(int j = 0; j < i + 1; j++){
				if(i < 4){
					Ellipse(hdc, i * 100, 350 - i * 50 + j * 100, i * 100 + 50, 350 - i * 50 + j * 100 + 50);
				}
				else{
					Ellipse(hdc, i * 100, 350 - i * 50 + j * 100, i * 100 + 50, 350 - i * 50 + j * 100 + 50);

				}
			}

		}
		else{
			int n = 14 - i;
			MoveToEx(hdc, i * 100 + 25, 350 - n * 50 + 25, NULL);
			LineTo(hdc, i * 100 + 25, 350 + n * 50 + 25);

			for (int j = 14 - i; j >= 0; j--){
				if(i > 10){
					Ellipse(hdc, i * 100, (i - 7) * 50 + j * 100, i * 100 + 50, (i - 7) * 50 + j * 100 + 50);
				}
				else{
					Ellipse(hdc, i * 100, (i - 7) * 50 + j * 100, i * 100 + 50, (i - 7) * 50 + j * 100 + 50);	
				}
			}
		}
		DeleteObject(hBrush);
		
	}
	Init_number(hdc);

	//选项按钮
	Rectangle(hdc, 50, 50, 200,100);
	Rectangle(hdc, 50, 100, 200,150);
	Rectangle(hdc, 1200, 50, 1350,100);
	Rectangle(hdc, 1200, 100, 1350,150);
	HFONT hFont;
	hFont = CreateFont(42, 0, 0, 0, 800, false, false, false, 
		ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, 
		PROOF_QUALITY, FF_MODERN, "Arial");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	//选项字
	SetTextColor(hdc, RGB(0,0,0));
	TextOut(hdc, 50, 53, "重新开始", 8);
	TextOut(hdc, 50, 103, "结束游戏", 8);
	TextOut(hdc, 1237, 53, "悔棋", 4);
	TextOut(hdc, 1237, 103, "认输", 4);
	DeleteObject(hFont);
}

void Init_number(HDC hdc)
{
	HFONT hFont;
	hFont = CreateFont(36, 0, 0, 0, 800, false, false, false, 
		ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, 
		PROOF_QUALITY, FF_MODERN, "Arial");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	//棋盘字
	SetTextColor(hdc, RGB(0,0,0));
	TextOut(hdc, 17, 360, "0", 1);
	TextOut(hdc, 1417, 360, "0", 1);
	TextOut(hdc, 317, 510, "1", 1);
	TextOut(hdc, 1117, 210, "1", 1);
	TextOut(hdc, 317, 410, "2", 1);
	TextOut(hdc, 1117, 310, "2", 1);
	TextOut(hdc, 317, 310, "3", 1);
	TextOut(hdc, 1117, 410, "3", 1);
	TextOut(hdc, 317, 210, "4", 1);
	TextOut(hdc, 1117, 510, "4", 1);
	TextOut(hdc, 217, 260, "5", 1);
	TextOut(hdc, 1217, 460, "5", 1);
	TextOut(hdc, 217, 360, "6", 1);
	TextOut(hdc, 1217, 360, "6", 1);
	TextOut(hdc, 217, 460, "7", 1);
	TextOut(hdc, 1217, 260, "7", 1);
	TextOut(hdc, 117, 410, "8", 1);
	TextOut(hdc, 1317, 310, "8", 1);
	TextOut(hdc, 117, 310, "9", 1);
	TextOut(hdc, 1317, 410, "9", 1);
	DeleteObject(hFont);
}

void Draw_a_piece(HDC hdc, HBRUSH hBrush, int color, int num, int x, int y)
{
	if(0 == color){
		hBrush = CreateSolidBrush(RGB(255,0,0));
	}
	if(1 == color){
		hBrush = CreateSolidBrush(RGB(0,0,255));
	}
	SelectObject(hdc,hBrush);
	Ellipse(hdc, x - 25, y - 25, x + 25, y + 25);
	DeleteObject(hBrush);
	HFONT hFont;
	hFont = CreateFont(36, 0, 0, 0, 800, false, false, false, 
		ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, 
		PROOF_QUALITY, FF_MODERN, "Arial");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255,255,255));
	char str[1];
	itoa(num, str, 10);
	TextOut(hdc, x - 8, y - 15, str, 1);
	DeleteObject(hFont);
}

void Draw_board_with_pieces(HDC hdc, HBRUSH hBrush, HPEN hPen)
{
	Draw_chessboard(hdc, hBrush, hPen);
	for(int i = 0; i <= 9; i++){
		Draw_a_piece(hdc, hBrush, red_pieces[i].color, red_pieces[i].num, 
			red_pieces[i].point.x, red_pieces[i].point.y);
	}
	for(int j = 0; j <= 9; j++){
		Draw_a_piece(hdc, hBrush, blue_pieces[j].color, blue_pieces[j].num, 
			blue_pieces[j].point.x, blue_pieces[j].point.y);
	}
}

int find(int x, int y)
{
	for(int i = 0; i <= 9; i++){
		if(red_pieces[i].point.x == x && red_pieces[i].point.y == y){
			return i;
		}
		if(blue_pieces[i].point.x == x && blue_pieces[i].point.y == y){
			return i;
		}
	}
	return -1;
}

bool Judge_position(int x, int y)
{
	for(int i = 0; i <= 9; i++){
		if(red_pieces[i].point.x == x && red_pieces[i].point.y == y){
			return true;		
		}
		if(blue_pieces[i].point.x == x && blue_pieces[i].point.y == y){
			return true;
		}
	}
	return false;
}

bool move_rule(int cur_x, int cur_y, int x, int y)
{
	if(abs(cur_x - x) <= 100 && abs(cur_y - y) <= 100){
		if(cur_x != x || cur_y != y){
			return true;
		}
	}
	return false;
}

bool adjacent_rule(int cur_x, int cur_y, int x, int y)
{
	if(Judge_position((cur_x + x) / 2, (cur_y + y) / 2) && abs(cur_x - x) <= 200){
		if(cur_x != x || cur_y != y){
			return true;
		}
	}
	return false;
}


//单跨
bool single_span(int value, int cur_x, int cur_y, int x, int y)
{
	int x1, y1, VALUE;
	int v[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int b[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	char * a;
	if(y == cur_y){
		return false;
	}
	else if(x == cur_x){
		y1 = (y - cur_y) / abs(y - cur_y);
		if(Judge_position(x, y - 100 * y1)){
			//a = getInput();
			a = remove_blank(inputbox("请输入合法的四则运算表达式"));
			if(0 == illegal){
				if(a != "error" && a != '\0'){
					VALUE = calculate(a);
					int j = 0;
					for(int i = 0; a[i] != '\0'; i++){
						if(a[i] >= '0' && a[i] <= '9'){
							b[j++] = a[i] - 48;
						}
					}
					j = 0;
					for(i = 0; i <= abs((y - cur_y) / 100 -1); i++){
						v[j++] = find(x, cur_y + (i + 1) * 100 * y1);
					}
					BubbleSort(b, 10);
					BubbleSort(v, 10);
					for(i = 0; i < 10; i++){
						if(b[i] != v[i]){
							return false;
						}
					}
					if(value == VALUE){
						return true;
					}
					return false;
				}
			}
		}
	}
	else{
		y1 = (y - cur_y) / abs(y - cur_y);
		x1 = (x - cur_x) / abs(x - cur_x);
		if(Judge_position(x - x1 * 100, y - 50 * y1) && abs(y - cur_y) / 50 == abs(x - cur_x) / 100){
			//a = getInput();
			a = remove_blank(inputbox("请输入合法的四则运算表达式"));
			if(0 == illegal){
				if(a != "error" && a != '\0'){
					VALUE = calculate(a);
					int j = 0;
					for(int i = 0; a[i] != '\0'; i++){
						if(a[i] >= '0' && a[i] <= '9'){
							b[j++] = a[i] - 48;
						}
					}
					j = 0;
					for(i = 0; i <= abs((x - cur_x) / 100) - 1; i++){
						v[j++] = find(cur_x + (i + 1) * 100 * x1, cur_y + (i + 1) * 50 * y1);
					}
					BubbleSort(b, 10);
					BubbleSort(v, 10);
					for(i = 0; i < 10; i++){
						if(b[i] != v[i]){
							return false;
						}
					}
					if(value == VALUE){
						return true;
					}
					return false;
				}
			}
		}
	}
	return false;
}

bool whowin()
{
	int i, j;
	for(i = 0; i <= 9; i++){
		for(j= 0; j <= 9; j++){
			if(red_pieces[i].point.x == blue_points[j].x && red_pieces[i].point.y == blue_points[j].y){
				break;
			}
		}
		if(j > 9){
			break;//该方未下完
		}
	}
	if(i > 9){
		return true;
	}

	for(i = 0; i <= 9; i++){
		for(j= 0; j <= 9; j++){
			if(blue_pieces[i].point.x == red_points[j].x && blue_pieces[i].point.y == red_points[j].y){
				break;
			}
		}
		if(j > 9){
			break;//该方未下完
		}
	}
	if(i > 9){
		return true;
	}
	
	return false;
}

//查找对应值
int find_score(int x, int y)
{
	if((x == 325 && y == 525) || (x == 1125 && y == 225)){
		return 1;
	}
	else if((x == 325 && y == 425) || (x == 1125 && y == 325)){
		return 2;
	}
	else if((x == 325 && y == 325) || (x == 1125 && y == 425)){
		return 3;
	}
	else if((x == 325 && y == 225) || (x == 1125 && y == 525)){
		return 4;
	}
	else if((x == 225 && y == 275) || (x == 1225 && y == 475)){
		return 5;
	}
	else if((x == 225 && y == 375) || (x == 1225 && y == 375)){
		return 6;
	}
	else if((x == 225 && y == 425) || (x == 1225 && y == 275)){
		return 7;
	}
	else if((x == 125 && y == 425) || (x == 1325 && y == 325)){
		return 8;
	}
	else if((x == 125 && y == 325) || (x == 1325 && y == 425)){
		return 9;
	}
	else{
		return 0;
	}
}

void scores()
{
	int red_score = 0 , blue_score = 0;
	
	int i, j;
	for(i = 0; i <= 9; i++){
		for(j = 0; j <= 9; j++){
			if(red_pieces[i].point.x == blue_points[j].x && red_pieces[i].point.y == blue_points[j].y){
				red_score += i * find_score(red_pieces[i].point.x, red_pieces[i].point.y);
				break;
			}
		}
	}
	for(i = 0; i <= 9; i++){
		for(j = 0; j <= 9; j++){
			if(blue_pieces[i].point.x == red_points[j].x && blue_pieces[i].point.y == red_points[j].y){
				blue_score += i * find_score(blue_pieces[i].point.x, blue_pieces[i].point.y);
				break;
			}
		}
	}
	score[0] = red_score;
	score[1] = blue_score;
}

//红蓝提示
void tip(HDC hdc, HBRUSH hBrush, HPEN hPen)
{
	hBrush = CreateSolidBrush(RGB(255,255,255));
	hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	Rectangle(hdc, 50, 600, 280,730);
	HFONT hFont;
	hFont = CreateFont(36, 0, 0, 0, 800, false, false, false, 
		ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, 
		PROOF_QUALITY, FF_MODERN, "Arial");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	//红
	scores();
	SetTextColor(hdc, RGB(255, 0, 0));
	char str[1];
	itoa(score[0], str, 10);
	TextOut(hdc, 55, 615, "红方得分", 8);
	TextOut(hdc, 205, 615, str, strlen(str));
	//蓝
	SetTextColor(hdc, RGB(0, 0, 255));
	char str1[1];
	itoa(score[1], str1, 10);
	TextOut(hdc, 55, 660, "蓝方得分", 8);
	TextOut(hdc, 205, 660, str1, strlen(str1));
	DeleteObject(hFont);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

//重新开始
void reset()
{
	for(int i = 0; i <= 9; i++){
		red_pieces[i] = o_red_pieces[i];
		blue_pieces[i] = o_blue_pieces[i];
	}
	working = true;
	order = true;
	running = true;
	
}

//字符四则运算
void push(seqstack *s,char x) /*进栈*/
{
	if(s->top==max-1)
	{
		printf("out of space.\n");
	}
	s->top++;
	s->sk[s->top]=x;
}
void spush(stack *s,int x) /*进栈*/
{
	if(s->top==max-1)
	{
		printf("sout of space.\n");
	}
	s->top++;
	s->sk[s->top]=x;
}
char pop(seqstack *s ,char c) /*出栈*/
{
	if(s->top==-1)
	{
		printf("no element.\n");
		return 0;
	}
	else
	{
		c=s->sk[s->top];
		s->top--;
		return c;
	}
}
int spop(stack *s ,int c) /*出栈*/
{
	if(s->top==-1)
	{
		illegal = 1;
		return 0;
	}
	else
	{
		c=s->sk[s->top];
		s->top--;
		return c;
	}
}
int pp(char c) /*判断字符*/
{
	switch(c)
	{
		case '#':return 0;break;
		case '+':return 2;break;
		case '-':return 2;break;
		case '*':return 3;break;
		case '/':return 3;break;
		case '(':return 1;break;
		case ')':return 4;break;
		default:return 9;break;
	}
}
int yunsuan(int a,char b,int c) /*运算*/
{
	switch(b)
	{
		case '+':return a+c;break;
		case '-':return a-c;break;
		case '*':return a*c;break;
		default:
			if(c==0 || a % c != 0){
				illegal = 1;
				return 0;
			}
		return a/c;break;
	}
}
char bijiao(seqstack *f,char c) /*比较优先级*/
{
	int m,n;
	char w;
	m=pp(c);
	w=f->sk[f->top];
	n=pp(w);
	if(n<m) return '>';
	return '<';
}

int calculate(char * str)
{
	int x,s;
	int q=0,w=0;
	int p=0; /*标示变量*/
	char ch,th='\0',d='\0';
	seqstack *h;
	h=(seqstack*)malloc(sizeof(seqstack));
	if(!h) printf("no creat");
	h->top=-1;
	stack *g;
	g=(stack*)malloc(sizeof(stack));
	if(!g) printf("no door");
	g->top=-1;
	push(h,'#');
	//ch=getchar();
	ch = *str;
	int i = 1;
	while(ch!='\0')
	{
		if(pp(ch)==9)
		{
			if(p>0)
			{
				x=spop(g,x);
				spush(g,10*x+ch-48);
			}
			else
			{
				spush(g,ch-48);
				p++;
			}
		}
		if(pp(ch)==1)
		{ push(h,ch);p=0;}
		if(pp(ch)==4)
		{
			while(pp(h->sk[h->top])!=1)
			{
				q=spop(g,q);
				w=spop(g,w);
				th=pop(h,th);
				spush(g,yunsuan(w,th,q));
			}
			d=pop(h,d);p=0;
		}
		if(pp(ch)!=9&&pp(ch)!=1&&pp(ch)!=4)
		{
			switch(bijiao(h,ch))
			{
				case '>':push(h,ch);p=0;break;
				case '<':
				while(pp(h->sk[h->top])>=pp(ch))
				{
					q=spop(g,q);w=spop(g,w);
					th=pop(h,th);
					spush(g,yunsuan(w,th,q));
				}
				push(h,ch); p=0;break;
			}
		}
		//ch=getchar();
		ch = *(str + i);
		i++;
	}
	while(ch=='\0'&&h->sk[h->top]!='#')
	{
		q=spop(g,q);w=spop(g,w);
		th=pop(h,th);
		s=yunsuan(w,th,q);
		spush(g,s);
	}
	return g->sk[g->top];
}

//排序
void BubbleSort(int *a, int len)
{
	int i, j, k;

	for (i=0;i<len-1;i++)
		for(j=i+1;j<len;j++)
		{
			if (a[i] > a[j])
			{
				k = a[i];
				a[i] = a[j];
				a[j] = k;
			}
		}
}

LRESULT CALLBACK inputBoxWndProc(HWND wndHandle, UINT message, WPARAM wParam, LPARAM lParam)
{    
    static char **result = 0;
    static HWND inputLabelHandle = 0;
    static HWND inputHandle = 0;    
    
    switch (message)
    {
    case WM_NCCREATE:
        result = (char **)((CREATESTRUCT *)lParam)->lpCreateParams;        
        break;
    
    case WM_CLOSE:
		DestroyWindow(wndHandle);
    case WM_DESTROY:
        DestroyWindow(wndHandle);
        break;
    
    case WM_COMMAND:    
        int notifyCode = HIWORD(wParam);    
        int controlId = LOWORD(wParam);    
        HWND controlHandle = (HWND)lParam;    
        
        switch (controlId)
        {
        case ID_INPUTEDIT:    
            inputHandle = controlHandle;
            break;
            
        case ID_OK:
            *result = (char *)malloc(256);
            GetWindowText(inputHandle, *result, 256);
			PostQuitMessage(0);
			DestroyWindow(wndHandle);
            break;
        }
        break;
    }
    
    return DefWindowProc(wndHandle, message, wParam, lParam);
}
 

char *inputbox(char *hint)
{
	#define X 5
	#define OKBUTTON_WIDTH 75
	#define INPUTEDIT_WIDTH (width0 - OKBUTTON_WIDTH)
	#define INPUTEDIT_Y 29
	#define HINT_HEIGHT 24
	#define CHAR_WIDTH 12
    HWND dialogHandle, inputLabelHandle, inputHandle, okButtonHandle;
    char *result;
    WNDCLASS wndClass;
    int hint_width = strlen(hint) * CHAR_WIDTH,
        width0 = hint_width + OKBUTTON_WIDTH,
        height = HINT_HEIGHT + INPUTEDIT_Y + 50;
    MSG msg;
    
    ZeroMemory(&wndClass, sizeof wndClass);
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
//    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
//    wndClass.cbSize = sizeof wndClass;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = inputBoxWndProc;
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
    wndClass.lpszClassName = "InputBox";
    wndClass.hInstance = GetModuleHandle(0);

    RegisterClass(&wndClass);
    
    dialogHandle = CreateWindow(
        wndClass.lpszClassName, "运算方法", WS_OVERLAPPEDWINDOW,
        100, 100, width0 + 50, height,
        0, 0, wndClass.hInstance, &result);

    inputLabelHandle = CreateWindow(
        "STATIC", hint, WS_CHILD | WS_VISIBLE,
        X, 5, hint_width, HINT_HEIGHT,
        dialogHandle, 0, wndClass.hInstance, 0);

    inputHandle = CreateWindow(
        "EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE,
        X, INPUTEDIT_Y, INPUTEDIT_WIDTH, 24,
        dialogHandle, (HMENU)ID_INPUTEDIT, wndClass.hInstance, 0);

    okButtonHandle = CreateWindow(
        "BUTTON", "确定", WS_CHILD | WS_VISIBLE,
        X + INPUTEDIT_WIDTH + 5, INPUTEDIT_Y, OKBUTTON_WIDTH, 24,
        dialogHandle, (HMENU)ID_OK, wndClass.hInstance, 0);
    
    if(dialogHandle == 0)
        return 0;
 
    // 在桌面显示窗口
    ShowWindow(dialogHandle, SW_SHOWNORMAL);
 
    // 刷新窗口客户区
    UpdateWindow(dialogHandle);
 
    /*do
    {        
        if (GetMessage(&msg, 0, 0, 0))    
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while (msg.message != WM_QUIT);*/
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
 
    return result;
}

char *remove_blank(char * str)
{
	char * str_c = str;
	int i, j = 0;
	//非规定字符输入判断
	for(i = 0; i < strlen(str); i++){
		if ((str[i] < '0' || str[i] > '9') && str[i] != '+' && str[i] != '-' && str[i] != '*' 
			&& str[i] != '/' && str[i] != ' ' && str[i] != '(' && str[i] != ')'){
			illegal = 1;
		}
	} 
	if(0 == illegal){
		for(i = 0; str[i] != '\0'; i++){
			if(str[i] != ' '){
				str_c[j++] = str[i];
			}
			//合法性判断
			//数字符号重复
			if(str[i] == str[i + 1]){
				illegal = 1;
			}

		}
		str_c[j] = '\0';
	}
	return str_c;
}