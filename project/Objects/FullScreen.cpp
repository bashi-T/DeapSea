#include "Sprite.h"
#include"Commons/SpriteCommon.h"
#include "FullScreen.h"

FullScreen::~FullScreen()
{
}

void FullScreen::Initialize(SpriteCommon* spriteCommon, SRVManager* srvManager, std::string texturefilePath)
{
}

void FullScreen::Update()
{
}

void FullScreen::Draw(SpriteCommon* spriteCommon)
{
}

ComPtr<ID3D12Resource> FullScreen::CreateBufferResource(SpriteCommon* spriteCommon, size_t sizeInBytes)
{
	return ComPtr<ID3D12Resource>();
}

void FullScreen::MakeBufferView()
{
}

void FullScreen::InputData(Vector4 color)
{
}

ComPtr<ID3D12Resource> FullScreen::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	return ComPtr<ID3D12Resource>();
}
