#include"WindowApp.h"
#include"Debug.h"
#include"Mesh.h"

const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const int32_t kNumTriangle = 2;
Vector4* vertexData = nullptr;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	CoInitializeEx(0, COINIT_MULTITHREADED);
	Mesh* mesh[kNumTriangle];
	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i] = new Mesh;
	}

	DX12Common* dx12Common = DX12Common::GetInstance();
	Debug* debug = new Debug;
	WinAPP* winAPP = WinAPP::GetInstance();
	MSG NewMSG = winAPP->GetMSG();
	MyImGui* imgui = new MyImGui;
	bool useWorldMap = true;

	winAPP->Initialize(kWindowWidth, kWindowHeight);
	winAPP->CreateWindowView(L"CG2");

#ifdef _DEBUG
	debug->DebugLayer();
#endif

	dx12Common->Init("Resource/uvChecker.png", kWindowWidth, kWindowHeight);
	imgui->Initialize(
	    winAPP->GetHWND(),
		dx12Common->GetInstance()->GetDevice(),
	    dx12Common->GetInstance()->GetSwapChainDesc(),
	    dx12Common->GetInstance()->GetRtvDesc(),
	    dx12Common->GetInstance()->GetSrvDescriptorHeap());

	Vector4 Top[kNumTriangle];
	Vector4 Left[kNumTriangle];
	Vector4 Right[kNumTriangle];
	Vector4 Color[kNumTriangle];
	Vector2 texcoordTop[kNumTriangle];
	Vector2 texcoordLeft[kNumTriangle];
	Vector2 texcoordRight[kNumTriangle];

	Top[0] = { 0.0f, 0.5f, 0.0f, 1.0f };
	Right[0] = { 0.5f, -0.5f, 0.0f, 1.0f };
	Left[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	Color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };
	texcoordTop[0] = { 0.5f,0.0f };
	texcoordRight[0] = { 1.0f,1.0f };
	texcoordLeft[0] = { 0.0f,1.0f };

	Vector4 LeftTop[kNumTriangle];
	Vector4 RightTop[kNumTriangle];
	Vector4 RightBottom[kNumTriangle];
	Vector4 LeftBottom[kNumTriangle];
	Vector4 ColorSprite[kNumTriangle];
	Vector2 texcoordLeftTop[kNumTriangle];
	Vector2 texcoordRightTop[kNumTriangle];
	Vector2 texcoordRightBottom[kNumTriangle];
	Vector2 texcoordLeftBottom[kNumTriangle];

	LeftTop[0] = {0.0f, 0.0f, 0.0f, 1.0f};
	RightTop[0] = { kWindowWidth/3, 0.0f, 0.0f, 1.0f};
	RightBottom[0] = { kWindowWidth/3, kWindowHeight/3, 0.0f, 1.0f};
	LeftBottom[0] = { 0.0f, kWindowHeight/3, 0.0f, 1.0f};
	ColorSprite[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
	texcoordLeftTop[0] = {0.0f, 0.0f};
	texcoordRightTop[0] = {1.0f, 0.0f};
	texcoordRightBottom[0] = {1.0f, 1.0f};
	texcoordLeftBottom[0] = {0.0f, 1.0f};

	
	Sphere sphere = { { 0.0f,0.0f,0.0f },1.0f };
	

	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i]->Initialize(kWindowWidth, kWindowHeight);
	}

	while (NewMSG.message != WM_QUIT)
	{
		
		imgui->Update();

		for (int i = 0; i < kNumTriangle; i++) {
			mesh[i]->Update();
		}
		ImGui::Begin("TriangleColor");
		//ImGui::DragFloat3("CameraTranslate", &mesh[0]->GetCameraMatrix().m[3][0], 0.01f);
		//ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::ColorEdit3("Triangle1", (float*)&Color[0]);
		//ImGui::ColorEdit3("Triangle2", (float*)&Color[1]);
		ImGui::ColorEdit3("Sprite1", (float*)&ColorSprite[0]);
		//ImGui::DragFloat3("sphere", (float*)&sphere.center,0.01f);
		ImGui::Checkbox("useWorldMap", &useWorldMap);
		ImGui::End();

				if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE)) {
			winAPP->ProcessMessage(NewMSG);
			ImGui::Render();
		} else{
			dx12Common->DrawScreen();

			ID3D12DescriptorHeap* descriptorHeaps[] =
			{
			    dx12Common->GetInstance()->GetSrvDescriptorHeap()
			};
			dx12Common->GetInstance()->GetCommandList()->
				SetDescriptorHeaps(1, descriptorHeaps);

			for (int i = 0; i < kNumTriangle; i++) {
				mesh[i]->DrawTriangle(
					Top[i],
					Right[i],
					Left[i],
					Color[i],
					texcoordTop[i],
					texcoordRight[i],
					texcoordLeft[i],
					useWorldMap);
			}
				mesh[0]->DrawSprite(
					LeftTop[0],
					RightTop[0],
					RightBottom[0],
					LeftBottom[0],
					ColorSprite[0],
					texcoordLeftTop[0],
					texcoordRightTop[0],
					texcoordRightBottom[0],
					texcoordLeftBottom[0]);

				mesh[0]->DrawSphere(sphere, Color[0],useWorldMap);
			imgui->Endframe(dx12Common->GetInstance()->GetCommandList());
			dx12Common->ClearScreen();
		}
	}

	CloseHandle(dx12Common->GetFenceEvent());
	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i]->MeshRelease();
	}
	dx12Common->DX12Release(debug->GetDebugController());
	debug->ReportLiveObject();
	CoUninitialize();
	return 0;
}