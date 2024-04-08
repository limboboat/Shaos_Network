#include "pch.h"
#include "CUser.h"
#include "CPlayer.h"

#include "CRect.h"
#include "CEllip.h"

extern int viewPlayer;

CUser::CUser(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist)
	: CTeam(p_myobjlist, p_opponentobjlist)
{
	mTower = new CTower(USERTOWER_POS, TEAM::USER, p_opponentobjlist);
	AddMyObjList(TEAM::USER, mTower);
}

CUser::~CUser()
{
}

void CUser::SetInitObj()
{
	POINTFLOAT tmp = {
		PLAYER_DEFAULT_POSITION.x,
		PLAYER_DEFAULT_POSITION.y
	};

	POINTFLOAT tmp2 = {
	PLAYER_DEFAULT_POSITION.x,
	PLAYER_DEFAULT_POSITION.y - (float)PLAYER_RADIUS * 4.0f
	};

	POINTFLOAT tmp3 = {
	PLAYER_DEFAULT_POSITION.x,
	PLAYER_DEFAULT_POSITION.y + (float)PLAYER_RADIUS * 4.0f
	};
	mPlayer1 = new CPlayer(tmp, TEAM::USER, p_opponentobjlist, 0);
	this->AddMyObjList(TEAM::USER, mPlayer1);
	mPlayer2 = new CPlayer(tmp2, TEAM::USER, p_opponentobjlist, 1);
	this->AddMyObjList(TEAM::USER, mPlayer2);
	mPlayer3 = new CPlayer(tmp3, TEAM::USER, p_opponentobjlist, 2);
	this->AddMyObjList(TEAM::USER, mPlayer3);

}

void CUser::Update()
{
	if (gameover) return;

	if (p_myobjlist[0]->IsDead()) {

		p_myobjlist[0]->Update();

		if (p_myobjlist[0]->IsDelete())
			gameover = TRUE;

		return;
	}

	// 유닛 젠
	if (iunitgentime) iunitgentime -= FRAMETIME;

	UnitGen();


	// 겜오브젝 업데이트
	for (auto tmp : p_myobjlist) {
		tmp.second->Update();
		if (p_myobjlist[0]->IsDead()) return;
		if (tmp.second->IsDelete()) {
			DeleteInList(tmp.first);
		}
	}

}

void CUser::MSG_Key(int playerID, UINT message, WPARAM wParam)
{
	((CPlayer*)p_myobjlist[playerID + 1])->MSG_Key(message, wParam);
}

void CUser::MSG_Mouse(int playerID, UINT message, POINT mousepos)
{
	// 상대편 obj 영역들을 검사해서 마우스 좌표에 있으면
	// 플레이어에게 신호를 보냄

	switch (message) {
	case WM_LBUTTONUP:
		((CPlayer*)p_myobjlist[playerID + 1])->MSG_MouseUp(mousepos);
		break;
	case WM_LBUTTONDOWN:
		((CPlayer*)p_myobjlist[playerID + 1])->MSG_MouseDown(mousepos);
		break;
	case WM_MOUSEMOVE:
		((CPlayer*)p_myobjlist[playerID + 1])->MSG_MouseMove(mousepos);
		break;
	}

}

void CUser::UnitGen()
{
	int deadtowercount = 0;

	for (int i = 1; i <= 4; ++i) {
		if (p_opponentobjlist[i]->IsDead())
			deadtowercount++;
	}

	if (!iunitgentime) {
		switch (deadtowercount) {
		case 0:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);

			iunitgentime = FRAMETIME * 2000;
			break;
		}
		case 1:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);

			iunitgentime = FRAMETIME * 1750;
			break;
		}

		case 2:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);

			iunitgentime = FRAMETIME * 1500;
			break;
		}
		case 3:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);

			iunitgentime = FRAMETIME * 1250;
			break;
		}
		case 4:
		{
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);

			iunitgentime = FRAMETIME * 1000;
			break;
		}

		}
	}

	switch (deadtowercount) {
	case 0:
	{
		if (iunitgentime == FRAMETIME * 1800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		break;
	}
	case 1:
		if (iunitgentime == FRAMETIME * 1600) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 1500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		break;
	case 2:
		if (iunitgentime == FRAMETIME * 1300) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		break;
	case 3:
		if (iunitgentime == FRAMETIME * 1000) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		break;
	case 4:
		if (iunitgentime == FRAMETIME * 900) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		if (iunitgentime == FRAMETIME * 800) {
			CUnit* unit1 = new CRect(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 600) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CRect(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}
		if (iunitgentime == FRAMETIME * 500) {
			CUnit* unit1 = new CEllip(ptUnitSponPos1, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit1);

			CUnit* unit2 = new CEllip(ptUnitSponPos2, TEAM::USER, p_opponentobjlist);
			this->AddMyObjList(TEAM::USER, unit2);
		}

		break;
	}


}

void CUser::Draw(HDC hdc)
{
	for (auto tmp : p_myobjlist) {
		tmp.second->Draw(hdc);
		tmp.second->DrawHP(hdc);
	}
}

POINTFLOAT CUser::GetPlayerPos()
{
	//return mPlayer1->GetPos();
	return p_myobjlist[viewPlayer + 1]->GetPos();
}

void CUser::SetSound(CSound* sound)
{
	//mPlayer1->msound = sound;
	((CPlayer*)p_myobjlist[viewPlayer + 1])->msound = sound;
}

void CUser::GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
	INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death)
{
	//mPlayer1->UI_GetPlayerInfo(ahp, ct_shoot, ct_AoE, ct_shield, ct_return, ct_death);
	((CPlayer*)p_myobjlist[viewPlayer + 1])->UI_GetPlayerInfo(ahp, ct_shoot, ct_AoE, ct_shield, ct_return, ct_death);
	*towerhp = mTower->GetTowerHp();
}

INT CUser::GeteffecttimeAoE()
{
	//return mPlayer1->GetEffectTimeAoE();
	return ((CPlayer*)p_myobjlist[viewPlayer + 1])->GetEffectTimeAoE();
}

void CUser::OffPlayerMove()
{
	mPlayer1->OffPlayerMove();
	mPlayer2->OffPlayerMove();
	mPlayer3->OffPlayerMove();
}

