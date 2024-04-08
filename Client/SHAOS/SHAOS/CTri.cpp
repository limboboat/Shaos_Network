#include "pch.h"
#include "CTri.h"


CTri::CTri(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist)
	: CUnit(OTRI, initPos, team, enemylist, TRI_MAXHP)
{
	mrcRng = { (LONG)mptpos.x - TRI_INNERCIRCLERADIUS, (LONG)mptpos.y - TRI_INNERCIRCLERADIUS,
		(LONG)mptpos.x + TRI_INNERCIRCLERADIUS, (LONG)mptpos.y + TRI_INNERCIRCLERADIUS };
	
	
	//mhp = new CHp(TRI_MAXHP);
	mrchpbar = { mrcRng.right + 4, mrcRng.top - TRI_INNERCIRCLERADIUS,
		mrcRng.right + 7, mrcRng.bottom + TRI_INNERCIRCLERADIUS };


	//pattacktarget = menemylist->next;
	//list<CGameObject*>::iterator itr = menemylist.begin();
	//advance(itr, 1);
	//pattacktarget = *itr;
	pattacktarget_key = 1;

	halfedge = sqrt(3) * TRI_INNERCIRCLERADIUS;
	iattakradius = TRI_INNERCIRCLERADIUS * 2;		//외심의 반지름

	iattackcooltime = 0;
}


CTri::~CTri()
{
	//delete mhp;
}

void CTri::Draw(HDC hdc)
{
	POINT vertax[3];


	vertax[0] = { (LONG)(mptpos.x - 2 * TRI_INNERCIRCLERADIUS) , (LONG)mptpos.y };
	vertax[1] = { mrcRng.right, (LONG)(mptpos.y - halfedge) };
	vertax[2] = { mrcRng.right, (LONG)(mptpos.y + halfedge) };


	if (ideatheffecttime) {
		// 죽었을 때 이펙트
		INT tmp = ideatheffecttime / FRAMETIME;
		if (tmp % 20 > 10) {
			Polygon(hdc, vertax, 3);
		}
		return;
	}

	Polygon(hdc, vertax, 3);

	if (iattackcooltime >= FRAMETIME * 25) {
		// tri 공격 이펙트

		HBRUSH hOld = (HBRUSH)SelectObject(hdc, hTIRATTACBRUSH);

		vertax[0] = { (LONG)(mptpos.x - 2 * TRI_INNERCIRCLERADIUS) - 10 , (LONG)mptpos.y };
		vertax[1] = { mrcRng.right + 5, (LONG)(mptpos.y - halfedge) - 5 };
		vertax[2] = { mrcRng.right + 5, (LONG)(mptpos.y + halfedge) + 5 };
		Polygon(hdc, vertax, 3);

		SelectObject(hdc, hOld);
	}


}

void CTri::SelectedDraw(HDC hdc, HBRUSH hbr)
{
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hbr);
	POINT vertax[3];
	vertax[0] = { (LONG)(mptpos.x - 2 * TRI_INNERCIRCLERADIUS) - 4 , (LONG)mptpos.y };
	vertax[1] = { mrcRng.right + 2, (LONG)(mptpos.y - halfedge) - 3 };
	vertax[2] = { mrcRng.right + 2, (LONG)(mptpos.y + halfedge) + 3 };

	Polygon(hdc, vertax, 3);
	SelectObject(hdc, hOld);
}

void CTri::Move()
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
		attackOn = FALSE;
	}
	else {
		movevector = { 0,0 };
		attackOn = TRUE;
	}

	mptpos.x += movevector.x;
	mptpos.y += movevector.y;

	mrcRng = {
		(LONG)mptpos.x - TRI_INNERCIRCLERADIUS, (LONG)mptpos.y - TRI_INNERCIRCLERADIUS,
		(LONG)mptpos.x + TRI_INNERCIRCLERADIUS, (LONG)mptpos.y + TRI_INNERCIRCLERADIUS
	};



	mrchpbar = { mrcRng.right + 4,
	mrcRng.bottom + TRI_INNERCIRCLERADIUS - (INT)GETHPBAR(mhp.GetHp(), TRI_INNERCIRCLERADIUS * 4, TRI_MAXHP),
	mrcRng.right + 7,
	mrcRng.bottom + TRI_INNERCIRCLERADIUS };

}

void CTri::Attack()
{
	if (!attackOn) return;
	menemylist[pattacktarget_key]->PutDamage(TRI_ATTACKDAMAGE);
	iattackcooltime = FRAMETIME * 30;
}

void CTri::Update()
{

	if (ideatheffecttime) {
		ideatheffecttime -= FRAMETIME;
		return;
	}

	//타겟 설정
	//if (menemylist->next->IsDead()) {
	//	pattacktarget = menemylist;
	//}
	//else pattacktarget = menemylist->next;
	//list<CGameObject*>::iterator itr = menemylist.begin();
	//advance(itr, 1);
	//if ((*itr)->IsDead()) {
	//	pattacktarget = menemylist.front();
	//}
	if (menemylist[1]->IsDead()) {
		pattacktarget_key = 0;
	}
	else pattacktarget_key = 1;

	Move();

	//공격
	if (iattackcooltime)
		iattackcooltime -= FRAMETIME;
	else {

		Attack();

	}

}

INT CTri::GetObjRadius()
{
	return TRI_INNERCIRCLERADIUS;
}
