#include "pch.h"
#include "CEnemy.h"
#include "CTurret.h"
#include "CTurret2.h"
#include "CRect.h"
#include "CEllip.h"
#include "CDia.h"
#include "CTri.h"

CEnemy::CEnemy(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist)
	: CTeam(p_myobjlist, p_opponentobjlist)
{
	mTower = new CTower(ENEMYTOWER_POS, TEAM::ENEMY, p_opponentobjlist);
	AddMyObjList(TEAM::ENEMY, mTower);
}


CEnemy::~CEnemy()
{
}

void CEnemy::SetInitObj()
{
	mTurret1 = new CTurret(TURRET1_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(TEAM::ENEMY, mTurret1);

	mTurret2 = new CTurret(TURRET2_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(TEAM::ENEMY, mTurret2);

	mTurret3 = new CTurret(TURRET3_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(TEAM::ENEMY, mTurret3);

	mTurret4 = new CTurret(TURRET4_POS, TEAM::ENEMY, p_opponentobjlist);
	this->AddMyObjList(TEAM::ENEMY, mTurret4);

}

void CEnemy::Update()
{
	if (gameover) return;

	if (p_myobjlist[0]->IsDead()) {

		p_myobjlist[0]->Update();

		if(p_myobjlist[0]->IsDelete())
			gameover = TRUE;

		return;
	}

	// À¯´Ö Á¨
	if (iunitgentime) iunitgentime -= FRAMETIME;

	UnitGen();
	
	for (auto tmp : p_myobjlist) {
		tmp.second->Update();

		if (tmp.second->IsDelete()) {
			DeleteInList(tmp.first);
		}
	}

}

void CEnemy::Draw(HDC hdc)
{
	for (auto tmp : p_myobjlist) {
		tmp.second->Draw(hdc);
		tmp.second->DrawHP(hdc);
	}
}

void CEnemy::UnitGen()
{
	int deadtowercount = 0;

	if (mTurret1->IsDead()) deadtowercount++;
	if (mTurret2->IsDead()) deadtowercount++;
	if (mTurret3->IsDead()) deadtowercount++;
	if (mTurret4->IsDead()) deadtowercount++;

	if (!iunitgentime) {
		switch (deadtowercount) {
		case 0:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

			iunitgentime = FRAMETIME * 2000;
			break;
		}
		case 1:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

			iunitgentime = FRAMETIME * 1750;
			break;
		}

		case 2:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

			iunitgentime = FRAMETIME * 1500;
			break;
		}
		case 3:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

			iunitgentime = FRAMETIME * 1250;
			break;
		}
		case 4:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

			iunitgentime = FRAMETIME * 1000;
			break;
		}

		}
	}
	switch (deadtowercount) {
	case 0:
	{
		if (iunitgentime == FRAMETIME * 1800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1700) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}

		break;
	}
	case 1:
	{
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1400) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}



		break;
	}
	case 2:
	{
		if (iunitgentime == FRAMETIME * 1400) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1200) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1100) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}


		break;
	}
	case 3:
	{
		if (iunitgentime == FRAMETIME * 1100) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 700) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}

		break;
	}
	case 4:
	{
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 700) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 500) {
			CUnit* unit1 = new CDia(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CDia(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 400) {
			CUnit* unit1 = new CTri(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CTri(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}
		if (iunitgentime == FRAMETIME * 300) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::ENEMY, p_opponentobjlist);
			this->AddMyObjList(TEAM::ENEMY, unit2);

		}


		break;
	}
	}


}
