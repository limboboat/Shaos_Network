#include "pch.h"
#include "GameFramework.h"
CGameFramework::CGameFramework()
{
	msound = new CSound;
	msound->SoundSystem();


}
CGameFramework::~CGameFramework()
{
}

void CGameFramework::Create(HWND hwnd, HWND htitlewnd, HINSTANCE hInst)
{
	msound->MyPlaySound(1, 2);//타이틀화면닫히고나서 부모창이 켜질때
	mhInst = hInst;
	mhWnd = hwnd;
	mhhTitleWnd = htitlewnd;

	pworld = new CWorld(hwnd);

	// 사운드 변수 넘겨주기
	pworld->SetSound(msound);

	HDC hdc = GetDC(hwnd);
	memdc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);

	hpausebutton = (HBITMAP)LoadImage(NULL, L"Resource/button_pause.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_shoot = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_shoot.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_aoe = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_aoe.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_shield = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_shield.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hskill_return = (HBITMAP)LoadImage(NULL, L"Resource/skillicon_return.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

}

void CGameFramework::Relese()
{
	msound->SoundStop(2); //부모창이 닫히고 타이틀창이 켜질때

	DeleteObject(hpausebutton);
	DeleteObject(hskill_shoot);
	DeleteObject(hskill_aoe);
	DeleteObject(hskill_shield);
	DeleteObject(hskill_return);

	DeleteDC(memdc);
	delete pworld;
	pworld = nullptr;
}

void CGameFramework::Update()
{
	if (pworld) pworld->Update();
	pworld->GetUIInfo(&p_hp, &p_ctshoot, &p_ctAoE, 
		&p_ctshield, &p_ctreturn, &t_hp , &p_ctdeath);

}

void CGameFramework::MSG_Key(UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (pworld) pworld->MSG_Key(message, wParam, lParam);
}

void CGameFramework::MSG_Mouse(UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT mousepos = { LOWORD(lParam),HIWORD(lParam) };
	if (PtInRect(&BUTTONPAUSE_RNG, mousepos)) {
		if (message == WM_LBUTTONUP) {
			pworld->OffPlayerMove();
			DialogBox(mhInst, MAKEINTRESOURCE(IDD_DIALOGPAUSE), mhWnd, (DLGPROC)DialogProc);
		}
		return;
	}
	else {
		if (pworld) pworld->MSG_Mouse(message, mousepos);
	}

}



void CGameFramework::Draw(HDC hdc)
{
	if (pworld) {
		pworld->Draw(hdc);

		// UI

		// 정지버튼
		SelectObject(memdc, hpausebutton);
		BitBlt(hdc, BUTTONPAUSE_RNG.left, BUTTONPAUSE_RNG.top, PAUSEBUTTONSIZE, PAUSEBUTTONSIZE,
			memdc, 0, 0, SRCCOPY);

		RECT UI_Rect = { 20,20,80,680 };
		FillRect(hdc, &UI_Rect , hUIBARBRUSH);

		// hp바 그리기
		RECT hp_rect1 = { 50, 30, 70,220 };
		RECT thp_rect1 = { 50, 255, 70, 445};
		
		RECT hp_rect2 = { 50, 220 - p_hp * 190/ PLAYER_MAXHP, 70, 220 };
		RECT thp_rect2 = { 50, 445 - t_hp * 190 / TOWER_MAXHP, 70, 445  };


		FillRect(hdc, &hp_rect2, hHPBRUSH);
		FillRect(hdc, &thp_rect2, hHPBRUSH);

		FrameRect(hdc, &hp_rect1, (HBRUSH)GetStockObject(BLACK_BRUSH));
		FrameRect(hdc, &thp_rect1, (HBRUSH)GetStockObject(BLACK_BRUSH));

		SelectObject(memdc, hskill_shoot);
		BitBlt(hdc, 30, 480, SKILLICONSIZE, SKILLICONSIZE,
			memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, hskill_aoe);
		BitBlt(hdc, 30, 530, SKILLICONSIZE, SKILLICONSIZE,
			memdc, 0, 0, SRCCOPY);

		SelectObject(memdc, hskill_shield);
		BitBlt(hdc, 30, 580, SKILLICONSIZE, SKILLICONSIZE,
			memdc, 0, 0, SRCCOPY);

		SelectObject(memdc, hskill_return);
		BitBlt(hdc, 30, 630, SKILLICONSIZE, SKILLICONSIZE,
			memdc, 0, 0, SRCCOPY);

		RECT sk_rect1,sk_rect2,sk_rect3,sk_rect4;
		sk_rect1 = { 30,480,70,520 };
		sk_rect2 = { 30,530,70,570 };
		sk_rect3 = { 30,580,70,620 };
		sk_rect4 = { 30,630,70,670 };

		RECT msk_rect1, msk_rect2, msk_rect3, msk_rect4;
		msk_rect1 = { 30,520 - p_ctshoot * SKILLICONSIZE / COOLTIME_SHOOT ,70,520 };
		msk_rect2 = { 30,570 - p_ctAoE * SKILLICONSIZE / COOLTIME_AOE,70,570 };
		msk_rect3 = { 30,620 - p_ctshield * SKILLICONSIZE / COOLTIME_SHIELD,70,620 };
		msk_rect4 = { 30,670 - p_ctreturn * SKILLICONSIZE / COOLTIME_RETURN,70,670 };

		FillRect(hdc, &msk_rect1, (HBRUSH)GetStockObject(BLACK_BRUSH));
		FillRect(hdc, &msk_rect2, (HBRUSH)GetStockObject(BLACK_BRUSH));
		FillRect(hdc, &msk_rect3, (HBRUSH)GetStockObject(BLACK_BRUSH));
		FillRect(hdc, &msk_rect4, (HBRUSH)GetStockObject(BLACK_BRUSH));

		//if()

		FrameRect(hdc, &sk_rect1, (HBRUSH)GetStockObject(WHITE_BRUSH));
		FrameRect(hdc, &sk_rect2, (HBRUSH)GetStockObject(WHITE_BRUSH));
		FrameRect(hdc, &sk_rect3, (HBRUSH)GetStockObject(WHITE_BRUSH));
		FrameRect(hdc, &sk_rect4, (HBRUSH)GetStockObject(WHITE_BRUSH));



		SetBkMode(hdc, TRANSPARENT);


		RECT Text = { 30,55,45,200 };
		DrawText(hdc, L"P L A Y E R    H P", lstrlen(L"P L A Y E R    H P"), &Text, DT_WORDBREAK| DT_CENTER);

		Text = { 30,280,45,500 };
		DrawText(hdc, L"T O W E R    H P", lstrlen(L"T O W E R    H P"), &Text, DT_WORDBREAK| DT_CENTER);

		//if (p_ctdeath) {
		//	RECT rcback = {500,300,700,400};
		//	FillRect(hdc, &rcback, (HBRUSH)GetStockObject(WHITE_BRUSH));
		//}

		if (pworld->IsEnding() != 0) {
			msound->SoundStop(2);
			msound->MyPlaySound(7, 3);
			HBRUSH hOldbr = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			HPEN hOldpn = (HPEN)SelectObject(hdc, hENDINGWHITEPEN);
			Rectangle(hdc, 0, 300, 1200, 400);
			SelectObject(hdc, hOldbr);
			SelectObject(hdc, hOldpn);

			HFONT myFont = CreateFont(100, 0, 0, 0, 10, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"궁서체");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
			SetTextColor(hdc, RGB(255, 255, 0));
			SetBkMode(hdc, TRANSPARENT);
			

			switch (pworld->IsEnding()) {
			case 1:
				TextOut(hdc, 400, 300, L"VICTORY!", lstrlen(L"VICTORY!"));
				break;
			case 2:
				TextOut(hdc, 400, 300, L"  LOSE", lstrlen(L"  LOSE"));
				break;
			}

			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}

	}

}


