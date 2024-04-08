#pragma once
#include "CHp.h"

#include <map>
using std::map;

class CSound;
class CGameObject
{
public:
	const int objTAG;

	const TEAM team;

	POINTFLOAT		mptpos;


	RECT			mrcRng;

	CHp				mhp;
	RECT			mrchpbar;
	BOOL			mdeath;


	map<int, CGameObject*>& menemylist;			// �� ������Ʈ ����Ʈ


	INT iattackcooltime;
	INT ideatheffecttime;

public:
	CGameObject(int objTAG, POINTFLOAT ainitPos, TEAM ateam , map<int, CGameObject*>& aenemylist, CHp mhp);
	virtual ~CGameObject();

	virtual void Draw(HDC) = 0;
	virtual void Update() = 0;
	void DrawHP(HDC);

	virtual void SelectedDraw(HDC, HBRUSH) {};


	POINTFLOAT		GetPos() const;
	RECT			GetRng(); // ? �ʿ��ұ�?
	
	virtual INT		GetObjRadius() = 0;

	virtual void	PutDamage(INT damage);

	virtual void	Death() = 0;
	BOOL			IsDead() const;
	BOOL			IsDelete() const;
};

BOOL IsInRange(CGameObject* me, CGameObject* obj, INT range);
