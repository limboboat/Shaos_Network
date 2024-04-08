#pragma once
#include "CUnit.h"


class CTri : public CUnit
{
	INT iattakradius;
	float halfedge;
	BOOL attackOn;
public:
	CTri(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist);
	~CTri();

	virtual void Update();
	virtual void Draw(HDC hdc);

	virtual void SelectedDraw(HDC, HBRUSH);

	void Move();
	void Attack();

	virtual INT	GetObjRadius();
};

