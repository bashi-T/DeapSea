#include"WindowApp.h"
#include"Debug.h"
#include"DX12Common.h"
const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
Vector4* vertexData = nullptr;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("HelloDirectX!\n");
	Mesh* mesh_ = new Mesh;
	DX12Common* dx12Common_ = new DX12Common;
	Debug* debug_ = new Debug;
	MSG NewMSG = WinAPP::GetMSG();
	WinAPP::Initialize(kWindowWidth, kWindowHeight);
	mesh_->Initialize(kWindowWidth, kWindowHeight);

	WinAPP::CreateWindowView(L"CG2");

#ifdef _DEBUG
	debug_->DebugLayer();
#endif
	dx12Common_->Init();

	while (NewMSG.message != WM_QUIT)
	{
		if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE))
		{
			WinAPP::ProcessMessage(NewMSG);
		}
		else
		{
			//mesh_->GetVertexResource()->Map(
			//	0, nullptr, reinterpret_cast<void**>(&vertexData));
			//vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };//左下
			//vertexData[1] = { 0.0f,0.0f,0.0f,1.0f };//上
			//vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };//右下

			dx12Common_->ClearScreen();
		}
	}
	dx12Common_->DX12Release(debug_->GetDebugController());
	debug_->ReportLiveObject();
	return 0;
}