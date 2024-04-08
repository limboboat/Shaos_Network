#pragma once
#include "CTeam.h"

class CTurret;
class CTurret2;

class CEnemy : public CTeam
{
	// gameobjlist에 타워, 포탑, 유닛 순으로 연결
	CTower* mTower;
	CTurret* mTurret1;
	CTurret* mTurret2;
	CTurret* mTurret3;
	CTurret* mTurret4;

	const POINTFLOAT ptUnitSponPos1 = { 3200, 200 };
	const POINTFLOAT ptUnitSponPos2 = { 3200, 500 };


public:
	CEnemy(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist);
	~CEnemy();

	void SetInitObj();

	void Update();

	void Draw(HDC hdc);

	void UnitGen();

};