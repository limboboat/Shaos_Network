#pragma once
#include "CGameObject.h"


class Bullet;
class Sound;
class CPlayer : public CGameObject
{
	int playerID;
public:
	BOOL R_On, L_On, U_On, D_On;
	BOOL pressQ, pressSft, onshield;


	UINT effecttime_AoE;
	UINT effecttime_Shoot;
	UINT effecttime_Return;

	UINT castingtime_return;
	UINT activetime_shield;
	
	const INT iAoERadius;

	UINT cooltime_Shoot;
	UINT cooltime_AoE;
	UINT cooltime_Shield;
	UINT cooltime_Return;
	UINT cooltime_Death;

	POINT triangle1[3];
	POINT triangle2[3];

	//CGameObject*	ptarget;
	int	ptarget_key;
	Bullet*			pbullet;

	POINT worldmousepos;

	POINT shootattackpt[7];
	RECT shootattackrange[7];

	BOOL immotal;
public:
	CSound* msound{ nullptr };
	CPlayer(POINTFLOAT ainitPos, TEAM team, map<int, CGameObject*>& enemylist, int playerID);
	~CPlayer();

	virtual void		Draw(HDC hdc);
	virtual void		Update();

	void				MSG_Key(UINT message, WPARAM wParam);
	void				MSG_MouseMove(POINT mousepos);
	void				MSG_MouseUp(POINT mousepos);
	void				MSG_MouseDown(POINT mousepos);

	void				Move();
	BOOL				Attack();


	void				Skill_Shoot();
	void				Skill_AreaOfEffect();
	void				ActiveShield();
	void				ReturnHome();



	virtual INT			GetObjRadius();
	virtual void		Death();

	POINTFLOAT			Player_Vector();
	void				SetPos(POINT setpos);
	void				UI_GetPlayerInfo(INT* ahp, INT* ct_shoot,
										INT* ct_AoE, INT* ct_shield, INT* ct_return, INT* ct_death);
	void				PutDamage(INT damage);
	INT					GetEffectTimeAoE();
	void				 OffPlayerMove();
	
};