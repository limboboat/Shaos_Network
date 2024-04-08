#include "pch.h"
#include "CNetwork.h"

#include "CRect.h"
#include "CEllip.h"
#include "CDia.h"
#include "CTri.h"

#include "CPlayer.h"

//SendData tempData;
extern CNetwork* Network;

int CNetwork::playerID = -1;

CNetwork::CNetwork(map<int, CGameObject*>& user_objlist, map<int, CGameObject*>& enemy_objlist)
	: user_objlist{ user_objlist }, enemy_objlist{ enemy_objlist }
{
	string SERVERIP;
	cout << "서버 IP 입력 : ";
	cin >> SERVERIP;

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP.c_str(), &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 플레이어 아이디 받기
	retval = recv(sock, (char*)&playerID, sizeof(playerID), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		//break;
	}


	HANDLE hThread = CreateThread(NULL, 0, RecvByServer,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }
}

CNetwork::~CNetwork()
{
	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
}

void CNetwork::SendToData(UINT message, WPARAM wParam, LPARAM lParam, int iViewX)
{
	SendData tempData = { playerID, message, wParam, lParam , iViewX};

	// 데이터 보내기(고정 길이)
	// send (소켓, 데이터(char*), 크기, 0);
	int retval = send(sock, (char*)&tempData, sizeof(tempData), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}
}

// 클라이언트와 데이터 통신
DWORD WINAPI CNetwork::RecvByServer(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (true) {
		// 데이터 받기 (데이터 타입)
		int DataType;
		retval = recv(client_sock, (char*)&DataType, sizeof(DataType), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			//break;
		}

		switch (DataType) {
		case RecvType::delObj: {
			RecvDelObj temp;
			retval = recv(client_sock, (char*)&temp, sizeof(temp), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//break;
			}
			if (temp.team == TEAM::USER) {
				delete Network->user_objlist[temp.key];
				Network->user_objlist.erase(temp.key);

				// 오류방지(실패)
				((CPlayer*)Network->user_objlist[1])->ptarget_key = -1;
				((CPlayer*)Network->user_objlist[2])->ptarget_key = -1;
				((CPlayer*)Network->user_objlist[3])->ptarget_key = -1;
			}
			else if (temp.team == TEAM::ENEMY) {
				delete Network->enemy_objlist[temp.key];
				Network->enemy_objlist.erase(temp.key);
			}
		}
							 break;

		case RecvType::initObj: {
			RecvInitObj temp;
			retval = recv(client_sock, (char*)&temp, sizeof(temp), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				//break;
			}
			if (temp.team == TEAM::USER) {
				if (Network)
					switch (temp.objTAG) {
					case OTRI:
						Network->user_objlist.insert({ temp.key, new CTri(temp.mptpos, TEAM::USER, Network->enemy_objlist) });
						//Network->user_objlist[temp.key] = new CTri(temp.mptpos, TEAM::USER, Network->enemy_objlist);
						break;
					case ORECT:
						Network->user_objlist.insert({ temp.key, new CRect(temp.mptpos, TEAM::USER, Network->enemy_objlist) });
						//Network->user_objlist[temp.key] = new CRect(temp.mptpos, TEAM::USER, Network->enemy_objlist);
						break;
					case OELLIP:
						Network->user_objlist.insert({ temp.key, new CEllip(temp.mptpos, TEAM::USER, Network->enemy_objlist) });
						//Network->user_objlist[temp.key] = new CEllip(temp.mptpos, TEAM::USER, Network->enemy_objlist);
						break;
					case ODIA:
						Network->user_objlist.insert({ temp.key, new CDia(temp.mptpos, TEAM::USER, Network->enemy_objlist) });
						//Network->user_objlist[temp.key] = new CDia(temp.mptpos, TEAM::USER, Network->enemy_objlist);
						break;
					}
			}
			else if (temp.team == TEAM::ENEMY) {
				if (Network)
					switch (temp.objTAG) {
					case OTRI:
						Network->enemy_objlist.insert({ temp.key, new CTri(temp.mptpos, TEAM::ENEMY, Network->user_objlist) });
						//Network->enemy_objlist[temp.key] = new CTri(temp.mptpos, TEAM::ENEMY, Network->user_objlist);
						break;
					case ORECT:
						Network->enemy_objlist.insert({ temp.key, new CRect(temp.mptpos, TEAM::ENEMY, Network->user_objlist) });
						//Network->enemy_objlist[temp.key] = new CRect(temp.mptpos, TEAM::ENEMY, Network->user_objlist);
						break;
					case OELLIP:
						Network->enemy_objlist.insert({ temp.key, new CEllip(temp.mptpos, TEAM::ENEMY, Network->user_objlist) });
						//Network->enemy_objlist[temp.key] = new CEllip(temp.mptpos, TEAM::ENEMY, Network->user_objlist);
						break;
					case ODIA:
						Network->enemy_objlist.insert({ temp.key, new CDia(temp.mptpos, TEAM::ENEMY, Network->user_objlist) });
						//Network->enemy_objlist[temp.key] = new CDia(temp.mptpos, TEAM::ENEMY, Network->user_objlist);
						break;
					}
			}

		}
							  break;
		case RecvType::UpdateObj: {
				int dataSize;
				retval = recv(client_sock, (char*)&dataSize, sizeof(dataSize), MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					//break;
				}
				for (int i = 0; i < dataSize; ++i) {
					RecvUpdateObj temp;
					retval = recv(client_sock, (char*)&temp, sizeof(temp), MSG_WAITALL);
					if (retval == SOCKET_ERROR) {
						err_display("recv()");
						//break;
					}
					if (temp.team == TEAM::USER) {
						if (Network->user_objlist.find(temp.key) != Network->user_objlist.end()) {
							Network->user_objlist[temp.key]->mptpos = temp.mptpos;
							Network->user_objlist[temp.key]->mrcRng = temp.mrcRng;
							Network->user_objlist[temp.key]->mhp = temp.mhp;
							Network->user_objlist[temp.key]->mrchpbar = temp.mrchpbar;
							Network->user_objlist[temp.key]->mdeath = temp.mdeath;
							Network->user_objlist[temp.key]->iattackcooltime = temp.iattackcooltime;
							Network->user_objlist[temp.key]->ideatheffecttime = temp.ideatheffecttime;
						}
					}
					else if (temp.team == TEAM::ENEMY) {
						if (Network->enemy_objlist.find(temp.key) != Network->enemy_objlist.end()) {
							Network->enemy_objlist[temp.key]->mptpos = temp.mptpos;
							Network->enemy_objlist[temp.key]->mrcRng = temp.mrcRng;
							Network->enemy_objlist[temp.key]->mhp = temp.mhp;
							Network->enemy_objlist[temp.key]->mrchpbar = temp.mrchpbar;
							Network->enemy_objlist[temp.key]->mdeath = temp.mdeath;
							Network->enemy_objlist[temp.key]->iattackcooltime = temp.iattackcooltime;
							Network->enemy_objlist[temp.key]->ideatheffecttime = temp.ideatheffecttime;
						}
					}
				}
			}
			break;

		case RecvType::PlayerData: {
			for (int i = 0; i < 3; ++i) {
				addRecvPlayerInfo temp;
				retval = recv(client_sock, (char*)&temp, sizeof(temp), MSG_WAITALL);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					//break;
				}
				((CPlayer*)Network->user_objlist[i + 1])->R_On = temp.R_On;
				((CPlayer*)Network->user_objlist[i + 1])->L_On = temp.L_On;
				((CPlayer*)Network->user_objlist[i + 1])->U_On = temp.U_On;
				((CPlayer*)Network->user_objlist[i + 1])->D_On = temp.D_On;
				((CPlayer*)Network->user_objlist[i + 1])->pressQ = temp.pressQ;
				((CPlayer*)Network->user_objlist[i + 1])->pressSft = temp.pressSft;
				((CPlayer*)Network->user_objlist[i + 1])->onshield = temp.onshield;

				((CPlayer*)Network->user_objlist[i + 1])->effecttime_AoE = temp.effecttime_AoE;
				((CPlayer*)Network->user_objlist[i + 1])->effecttime_Shoot = temp.effecttime_Shoot;
				((CPlayer*)Network->user_objlist[i + 1])->effecttime_Return = temp.effecttime_Return;

				((CPlayer*)Network->user_objlist[i + 1])->castingtime_return = temp.castingtime_return;
				((CPlayer*)Network->user_objlist[i + 1])->activetime_shield = temp.activetime_shield;

				((CPlayer*)Network->user_objlist[i + 1])->cooltime_Shoot = temp.cooltime_Shoot;
				((CPlayer*)Network->user_objlist[i + 1])->cooltime_AoE = temp.cooltime_AoE;
				((CPlayer*)Network->user_objlist[i + 1])->cooltime_Shield = temp.cooltime_Shield;
				((CPlayer*)Network->user_objlist[i + 1])->cooltime_Return = temp.cooltime_Return;
				((CPlayer*)Network->user_objlist[i + 1])->cooltime_Death = temp.cooltime_Death;

				((CPlayer*)Network->user_objlist[i + 1])->ptarget_key = temp.ptarget_key;

				((CPlayer*)Network->user_objlist[i + 1])->worldmousepos = temp.worldmousepos;

				((CPlayer*)Network->user_objlist[i + 1])->immotal = temp.immotal;


				for(int k=0;k<7;++k)
					((CPlayer*)Network->user_objlist[i + 1])->shootattackpt[k] = temp.shootattackpt[k];
				for(int k=0;k<7;++k)
					((CPlayer*)Network->user_objlist[i + 1])->shootattackrange[k] = temp.shootattackrange[k];
				for(int k=0;k<3;++k)
					((CPlayer*)Network->user_objlist[i + 1])->triangle1[k] = temp.triangle1[k];
				for(int k=0;k<3;++k)
					((CPlayer*)Network->user_objlist[i + 1])->triangle2[k] = temp.triangle2[k];
			}
		}
								 break;
		}

	}

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}

// 소켓 함수 오류 출력 후 종료
void CNetwork::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void CNetwork::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void CNetwork::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
