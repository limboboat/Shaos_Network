#pragma once
#include "Bullet.h"

class CGameObject;
class Bullet
{
	POINTFLOAT ptbulletpos;
	CGameObject* ptargetobj;
	
	const INT idamage;

public:
	Bullet(const POINTFLOAT* initPos, CGameObject* target, INT damage);
	~Bullet();
	Bullet* Move();
	void Draw(HDC hdc);
};