#include "KinectSDKforDxLib_v2.0.h"
#include <stdio.h>
#include <math.h>
#include <stdexcept>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	TCHAR t[100];//変換用

	//画面モードの変更
	DxLib::ChangeWindowMode(TRUE);
	const short width = 1600, height = 900;
	DxLib::SetGraphMode(width, height, 32);
	//ウィンドウの設定
	const char* Title = "BBYMYBABY";
	MultiByteToWideChar(CP_ACP, 0, Title, -1, t, MultiByteToWideChar(CP_ACP, 0, Title, -1, NULL, 0));
	DxLib::SetMainWindowText(t);
	DxLib::SetWindowSizeChangeEnableFlag(TRUE);

	if(DxLib::DxLib_Init() == -1){	// ＤＸライブラリ初期化処理{
		return -1;	// エラーが起きたら直ちに終了
	}
	// kinectオブジェクト生成
	KinectDX kinect;
	HANDLE depthFrameHandle;
	Vector4 user1[NUI_SKELETON_POSITION_COUNT];		// NUI_SKELETON_POSITION_COUNTは20（定数）
	Vector4 user2[NUI_SKELETON_POSITION_COUNT];
	if(kinect.m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, 0, 2, 0, &depthFrameHandle)!=S_OK){
		return -1;
		//throw std::runtime_error("NuiImageStreamOpen");
	}

	while(DxLib::ProcessMessage() == 0 && DxLib::CheckHitKeyAll() == 0){
		// 画面をクリア
		DxLib::ClearDrawScreen();
		// RGBカメラ表示
		DxLib::DrawGraph(0, 0, kinect.GetVideoGrHandle(), false);
		// トラッキング
		int count = kinect.GetSkeletonData(user1, user2);

		NUI_IMAGE_FRAME depthFrame = {0};
		if(kinect.m_pNuiSensor->NuiImageStreamGetNextFrame(depthFrameHandle, 0, &depthFrame) != S_OK){
			return -1;
			//throw std::runtime_error("NuiImageStreamGetNextFrame");
		}
		NUI_LOCKED_RECT depthData = {0};
		depthFrame.pFrameTexture->LockRect(0, &depthData, 0, 0);
		USHORT* depth = (USHORT*)depthData.pBits;

		for(int i = 0; i < (depthData.size / sizeof(USHORT)); ++i){
			// 距離
			USHORT distance = ::NuiDepthPixelToDepth(depth[i]);
			//座標
			LONG depthX = i % width;
			LONG depthY = i / width;
			if(distance > 300){
				const unsigned int color = DxLib::GetColor(255, 255, 255);
				DxLib::DrawPixel(depthX, depthY, color);
			}
		}

		if(kinect.m_pNuiSensor->NuiImageStreamReleaseFrame(depthFrameHandle, &depthFrame) != S_OK){
			return -1;
			//throw std::runtime_error("NuiImageStreamReleaseFrame");
		}

		// 裏画面の内容を表画面に反映
		DxLib::ScreenFlip();
	}

	DxLib::DxLib_End();		
	return 0;		
}