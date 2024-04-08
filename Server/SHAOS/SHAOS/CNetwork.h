#pragma once

//#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
//#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <WS2tcpip.h>
#include <Windows.h>
#include <commctrl.h>
#include <iostream>

#include "CGameObject.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000

using namespace std;

struct RecvData {
	int playerID;

	UINT message;
	WPARAM wParam;
	LPARAM lParam;

	int iViewX;
};

enum SendType {
	initObj, delObj, UpdateObj, PlayerData
};

struct SendInitObj {
	int key;
	int objTAG;
	TEAM team;
	POINTFLOAT		mptpos;
};

struct SendDelObj {
	int key;
	TEAM team;
};

struct SendUpdateObj {
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

struct addSendPlayerInfo {
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
	map<int, CGameObject*>& user_objlist;
	map<int, CGameObject*>& enemy_objlist;

public:
	static SOCKET listen_sock;
	static SOCKET client_sock[3];
	static HANDLE hSendEvent;

public:
	CNetwork(map<int, CGameObject*>& user_objlist,
		map<int, CGameObject*>& enemy_objlist);
	~CNetwork();



	static DWORD WINAPI RecvByClient(LPVOID arg);
	void SendDel(SendDelObj data);
	void SendInit(SendInitObj data);
	static DWORD WINAPI SendObjects(LPVOID arg);

	// 소켓 함수 오류 출력 후 종료
	static void err_quit(const char* msg);
	// 소켓 함수 오류 출력
	static void err_display(const char* msg);
	// 소켓 함수 오류 출력
	static void err_display(int errcode);

};
