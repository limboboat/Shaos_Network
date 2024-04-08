#pragma once

#include <WS2tcpip.h>
#include <Windows.h>
#include <commctrl.h>
#include <iostream>

#include "CGameObject.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000

using namespace std;

struct SendData {
	int playerID;

	UINT message;
	WPARAM wParam;
	LPARAM lParam;

	int iViewX;
};

enum RecvType {
	initObj, delObj, UpdateObj, PlayerData
};

struct RecvInitObj {
	int key;
	int objTAG;
	TEAM team;
	POINTFLOAT		mptpos;
};

struct RecvDelObj {
	int key;
	TEAM team;
};

struct RecvUpdateObj {
	int key;
	TEAM team;


	POINTFLOAT		mptpos;

	RECT			mrcRng;

	CHp				mhp;
	RECT			mrchpbar;
	BOOL			mdeath;

	INT iattackcooltime;
	INT ideatheffecttime;
};

struct addRecvPlayerInfo {
	BOOL R_On, L_On, U_On, D_On;
	BOOL pressQ, pressSft, onshield;


	UINT effecttime_AoE;
	UINT effecttime_Shoot;
	UINT effecttime_Return;

	UINT castingtime_return;
	UINT activetime_shield;

	UINT cooltime_Shoot;
	UINT cooltime_AoE;
	UINT cooltime_Shield;
	UINT cooltime_Return;
	UINT cooltime_Death;

	int	ptarget_key;

	POINT worldmousepos;

	BOOL immotal;



	POINT triangle1[3];
	POINT triangle2[3];
	POINT shootattackpt[7];
	RECT shootattackrange[7];
};

class CNetwork
{
public:
	static int playerID;

	//char* SERVERIP = (char*)"127.0.0.1";

	map<int, CGameObject*>& user_objlist;
	map<int, CGameObject*>& enemy_objlist;

	SOCKET sock;

public:
	CNetwork(map<int, CGameObject*>& user_objlist,
		map<int, CGameObject*>& enemy_objlist);
	~CNetwork();


	void SendToData(UINT, WPARAM, LPARAM, int);
	//static DWORD WINAPI SendToData(LPVOID arg);

	static DWORD WINAPI RecvByServer(LPVOID arg);

	// 소켓 함수 오류 출력 후 종료
	static void err_quit(const char* msg);
	// 소켓 함수 오류 출력
	static void err_display(const char* msg);
	// 소켓 함수 오류 출력
	static void err_display(int errcode);

};
