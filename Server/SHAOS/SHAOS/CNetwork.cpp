#include "pch.h"
#include "CNetwork.h"

#include "GameFramework.h"
#include "CPlayer.h"
extern CGameFramework g_GameFrameWork;
extern CNetwork* Network;

SOCKET CNetwork::client_sock[3]{};
SOCKET CNetwork::listen_sock = 0;
HANDLE CNetwork::hSendEvent{};

CNetwork::CNetwork(map<int, CGameObject*>& user_objlist, map<int, CGameObject*>& enemy_objlist)
	: user_objlist{ user_objlist }, enemy_objlist{ enemy_objlist }
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// 이벤트 생성
	hSendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	//SOCKET client_sock;	//스태틱 사용 -> 플레이어 늘릴때 다시 생각해봐야 한다.
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	for (int playerID = 0; playerID < 3; ++playerID) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock[playerID] = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock[playerID] == INVALID_SOCKET) {
			err_display("accept()");
			//break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 플레이어 아이디 전송
		retval = send(client_sock[playerID], (char*)&playerID, sizeof(playerID), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}

		// 키입력 받는 스레드 생성
		hThread = CreateThread(NULL, 0, RecvByClient,
			(LPVOID)client_sock[playerID], 0, NULL);
		if (hThread == NULL) { closesocket(client_sock[playerID]); }
		else { CloseHandle(hThread); }
	}
}

CNetwork::~CNetwork()
{
	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
}

// 클라이언트와 데이터 통신
DWORD WINAPI CNetwork::RecvByClient(LPVOID arg)
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

	RecvData tempData;
	while (true) {
		// 데이터 받기(파일 이름)
		retval = recv(client_sock, (char*)&tempData, sizeof(tempData), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}
		switch (tempData.message) {
		case WM_KEYDOWN:
		case WM_KEYUP:
			g_GameFrameWork.MSG_Key(tempData.playerID, tempData.message, tempData.wParam, tempData.lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
			g_GameFrameWork.MSG_Mouse(tempData.playerID, tempData.message, tempData.wParam, tempData.lParam, tempData.iViewX);
			break;
		}
	}
}

void CNetwork::SendDel(SendDelObj data)
{
	WaitForSingleObject(hSendEvent, INFINITE);		// 전송 완료 대기

	int type = SendType::delObj;

	for (int i = 0; i < 3; ++i) {
		int retval = send(client_sock[i], (char*)&type, sizeof(type), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}

		retval = send(client_sock[i], (char*)&data, sizeof(data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
	}

	SetEvent(hSendEvent);			// 전송 완료 알림
}

void CNetwork::SendInit(SendInitObj data)
{
	WaitForSingleObject(hSendEvent, INFINITE);		// 전송 완료 대기

	int type = SendType::initObj;

	for (int i = 0; i < 3; ++i) {
		int retval = send(client_sock[i], (char*)&type, sizeof(type), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}

		retval = send(client_sock[i], (char*)&data, sizeof(data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
	}

	SetEvent(hSendEvent);			// 전송 완료 알림
}

DWORD WINAPI CNetwork::SendObjects(LPVOID arg) {

	WaitForSingleObject(hSendEvent, INFINITE);		// 전송 완료 대기

	int retval;

	SOCKET client_sock = (SOCKET)arg;

	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


	// 유저리스트 보내기
	int type = SendType::UpdateObj;

	retval = send(client_sock, (char*)&type, sizeof(type), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	int userDataSize = Network->user_objlist.size();
	retval = send(client_sock, (char*)&userDataSize, sizeof(userDataSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	for (auto tmp : Network->user_objlist) {
		SendUpdateObj temp;
		temp.key = tmp.first;
		temp.team = TEAM::USER;
		temp.mptpos = tmp.second->mptpos;
		temp.mrcRng = tmp.second->mrcRng;
		temp.mhp = tmp.second->mhp;
		temp.mrchpbar = tmp.second->mrchpbar;
		temp.mdeath = tmp.second->mdeath;
		temp.iattackcooltime = tmp.second->iattackcooltime;
		temp.ideatheffecttime = tmp.second->ideatheffecttime;

		retval = send(client_sock, (char*)&temp, sizeof(temp), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
	}


	// 적팀리스트 보내기
	type = SendType::UpdateObj;

	retval = send(client_sock, (char*)&type, sizeof(type), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	userDataSize = Network->enemy_objlist.size();
	retval = send(client_sock, (char*)&userDataSize, sizeof(userDataSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	for (auto tmp : Network->enemy_objlist) {
		SendUpdateObj temp;
		temp.key = tmp.first;
		temp.team = TEAM::ENEMY;
		temp.mptpos = tmp.second->mptpos;
		temp.mrcRng = tmp.second->mrcRng;
		temp.mhp = tmp.second->mhp;
		temp.mrchpbar = tmp.second->mrchpbar;
		temp.mdeath = tmp.second->mdeath;
		temp.iattackcooltime = tmp.second->iattackcooltime;
		temp.ideatheffecttime = tmp.second->ideatheffecttime;

		retval = send(client_sock, (char*)&temp, sizeof(temp), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
	}


	// 플레이어 데이터 전송
	type = SendType::PlayerData;

	retval = send(client_sock, (char*)&type, sizeof(type), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}
	for (int j = 0; j < 3; ++j) {
		addSendPlayerInfo temp;
		temp.ptarget_key = ((CPlayer*)Network->user_objlist[j + 1])->ptarget_key;
		temp.effecttime_AoE = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_AoE;
		temp.effecttime_Shoot = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_Shoot;
		temp.effecttime_Return = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_Return;

		temp.R_On = ((CPlayer*)Network->user_objlist[j + 1])->R_On;
		temp.L_On = ((CPlayer*)Network->user_objlist[j + 1])->L_On;
		temp.U_On = ((CPlayer*)Network->user_objlist[j + 1])->U_On;
		temp.D_On = ((CPlayer*)Network->user_objlist[j + 1])->D_On;
		temp.pressQ = ((CPlayer*)Network->user_objlist[j + 1])->pressQ;
		temp.pressSft = ((CPlayer*)Network->user_objlist[j + 1])->pressSft;
		temp.onshield = ((CPlayer*)Network->user_objlist[j + 1])->onshield;

		temp.effecttime_AoE = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_AoE;
		temp.effecttime_Shoot = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_Shoot;
		temp.effecttime_Return = ((CPlayer*)Network->user_objlist[j + 1])->effecttime_Return;

		temp.castingtime_return = ((CPlayer*)Network->user_objlist[j + 1])->castingtime_return;
		temp.activetime_shield = ((CPlayer*)Network->user_objlist[j + 1])->activetime_shield;

		temp.cooltime_Shoot = ((CPlayer*)Network->user_objlist[j + 1])->cooltime_Shoot;
		temp.cooltime_AoE = ((CPlayer*)Network->user_objlist[j + 1])->cooltime_AoE;
		temp.cooltime_Shield = ((CPlayer*)Network->user_objlist[j + 1])->cooltime_Shield;
		temp.cooltime_Return = ((CPlayer*)Network->user_objlist[j + 1])->cooltime_Return;
		temp.cooltime_Death = ((CPlayer*)Network->user_objlist[j + 1])->cooltime_Death;

		temp.ptarget_key = ((CPlayer*)Network->user_objlist[j + 1])->ptarget_key;

		temp.worldmousepos = ((CPlayer*)Network->user_objlist[j + 1])->worldmousepos;

		temp.immotal = ((CPlayer*)Network->user_objlist[j + 1])->immotal;



		for (int k = 0; k < 7; ++k)
			temp.shootattackpt[k] = ((CPlayer*)Network->user_objlist[j + 1])->shootattackpt[k];
		for (int k = 0; k < 7; ++k)
			temp.shootattackrange[k] = ((CPlayer*)Network->user_objlist[j + 1])->shootattackrange[k];
		for (int k = 0; k < 3; ++k)
			temp.triangle1[k] = ((CPlayer*)Network->user_objlist[j + 1])->triangle1[k];
		for (int k = 0; k < 3; ++k)
			temp.triangle2[k] = ((CPlayer*)Network->user_objlist[j + 1])->triangle2[k];


		retval = send(client_sock, (char*)&temp, sizeof(temp), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
	}

	SetEvent(hSendEvent);			// 전송 완료 알림
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
