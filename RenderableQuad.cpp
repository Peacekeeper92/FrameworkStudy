#include "stdafx.h"
#include "RenderableQuad.h"

RenderableQuad::RenderableQuad()
{

	TRS = AddComponent<Transform>();

	QuadTex = new UATexture3D();

	auto Result = CreateUnorderedAccessTexture3D(DXGI_FORMAT::DXGI_FORMAT_R32_UINT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 128, 128, 128, QuadTex);
	auto Memory = MemoryBank::GetInstance();


	CO.BindUAV(QuadTex->UAV.Get(), 0);

}

RenderableQuad::~RenderableQuad()
{
}

void RenderableQuad::Init()
{


}

void RenderableQuad::Update(float Delta)
{
	auto Context = D3DHardware::GetInstance().GetContext();
	auto CS = CO.CS.Get();

	Context->CSSetShader(CS, nullptr, 0);
	Context->CSSetUnorderedAccessViews(0, 1, CO.RegisterU[0].GetAddressOf(), nullptr);

	Context->Dispatch(32, 32, 32);
	auto SC = D3DHardware::GetInstance().GetSwapChain();
	SC->Present(0, 0);

	DebugLog(L_NORMAL, "Renderable Quad, compute shader has been dispatched.");

}

void RenderableQuad::Render(float Delta)
{
}

void RenderableQuad::Release()
{
}


