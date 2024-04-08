#pragma once
#include "CUnit.h"
#include "CTower.h"

class CGameObject;
class CTeam
{
protected:
	// ���� �׻� Ÿ���� ����Ų��
	//CGameObject* p_myobjlist{ nullptr };
	//CGameObject* p_opponentobjlist{ nullptr };
	map<int, CGameObject*>& p_myobjlist;
	map<int, CGameObject*>& p_opponentobjlist;


	//INT imyobjnum;
	//INT iopnobjnum;

	UINT iunitgentime;

public:
	BOOL gameover;


	CTeam(map<int, CGameObject*>& p_myobjlist, map<int, CGameObject*>& p_opponentobjlist);
	virtual ~CTeam();

	void AddMyObjList(TEAM team, CGameObject*);
	void DeleteInList(int key);


};

