#include "stdafx.h"
#include "Application.h"

shared_ptr<Application> Application::Inst = nullptr;

void Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	ImGui::StyleColorsDark();


}

bool Application::Init()
{
	Hardware = &D3DHardware::GetInstance(WindowHandle, WindowInstance);
	Renderer = &ImmediateRenderer::GetInstance();
	Input = D3DHardware::GetInstance().GetInputManager();

	auto Memory = MemoryBank::GetInstance();

	Memory->RegisterComponent<MeshRenderer>();
	Memory->RegisterComponent<Camera>();
	Memory->RegisterComponent<Transform>();

	Memory->AssignMaterialPass("Assets/Shaders/SampleShader.hlsl", "Sample");
	Memory->AssignMaterialPass("Assets/Shaders/Skybox.hlsl", "Skybox");

	if (!Hardware || !Renderer)
		AssertCritical("Hardware or Renderer didn't initialized!", E_INVALIDARG);

	// Swapchain Buffer
	Renderer->AddTexture2D(Renderer->GetBufferFromSwapChain());

	// Render Target #0
	Renderer->AddTexture2D(CreateTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, Width, Height));
	Renderer->AddDepthTexture(CreateDepthStencil(Width, Height));

	// Clear Swapchain buffer
	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Aqua);
	Renderer->ClearDepthStencil(Renderer->GetDepthStencils()[0]);
	Renderer->SetRenderTarget(Renderer->GetTextures2D(), 1, Renderer->GetDepthStencils()[0]);

	Renderer->AddViewport(CD3D11_VIEWPORT(0.0f, 0.0f, Width, Height));
	auto SceneID = Memory->AddScene(new Scene());
	SelectedScene = Memory->GetScene(SceneID);
	SelectedScene->SetName("Testing Scene");

	auto CameraID = SelectedScene->AddInstance(new Instance());

	Instance* CameraInst = SelectedScene->FindInstanceWithIID(CameraID);

	CameraInst->AddComponent<Camera>();

	SelectedScene->AddInstance(new SkullObject());
	SelectedScene->AddInstance(new Skybox());

	SelectedScene->AddSpotLight(XMVectorSet(20.0f, 30.0f, 0.0f, 1.0f), 25.0f);
	//SelectedScene->AddSpotLight(XMVectorSet(0.0f, 10.0f, 0.0f, 1.0f), 10.0f);

	SelectedScene->AddDirectionalLight(
		XMVector4Normalize(XMVectorSet(-5.0f, -5.0f, 0.0f, 1.0f)
						   - XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)), 0.6f);

	SelectedScene->Init();

	InitImGui();
	ImGui_ImplDX11_Init(Hardware->GetDevice(), Hardware->GetContext());

	ImGui_ImplWin32_Init(WindowHandle);

	auto MainCam = SelectedScene->GetMainCamera();
	auto ForwardBind = [this, MainCam]() { CameraForward(MainCam); };
	auto BackwardBind = [this, MainCam]() { CameraBackward(MainCam); };
	auto LeftBind = [this, MainCam]() { CameraLeft(MainCam); };
	auto RightBind = [this, MainCam]() { CameraRight(MainCam); };
	auto UpBind = [this, MainCam]() { CameraUp(MainCam); };
	auto DownBind = [this, MainCam]() { CameraDown(MainCam); };
	
	auto PitchBind = [this, MainCam](int dx, int dy) {CameraRotateOrbitPitch(MainCam,dy ); };
	auto YawBind = [this, MainCam](int dx, int dy) {CameraRotateOrbitYaw(MainCam,dx); };

	Input->AddTask(DIK_W, ForwardBind);
	Input->AddTask(DIK_S, BackwardBind);
	Input->AddTask(DIK_A, LeftBind);
	Input->AddTask(DIK_D, RightBind);
	Input->AddTask(DIK_Q, UpBind);
	Input->AddTask(DIK_E, DownBind);

//	Input->AddMouseTask(0, PitchBind);
//	Input->AddMouseTask(0, YawBind);

	AppTimer.Start();

	return true;
}

void Application::Update()
{
	AppTimer.Stop();

	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Green);
	Renderer->ClearDepthStencil(Renderer->GetDepthStencils()[0]);
	
	Input->Frame();
	
	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui_SceneViewer(SelectedScene);
	
	

	ImGui::EndFrame();
	

	ImGui::Render();
	
	SelectedScene->Update(AppTimer.GetDelta());
}

void Application::Render()
{


	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Renderer->SwapFrame();

}

void Application::Release()
{
	
}
