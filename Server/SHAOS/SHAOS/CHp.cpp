#include "pch.h"
#include "CHp.h"


CHp::CHp(INT initHp) : ihp(initHp), imaxHp(initHp)
{
	IsZero = FALSE;
}

CHp::CHp()
{
}

CHp::~CHp()
{
}

INT CHp::GetHp() const
{
	return ihp;
}

void CHp::SetHp(INT sethp)
{
	IsZero = FALSE;
	ihp = sethp;
}

void CHp::AddHp(INT addhp)
{
	(ihp + addhp > imaxHp) ? ihp = imaxHp : ihp += addhp;
}

BOOL CHp::SubHp(INT subhp)
{
	ihp -= subhp;
	if (ihp <= 0) {
		ihp = 0;
		IsZero = TRUE;
	}
	return IsZero;
}

