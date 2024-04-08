#include "pch.h"
#include "CTeam.h"
#include "CGameObject.h"

#include "CWorld.h"

#include "CNetwork.h"
extern CNetwork* Network;

CTeam::CTeam(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist)
	: p_myobjlist(p_myobjlist), p_opponentobjlist(p_opponentobjlist), iunitgentime(0)
{
	gameover = FALSE;
}


CTeam::~CTeam()
{

}

void CTeam::AddMyObjList(TEAM team, CGameObject* addobj)
{
	if (team == TEAM::USER) {
		p_myobjlist.insert({ CWorld::user_manageID++, addobj });

		SendInitObj temp;
		temp.key = CWorld::user_manageID - 1;
		temp.objTAG = addobj->objTAG;
		temp.team = TEAM::USER;
		temp.mptpos = addobj->mptpos;

		//타워, 포탑, 유닛을 제외하고 보낸다(기본생성)
		if (CWorld::user_manageID > 4)
			Network->SendInit(temp);
	}
	else if (team == TEAM::ENEMY) {
		p_myobjlist.insert({ CWorld::enemy_manageID++, addobj });

		SendInitObj temp;
		temp.key = CWorld::enemy_manageID - 1;
		temp.objTAG = addobj->objTAG;
		temp.team = TEAM::ENEMY;
		temp.mptpos = addobj->mptpos;

		//타워, 포탑, 유닛을 제외하고 보낸다(기본생성)
		if (CWorld::enemy_manageID > 5)
			Network->SendInit(temp);
	}
}

void CTeam::DeleteInList(int key)
{
	SendDelObj temp;
	temp.key = key;
	temp.team = p_myobjlist[key]->team;

	Network->SendDel(temp);

	delete p_myobjlist[key];
	p_myobjlist.erase(key);
}

