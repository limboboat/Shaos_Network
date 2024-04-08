#pragma once
#include "CGameObject.h"

#define TURRET2_ATTAKRANGE 50

class CTurret2 : public CGameObject
{

	CGameObject* ptarget;
	BOOL attackOn;
	INT attackdamage;
	POINTFLOAT attakpoint;

public:
	CTurret2(POINTFLOAT initPos, TEAM team, CGameObject* enemylist);
	~CTurret2();

	virtual void Draw(HDC hdc);
	virtual void Update();

	CGameObject* FindTarget();
	void AttackStart();
	void AttackEnd();

	virtual void SelectedDraw(HDC, HBRUSH);

	virtual INT		GetObjRadius();
	virtual void	Death();

};

