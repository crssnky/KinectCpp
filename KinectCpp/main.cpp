#include "KinectSDKforDxLib_v2.0.h"
#include <stdio.h>
#include <math.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	TCHAR t[100];//変換用

	//画面モードの変更
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(1600, 900, 32);
	//ウィンドウの設定
	const char* Title = "BBYMYBABY";
	MultiByteToWideChar(CP_ACP, 0, Title, -1, t, MultiByteToWideChar(CP_ACP, 0, Title, -1, NULL, 0));
	DxLib::SetMainWindowText(t);
	DxLib::SetWindowSizeChangeEnableFlag(TRUE);

	if(DxLib::DxLib_Init() == -1){	// ＤＸライブラリ初期化処理{
		return -1;	// エラーが起きたら直ちに終了
	}

	while(DxLib::ProcessMessage() == 0 && DxLib::CheckHitKeyAll() == 0){

	}
}