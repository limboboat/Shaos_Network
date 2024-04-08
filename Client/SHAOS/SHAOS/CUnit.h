#pragma once
#include "CGameObject.h"


class CTower;

class CUnit : public CGameObject
{
protected:
	BOOL moveOn;
	POINTFLOAT ptenemytowerpos;
	//CGameObject* pattacktarget;
	int pattacktarget_key;

public:
	CUnit(int objTAG, POINTFLOAT ainitPos, TEAM team, map<int, CGameObject*>& enemylist, CHp mhp);
	~CUnit();

	virtual void Draw(HDC hdc) = 0;
	virtual void Update() = 0;

	virtual INT GetObjRadius() = 0;
	virtual void Death();
};

