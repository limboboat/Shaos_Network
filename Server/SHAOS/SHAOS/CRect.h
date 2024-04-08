#pragma once
#include "CUnit.h"
#include "CTower.h"


class CRect : public CUnit
{
	const INT iattakradius = RECT_RADIUS + 10;

public:
	CRect(POINTFLOAT ainitPos, TEAM team, map<int, CGameObject*>& enemylist);
	~CRect();

	virtual void Update();
	virtual void Draw(HDC hdc);
	
	virtual void SelectedDraw(HDC, HBRUSH);


	void Move();
	void Attack();
	void SetTarget();

	virtual INT		GetObjRadius();
};

