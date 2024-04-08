#pragma once
#include "CUser.h"
#include "CEnemy.h"

class CWorld
{
	HBITMAP hbackgroundbmp;		//map.bmp 핸들

	HDC hUpdateDC;
	HDC hBackgroundDC;

	HBITMAP hbackgroundsizebmp;
	HBITMAP hbackgroundOld, hupdateOld;
	RECT rcClient;

	map<int, CGameObject*> user_objlist;
	map<int, CGameObject*> enemy_objlist;

	CUser* pUserTeam{ nullptr };
	CEnemy* pEnemyTeam{ nullptr };

	

	INT gamestate;	//0:진행 1:유저 승 2: 유저 패
	CSound* sound{ nullptr };

public:
	static INT iViewX;

	static int user_manageID;
	static int enemy_manageID;

public:
	CWorld(HWND hwnd);
	~CWorld();

	
	void MSG_Mouse(UINT, POINT);
	void MSG_Key(UINT, WPARAM, LPARAM);

	void Update();
	
	void Draw(HDC);

	void SetSound(CSound*);

	void GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
		INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death);

	INT IsEnding();

	void OffPlayerMove();
};

