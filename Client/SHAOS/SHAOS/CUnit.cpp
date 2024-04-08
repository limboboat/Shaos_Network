#include "pch.h"
#include "CUnit.h"


CUnit::CUnit(int objTAG, POINTFLOAT ainitPos, TEAM team, map<int, CGameObject*>& enemylist, CHp mhp)
	: CGameObject(objTAG, ainitPos, team, enemylist, mhp)
{
	moveOn = TRUE;
	ptenemytowerpos = menemylist[0]->GetPos();
}


CUnit::~CUnit()
{
}

void CUnit::Draw(HDC hdc)
{
}

void CUnit::Death()
{
	ideatheffecttime = UNIT_EFFECTTIME_DEATH;
}
