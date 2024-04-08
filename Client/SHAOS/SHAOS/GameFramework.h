#pragma once
#include "CWorld.h"
#include "resource.h"
#include "Sound.h"
void	CALLBACK	TimerProc(HWND, UINT, UINT, DWORD);
BOOL	CALLBACK    DialogProc(HWND, UINT, WPARAM, LPARAM);


class CGameFramework
{
	HINSTANCE		mhInst;
	HWND			mhWnd;
	HWND			mhhTitleWnd;



	HDC				memdc;
	HBITMAP			hpausebutton;

	// 월드한테 좌표를 넘길 때 어디서 넘기지...
	CWorld* pworld{ nullptr };

	HBITMAP hskill_shoot;
	HBITMAP hskill_aoe;
	HBITMAP hskill_shield;
	HBITMAP hskill_return;

	INT p_hp, t_hp, p_ctshoot, p_ctAoE, p_ctshield, p_ctreturn, p_ctdeath;


public:
	CSound* msound{ nullptr };
	CGameFramework();
	~CGameFramework();

	void Create(HWND hwnd, HWND htitlewnd, HINSTANCE hInst);//Init

	void Relese();

	void Update();

	void MSG_Key(UINT message, WPARAM wParam, LPARAM lParam);
	void MSG_Mouse(UINT message, WPARAM wParam, LPARAM lParam);

	void Draw(HDC hdc);

};
