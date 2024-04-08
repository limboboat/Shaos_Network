#include "pch.h"
#include "Bullet.h"
#include "CGameObject.h"

Bullet::Bullet(const POINTFLOAT* initPos, CGameObject* target, INT damage)
	: ptbulletpos(*initPos), ptargetobj(target) , idamage(damage)
{
}
Bullet::~Bullet()
{
}

Bullet* Bullet::Move()
{
	if (ptargetobj->IsDead()) {
		this->~Bullet();
		return nullptr;
	}
	float projX = ptargetobj->GetPos().x - ptbulletpos.x;
	float projY = ptargetobj->GetPos().y - ptbulletpos.y;

	float distance = sqrt(projX * projX + projY * projY);

	// 총알이 목표 오브젝트의 안에 들어가면 소멸
	if (distance < ptargetobj->GetObjRadius()) {
		// 상대편 hp깎기
		ptargetobj->PutDamage(idamage);

		this->~Bullet();
		return nullptr;
	}

	float nomalizedX = projX / distance;
	float nomalizedY = projY / distance;

	ptbulletpos.x += nomalizedX * BULLET_SPEED;
	ptbulletpos.y += nomalizedY * BULLET_SPEED;

	return this;
}

void Bullet::Draw(HDC hdc)
{
	Ellipse(hdc, ptbulletpos.x - BULLET_RADIUS, ptbulletpos.y - BULLET_RADIUS,
		ptbulletpos.x + BULLET_RADIUS, ptbulletpos.y + BULLET_RADIUS);
}