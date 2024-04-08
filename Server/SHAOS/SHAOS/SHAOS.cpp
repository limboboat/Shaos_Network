// SHAOS.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "SHAOS.h"
#include "GameFramework.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;										// 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = L"SHAOS";               // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING] = L"WndClass";         // 기본 창 클래스 이름입니다.
CGameFramework g_GameFrameWork;

int viewPlayer;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
void                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    TitleProc(HWND, UINT, WPARAM, LPARAM);
void	CALLBACK	TimerProc(HWND, UINT, UINT, DWORD);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, L"SHAOS");

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
void MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	wcex.lpfnWndProc = TitleProc;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName = L"TitleWndClass";

	RegisterClassEx(&wcex);

}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	DWORD dwStyle = WS_CAPTION;

	RECT rcWindow{};
	GetWindowRect(GetDesktopWindow(), &rcWindow);         //전체 윈도우에서 창이 어디에 있는지 반환하는 함수(전체 윈도우의 해상도 구하는 함수,)
	RECT rcClient{ 0,0,CLIENT_WIDTH,CLIENT_HEIGHT};
	//AdjustWindowRect(&rcClient, dwStyle, true);         //dwStyle을 포함한 rect값을 rcClient에 넣어줌
	//
	//													//AdjustWindowRect가 값을 빼줌으로 그 값에 맞게 값 조절
	//rcClient.right -= rcClient.left;
	//rcClient.bottom -= rcClient.top;
	//rcClient.left = rcClient.top = 0;

	POINT Start{ (rcWindow.right - rcClient.right) / 2         //창이 윈도우 중간에 뜨도록 
		,(rcWindow.bottom - rcClient.bottom) / 2 };




	HWND hWnd = CreateWindowW(szWindowClass, szTitle, dwStyle,
		Start.x,
		Start.y,
		rcClient.right,
		rcClient.bottom,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//타이틀 윈도우가 활성화되어있을 때는 부모 윈도우의 타이머를 꺼준다
// -> 게임 프레임워크의 함수를 부르지 않는다
// 메인 윈도우의 타이머관리를 타이틀 윈도우에서 해준다.


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	case WM_CREATE:
	{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		CreateWindow(L"TitleWndClass", NULL, WS_CHILD | WS_VISIBLE,
			0, 0, rcClient.right, rcClient.bottom, hWnd, NULL, hInst, NULL);
	
		//g_GameFrameWork.Create(hWnd, titleWnd, hInst);
		//SetTimer(hWnd, 0, 16, nullptr);
	
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

		//더블 버퍼링
		HBITMAP hNew, hOld; // 비트맵
		HDC backDC = CreateCompatibleDC(hdc); //뒤버퍼
		RECT rect; // 창크기
		GetClientRect(hWnd, &rect); // 창크기 구하기 
		hNew = CreateCompatibleBitmap(hdc, rect.right, rect.bottom); //hdc와 호환되는 비트맵을 불러옴
		hOld = (HBITMAP)SelectObject(backDC, hNew);  //backDC의 이전 비트맵을 반환하고 새 비트맵으로 교체
		FillRect(backDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH)); // 뒷 버퍼 배경을 칠해줌

		g_GameFrameWork.Draw(backDC);

		BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, backDC, rect.left, rect.top, SRCCOPY); // 비트맵 복사 현재 버퍼에 뒷버퍼의 내용을 복사함
		DeleteObject(SelectObject(backDC, hOld)); //이전 비트맵 삭제
		DeleteDC(backDC); //뒷버퍼 삭제
						  //더블 버퍼링 종료


		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
	//case WM_KEYUP:
		viewPlayer = (viewPlayer + 1) % 3;
		//g_GameFrameWork.MSG_Key(message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:

		//g_GameFrameWork.MSG_Mouse(message, wParam, lParam);
		break;
	//case WM_COMMAND:
	//	g_GameFrameWork.Command(HIWORD(wParam));
	//	break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		g_GameFrameWork.msound->SoundRelease();
		g_GameFrameWork.Relese();
		PostQuitMessage(0);
		break;
	}
    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK TitleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HBITMAP hTitleBit;
	switch (message) {
	case WM_CREATE:
	{	

		g_GameFrameWork.msound->MyPlaySound(0, 1);//창이켜질때

		HWND hStart = CreateWindow(
			L"button",
			L"START",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			850, 340, 200, 30,
			hWnd,
			(HMENU)IDC_BUTTON_START,
			hInst,
			nullptr
		);

		HWND hExit = CreateWindow(
			L"button",
			L"EXIT",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			850, 400, 200, 30,
			hWnd,
			(HMENU)IDC_BUTTON_EXIT,
			hInst,
			nullptr
		);
		hTitleBit = (HBITMAP)LoadImage(NULL, L"Resource/SHAOS타이틀.bmp", IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	}
	case WM_COMMAND:
	{
		UINT ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDC_BUTTON_START:
			g_GameFrameWork.msound->SoundStop(1); //타이틀화면이 닫힐때
			g_GameFrameWork.Create(GetParent(hWnd), hWnd, hInst);
			SetTimer(GetParent(hWnd), 0, FRAMETIME, (TIMERPROC)TimerProc);
			DestroyWindow(hWnd);
			break;

		case IDC_BUTTON_EXIT:
			DestroyWindow(GetParent(hWnd));
			break;

		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hOld = (HBITMAP)SelectObject(memdc, hTitleBit);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom,
			memdc, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		// 부모 죽으면 차일드는 알아서 죽는다
		DeleteObject(hTitleBit);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CALLBACK TimerProc(HWND hWnd, UINT iMsg, UINT idEvent, DWORD dwTime) {
	g_GameFrameWork.Update();
	InvalidateRect(hWnd, nullptr, false);
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	//bool값 등 활성화된 상태가 있으면 복귀했을 때 문제다



	switch (message) {
	case WM_INITDIALOG:


		KillTimer(GetParent(hDlg), 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			SetTimer(GetParent(hDlg), 0, FRAMETIME, (TIMERPROC)TimerProc);
			EndDialog(hDlg, 0);
			break;
		case ID_BACKTITLE:
			g_GameFrameWork.Relese();	// 게임 종료

			g_GameFrameWork.msound->SoundStop(4);

			//  타이틀 윈도우 살리기
			RECT rcClient;
			GetClientRect(GetParent(hDlg), &rcClient);
			CreateWindow(L"TitleWndClass", NULL, WS_CHILD | WS_VISIBLE,
				0, 0, rcClient.right, rcClient.bottom, GetParent(hDlg), NULL, hInst, NULL);
			
			EndDialog(hDlg, 0);	//대화상자 종료
			break;
		case ID_EXIT:
			DestroyWindow(GetParent(hDlg));
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		RECT rcDlgClient;
		GetClientRect(hDlg, &rcDlgClient);
		FillRect(hdc, &rcDlgClient, (HBRUSH)GetStockObject(BLACK_BRUSH));

		EndPaint(hDlg, &ps);
	}
	break;
	case WM_DESTROY:

		break;
	}
	return false;
}
