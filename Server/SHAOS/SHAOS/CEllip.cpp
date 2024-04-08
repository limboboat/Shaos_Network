#include "pch.h"
#include "CEllip.h"
#include "Bullet.h"

CEllip::CEllip(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist)
	: CUnit(OELLIP, initPos, team, enemylist, ELLIP_MAXHP)
{
	mrcRng = { (LONG)mptpos.x - ELLIP_RADIUS, (LONG)mptpos.y - ELLIP_RADIUS,
		(LONG)mptpos.x + ELLIP_RADIUS, (LONG)mptpos.y + ELLIP_RADIUS };

	//mhp = new CHp(ELLIP_MAXHP);
	if (team == TEAM::USER) {
		mrchpbar = { mrcRng.left - 7, mrcRng.top, mrcRng.left - 4, mrcRng.bottom };
	}
	else {
		mrchpbar = { mrcRng.right + 4, mrcRng.top, mrcRng.right + 7, mrcRng.bottom };
	}

	iattackcooltime = 0;
	//pattacktarget = menemylist[0];
	pattacktarget_key = 0;

	ideatheffecttime = 0;
}


CEllip::~CEllip()
{
	//delete mhp;
}

void CEllip::Draw(HDC hdc)
{
	Ellipse(hdc, mptpos.x - iattakradius, mptpos.y - iattakradius,
		mptpos.x + iattakradius, mptpos.y+- iattakradius);



	if (ideatheffecttime) {
		// 죽었을 때 이펙트
		INT tmp = ideatheffecttime / FRAMETIME;
		if (tmp % 20 > 10) {
			Ellipse(hdc, mrcRng.left, mrcRng.top,
				mrcRng.right, mrcRng.bottom);
		}
		return;
	}


	Ellipse(hdc, mrcRng.left, mrcRng.top,
		mrcRng.right, mrcRng.bottom);

	if (pbullet) pbullet->Draw(hdc);
}

void CEllip::SelectedDraw(HDC hdc, HBRUSH hbr)
{
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hbr);
	Ellipse(hdc, mrcRng.left - 2, mrcRng.top - 2,
		mrcRng.right + 2, mrcRng.bottom + 2);
	SelectObject(hdc, hOld);
}

void CEllip::Update()
{
	if (ideatheffecttime) {
		ideatheffecttime -= FRAMETIME;
		return;
	}

	// 공격할 대상 정하기
	SetTarget();
	if (menemylist[pattacktarget_key]->IsDead()) {
		pattacktarget_key = 0;
	}

	
	Move();


	if (iattackcooltime)
		iattackcooltime -= FRAMETIME;
	else {
		if (Attack())
			iattackcooltime = FRAMETIME * 50;
	}
	if (pbullet) pbullet = pbullet->Move();


}

void CEllip::Move()
{
	POINTFLOAT movevector;

	// 타겟과 거리 구하기
	float projX = menemylist[pattacktarget_key]->GetPos().x - mptpos.x;
	float projY = menemylist[pattacktarget_key]->GetPos().y - mptpos.y;

	float distance = sqrt(projX * projX + projY * projY);
	INT stopdistance = menemylist[pattacktarget_key]->GetObjRadius() + iattakradius;

	// movevector 구하는 곳
	if (distance > stopdistance) {
		float nomalizedX = projX / distance;
		float nomalizedY = projY / distance;

		movevector = { nomalizedX,nomalizedY };
	}
	else movevector = { 0,0 };


	mptpos.x += movevector.x;
	mptpos.y += movevector.y;

	mrcRng = {
	(LONG)mptpos.x - ELLIP_RADIUS, (LONG)mptpos.y - ELLIP_RADIUS,
	(LONG)mptpos.x + ELLIP_RADIUS, (LONG)mptpos.y + ELLIP_RADIUS
	};

	//hp바 이동
	if (team == TEAM::USER) {
		mrchpbar = {
			mrcRng.left - 7,
			mrcRng.bottom - (INT)GETHPBAR(mhp.GetHp(), ELLIP_RADIUS * 2, ELLIP_MAXHP),
			mrcRng.left - 4,
			mrcRng.bottom
		};
	}
	else {
		mrchpbar = { mrcRng.right + 4,
			mrcRng.bottom - (INT)GETHPBAR(mhp.GetHp(), ELLIP_RADIUS * 2, ELLIP_MAXHP),
			mrcRng.right + 7,
			mrcRng.bottom };
	}


}

BOOL CEllip::Attack()
{
	// 타겟과의 거리가 공격범위 이내일 때만 
	INT range = menemylist[pattacktarget_key]->GetObjRadius() + iattakradius;
	if (IsInRange(this, menemylist[pattacktarget_key], range)) {
		pbullet = new Bullet(&mptpos, menemylist[pattacktarget_key], ELLIP_BULLETDAMAGE);
		return TRUE;
	}
	return FALSE;

}

void CEllip::SetTarget()
{
	for (auto tmp : menemylist) {
		if (tmp.second->IsDead()) continue;
		INT range = UNIT_RECOGRNGRADIUS + RECT_RADIUS;
		if (IsInRange(this, tmp.second, range)) {
			pattacktarget_key = tmp.first;
		}
	}

}

INT CEllip::GetObjRadius()
{
	return ELLIP_RADIUS;
}
