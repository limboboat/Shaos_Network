#pragma once
#include "CUnit.h"
 
class Bullet;
class CEllip : public CUnit
{
	Bullet* pbullet;
	INT iattakradius = 200;
public:
	CEllip(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist);
	~CEllip();
	virtual void Update();
	virtual void Draw(HDC hdc);

	virtual void SelectedDraw(HDC, HBRUSH);

	void Move();
	BOOL Attack();

	void SetTarget();

	virtual INT		GetObjRadius();

};

