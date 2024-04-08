#pragma once
#include "CGameObject.h"



class Bullet;
class CTurret : public CGameObject
{
	//CGameObject* ptarget;
	int ptarget_key;
	Bullet* pbullet;
	CSound* msound;
public:
	CTurret(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist);
	~CTurret();
	virtual void Draw(HDC hdc);
	virtual void Update();

	//CGameObject* FindTarget();
	int FindTarget();
	void Attack();

	virtual void SelectedDraw(HDC, HBRUSH);

	virtual INT		GetObjRadius();
	virtual void	Death();

	void GetSound(CSound* sound) {
		msound = sound;
	}
};

