#include "pch.h"
#include "CWorld.h"
#include "Sound.h"

#include "CNetwork.h"

int CWorld::user_manageID = 0;
int CWorld::enemy_manageID = 0;

// 게임시작을 누르면 네트워크를 실행
CNetwork* Network;

CWorld::CWorld(HWND hwnd)
{
	hbackgroundbmp = (HBITMAP)LoadImage(NULL, L"Resource/map.bmp", IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetClientRect(hwnd, &rcClient);

	// 출력 dc의 성질 가져오기
	HDC tmpdc = GetDC(hwnd);
	hUpdateDC = CreateCompatibleDC(tmpdc);
	hBackgroundDC = CreateCompatibleDC(tmpdc);
	hbackgroundsizebmp = CreateCompatibleBitmap(tmpdc, MAPSIZE_WIDTH, MAPSIZE_HEIGHT);
	ReleaseDC(hwnd, tmpdc);


	// hMapDC는 맵 이미지를 선택
	hbackgroundOld = (HBITMAP)SelectObject(hBackgroundDC, hbackgroundbmp);
	hupdateOld = (HBITMAP)SelectObject(hUpdateDC, hbackgroundsizebmp);


	// 팀 할당받기
	pUserTeam = new CUser(user_objlist, enemy_objlist);
	pEnemyTeam = new CEnemy(enemy_objlist, user_objlist);

	// 초기 오브젝트 할당

	pUserTeam->SetInitObj();
	pEnemyTeam->SetInitObj();

	// 네트워크 생성
	Network = new CNetwork(user_objlist, enemy_objlist);

	gamestate = 0;	// 진행중
}


CWorld::~CWorld()
{
	// 비트맵 삭제
	DeleteObject(SelectObject(hUpdateDC, hbackgroundsizebmp));	// 도화지 삭제
	DeleteObject(SelectObject(hBackgroundDC, hbackgroundOld));	// 배경 삭제

	// dc 삭제
	DeleteDC(hUpdateDC);
	DeleteDC(hBackgroundDC);


	// 팀 해제
	delete pUserTeam;
	delete pEnemyTeam;

	delete Network;
}

void CWorld::MSG_Mouse(int playerID, UINT message, POINT clientmousepos, int iViewX)
{
	POINT worldmousepos =
	{ clientmousepos.x + iViewX - MIN_VIEWX,clientmousepos.y };

	pUserTeam->MSG_Mouse(playerID, message, worldmousepos);

}

void CWorld::MSG_Key(int playerID, UINT message, WPARAM wParam, LPARAM lParam)
{
	pUserTeam->MSG_Key(playerID, message, wParam);
}

void CWorld::Update()
{
	// 타워 죽음 이펙트가 끝났을 때
	if (pUserTeam->gameover) {
		if (gamestate == 0) {
			sound->MyPlaySound(5, 4);// 사운드 넣기 

			// 게임오버 되면 최초에 한번 데이터 한번 더 보낸다.
			for (int i = 0; i < 3; ++i) {
				HANDLE hThread = CreateThread(NULL, 0, CNetwork::SendObjects,
					(LPVOID)CNetwork::client_sock[i], 0, NULL);
				if (hThread == NULL) { closesocket(CNetwork::client_sock[i]); }
				else { CloseHandle(hThread); }
			}
		}
		gamestate = 2;
		//iViewX = 600;
		return;
	}
	else if (pEnemyTeam->gameover)
	{
		if (gamestate == 0) {
			sound->MyPlaySound(6, 4); // 사운드 넣기 

			// 게임오버 되면 최초에 한번 데이터 한번 더 보낸다.
			for (int i = 0; i < 3; ++i) {
				HANDLE hThread = CreateThread(NULL, 0, CNetwork::SendObjects,
					(LPVOID)CNetwork::client_sock[i], 0, NULL);
				if (hThread == NULL) { closesocket(CNetwork::client_sock[i]); }
				else { CloseHandle(hThread); }
			}
		}
		gamestate = 1;
		//iViewX == 3100;
		return;
	}

	pUserTeam->Update();
	pEnemyTeam->Update();

	// 타워가 죽었을 때
	if (user_objlist[0]->IsDead()) {
		// 타워 죽었을 때 효과음
		iViewX = 600;
		return;
	}
	else if (enemy_objlist[0]->IsDead()) {
		// 타워 죽었을 때 효과음
		iViewX = 3100;
		return;
	}

	if (pUserTeam->GeteffecttimeAoE()) {
		// 15프레임
		iViewX = (INT)SET_VIEWX(pUserTeam->GetPlayerPos().x);

		switch (pUserTeam->GeteffecttimeAoE()) {
		case FRAMETIME * 15:
		case FRAMETIME * 14:
		case FRAMETIME * 13:
		case FRAMETIME * 9:
		case FRAMETIME * 8:
		case FRAMETIME * 7:
		case FRAMETIME * 3:
		case FRAMETIME * 2:
		case FRAMETIME * 1:
			iViewX += 3;
			break;
		case FRAMETIME * 12:
		case FRAMETIME * 11:
		case FRAMETIME * 10:
		case FRAMETIME * 6:
		case FRAMETIME * 5:
		case FRAMETIME * 4:
			iViewX -= 3;
			break;
		default:
			break;
		}
		return;
	}

	iViewX = (INT)SET_VIEWX(pUserTeam->GetPlayerPos().x);

	//업데이트 한번 할 때마다 오브젝트 정보 보낸다.
	for (int i = 0; i < 3; ++i) {
		HANDLE hThread = CreateThread(NULL, 0, CNetwork::SendObjects,
			(LPVOID)CNetwork::client_sock[i], 0, NULL);
		if (hThread == NULL) { closesocket(CNetwork::client_sock[i]); }
		else { CloseHandle(hThread); }
	}
}

void CWorld::Draw(HDC clientDC)
{
	BitBlt(hUpdateDC, 0, 0, MAPSIZE_WIDTH, MAPSIZE_HEIGHT,
		hBackgroundDC, 0, 0, SRCCOPY);

	HBRUSH hOld = (HBRUSH)SelectObject(hUpdateDC, hUSERUNITBRUSH);
	// 객체 그리기
	pUserTeam->Draw(hUpdateDC);
	SelectObject(hUpdateDC, hENEMYUNITBRUSH);
	pEnemyTeam->Draw(hUpdateDC);
	SelectObject(hUpdateDC, hOld);


	//----
	//if(pUserTeam->GetMyObjList().front()->IsDead())
	//	iViewX = 600;
	//if (pEnemyTeam->GetMyObjList().front()->IsDead())
	//	iViewX = 3100;
	if (user_objlist[0]->IsDead())
		iViewX = 600;
	if (enemy_objlist[0]->IsDead())
		iViewX = 3100;

	BitBlt(clientDC, 0, 0, rcClient.right, rcClient.bottom,
		hUpdateDC, iViewX - MIN_VIEWX, 0, SRCCOPY);
}

void CWorld::SetSound(CSound* asound)
{
	sound = asound;
	pUserTeam->SetSound(asound);
}

void CWorld::GetUIInfo(INT* ahp, INT* ct_shoot, INT* ct_AoE,
	INT* ct_shield, INT* ct_return, INT* towerhp, INT* ct_death)
{
	pUserTeam->GetUIInfo(ahp, ct_shoot, ct_AoE, ct_shield, ct_return, towerhp, ct_death);
}

INT CWorld::IsEnding()
{
	return gamestate;
}

void CWorld::OffPlayerMove()
{
	pUserTeam->OffPlayerMove();
}
