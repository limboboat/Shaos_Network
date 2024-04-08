#pragma once
#include "CUnit.h"


class CDia : public CUnit
{
	BOOL attackOn;
public:
	CDia(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist);
	~CDia();


	virtual void Update();
	virtual void Draw(HDC hdc);

	virtual void SelectedDraw(HDC, HBRUSH);

	void Move();
	void Attack();

	virtual INT		GetObjRadius();

};

