#include "pch.h"
#include "CTower.h"
#include "Sound.h"

CTower::CTower(POINTFLOAT initPos, TEAM team, map<int, CGameObject*>& enemylist)
	: CGameObject(OTOWER, initPos, team, enemylist, TOWER_MAXHP)
{
	mrcRng = { (LONG)(mptpos.x - TOWER_CENTER2VERTAX),
		(LONG)(mptpos.y - TOWER_CENTER2VERTAX),
		(LONG)(mptpos.x + TOWER_CENTER2VERTAX),
		(LONG)(mptpos.y + TOWER_CENTER2VERTAX)
	};

	// 먼저 그리는 삼각형
	triangle1[0] = { (LONG)mptpos.x, (LONG)(mptpos.y + TOWER_CENTER2VERTAX) };
	triangle1[1] = { (LONG)(mptpos.x - TOWER_HALFSIDE),(LONG)(mptpos.y - TOWER_CENTER2SIDE) };
	triangle1[2] = { (LONG)(mptpos.x + TOWER_HALFSIDE),(LONG)(mptpos.y - TOWER_CENTER2SIDE) };
	// 나중에 그리는 삼각형
	triangle2[0] = { (LONG)mptpos.x, (LONG)(mptpos.y - TOWER_CENTER2VERTAX) };
	triangle2[1] = { (LONG)(mptpos.x - TOWER_HALFSIDE),(LONG)(mptpos.y + TOWER_CENTER2SIDE) };
	triangle2[2] = { (LONG)(mptpos.x + TOWER_HALFSIDE),(LONG)(mptpos.y + TOWER_CENTER2SIDE) };

	//mhp = new CHp(TOWER_MAXHP);
	if (team == TEAM::USER) {
		mrchpbar = { mrcRng.left - 7, mrcRng.top, mrcRng.left - 4, mrcRng.bottom };
		hTriBrush = CreateSolidBrush(RGB(0, 0, 200));
	}
	else {
		mrchpbar = { mrcRng.right + 4, mrcRng.top, mrcRng.right + 7, mrcRng.bottom };
		hTriBrush = CreateSolidBrush(RGB(200, 0, 0));
	}
	msound = new CSound;
}


CTower::~CTower()
{
	DeleteObject(hTriBrush);
	//delete mhp;
}


void CTower::Draw(HDC hdc)
{
	HBRUSH hOld;
	(team == TEAM::USER) ?
		hOld = (HBRUSH)SelectObject(hdc, hUSERTOWERCIRCLEBRUSH) :
		hOld = (HBRUSH)SelectObject(hdc, hENEMYTOWERCIRCLEBRUSH);
	Ellipse(hdc, mrcRng.left, mrcRng.top, mrcRng.right, mrcRng.bottom);
	
	SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	Polygon(hdc, triangle1, 3);
	
	SelectObject(hdc, hTriBrush);
	Polygon(hdc, triangle2, 3);

	SelectObject(hdc, hOld);

	if (ideatheffecttime) {
		INT color = 255 * ideatheffecttime / TOWER_EFFECTTIME_DEATH;
		DeleteObject(hTriBrush);
		(team == TEAM::USER) ? hTriBrush = CreateSolidBrush(RGB(0, 0, color)) : hTriBrush = CreateSolidBrush(RGB(color, 0, 0));
	}
}

void CTower::SelectedDraw(HDC hdc, HBRUSH hbr)
{
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hbr);

	Ellipse(hdc, mrcRng.left - 4, mrcRng.top - 4,
		mrcRng.right + 4, mrcRng.bottom + 4);


	SelectObject(hdc, hOld);

}

void CTower::Update()
{
	mrchpbar.top = mrcRng.bottom - (INT)GETHPBAR(mhp.GetHp(), TOWER_CENTER2VERTAX * 2, TOWER_MAXHP);
	
	
	if (ideatheffecttime) {
		if (ideatheffecttime == TOWER_EFFECTTIME_DEATH) {
			msound->MyPlaySound(10, 3);

		}
		ideatheffecttime -= FRAMETIME;
	}
}

INT CTower::GetObjRadius()
{
	return TOWER_CENTER2VERTAX;
}

void CTower::Death()
{
	if (!ideatheffecttime) {
		ideatheffecttime = TOWER_EFFECTTIME_DEATH;
		//msound->MyPlaySound(10, 3);
	}
}

INT CTower::GetTowerHp()
{
	return mhp.GetHp();
}

