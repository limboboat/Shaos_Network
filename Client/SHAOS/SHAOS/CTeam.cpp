#include "pch.h"
#include "CTeam.h"
#include "CGameObject.h"

#include "CWorld.h"

CTeam::CTeam(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist)
	: p_myobjlist(p_myobjlist), p_opponentobjlist(p_opponentobjlist)
{
	gameover = FALSE;
}


CTeam::~CTeam()
{

}

void CTeam::AddMyObjList(TEAM team, CGameObject* addobj)
{
	if (team == TEAM::USER) p_myobjlist.insert({ CWorld::user_manageID++, addobj });
	else if (team == TEAM::ENEMY) p_myobjlist.insert({ CWorld::enemy_manageID++, addobj });
}

void CTeam::DeleteInList(int key)
{
	delete p_myobjlist[key];
	p_myobjlist.erase(key);
}

