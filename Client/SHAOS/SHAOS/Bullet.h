#pragma once
#include "Bullet.h"
#include <map>
using std::map;

class CGameObject;
class Bullet
{
	POINTFLOAT ptbulletpos;
	//CGameObject* ptargetobj;
	map<int, CGameObject*>& enemylist;
	int target_key;
	
	const INT idamage;

public:
	Bullet(const POINTFLOAT* initPos, map<int, CGameObject*>& enemylist, int target_key, INT damage);
	~Bullet();
	Bullet* Move();
	void Draw(HDC hdc);
};