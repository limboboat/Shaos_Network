#pragma once
#include "CUser.h"
#include "CEnemy.h"

class CWorld
{
	HBITMAP hbackgroundbmp;		//map.bmp �ڵ�

	HDC hUpdateDC;
	HDC hBackgroundDC;

	HBITMAP hbackgroundsizebmp;
	HBITMAP hbackgroundOld, hupdateOld;
	RECT rcClient;

	map<int, CGameObject*> user_objlist;
	map<int, CGameObject*> enemy_objlist;

	CUser* pUserTeam{ nullptr };
	CEnemy* pEnemyTeam{ nullptr };

	INT iViewX;
	

	INT gamestate;	//0:���� 1:���� �� 2: ���� ��
	CSound* sound{ nullptr };

public:
	// user_objlist�� enemy_objlist�� ���� ������ +�Ǵ� ID
	static int user_manageID;
	static int enemy_manageID;

public:
	CWorld(HWND hwnd);
	~CWorld();

	
	void MSG_Mouse(int, UINT, POINT, int);
	void MSG_Key(int, UINT, WPARAM, LPARAM);

	void Update();
	
	void Draw(HDC);

	void SetSound(CSound*);

	void GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
		INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death);

	INT IsEnding();

	void OffPlayerMove();
};

