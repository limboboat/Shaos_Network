#pragma once
#include "CUnit.h"
#include "CTower.h"

class CGameObject;
class CTeam
{
protected:
	// ���� �׻� Ÿ���� ����Ų��
	map<int, CGameObject*>& p_myobjlist;
	map<int, CGameObject*>& p_opponentobjlist;

	UINT iunitgentime;

public:
	BOOL gameover;


	CTeam(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist);
	virtual ~CTeam();


	void AddMyObjList(TEAM team, CGameObject*);
	void DeleteInList(int key);


};

