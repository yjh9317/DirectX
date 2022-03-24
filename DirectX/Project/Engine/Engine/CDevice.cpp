#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	:m_hWnd(nullptr)
	,m_tSwapChainDesc{}
	,m_tViewPort{}
	,m_arrCB{}
{

}

CDevice::~CDevice()
{	
	Safe_Del_Arr(m_arrCB);
}

int CDevice::init(HWND _hWnd, Vec2 _vRenderResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vRenderResolution;

	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG; //디버그일때 오류를 잡는 메세지를 쏴준다.그래서 Debug모드일 때 느리다.
#endif
	D3D_FEATURE_LEVEL iFeautureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;//그래픽카드가 지원하기만 한다면 DirectX11.0버전으로 지원



	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, 0
		, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_pDevice.GetAddressOf(), &iFeautureLevel, m_pDeviceContext.GetAddressOf());


	// D3D_DRIVER_TYPE_HARDWARE -> 그래픽 카드를 사용
	// D3D_DRIVER_TYPE_SOFTWARE -> CPU를 사용
	// D3D11_SDK_VERSION -> 라이브러리 버전을 나타냄.
	
	// m_pDevice와 m_pDeviceContext는 ID3D11Device 포인터 변수를 스마트포인터(클래스)의 멤버로 만든 상태로
	// 스마트 포인터의 GetAddressOf란 함수를 사용하여 인터페이스의 주소를 가져올 수 있다.

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"장치 초기화 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}

	UINT iQuality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQuality);
	//멀티샘플링 중에 사용할 수 있는 품질 수준의 수를 가져옵니다.

	if (0 == iQuality)
	{
		MessageBox(nullptr, L"멀티셈플 레벨체크 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}
	

	if (FAILED(CreateSwapchain()))
	{
		MessageBox(nullptr, L"스왑체인 생성 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// View 
	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"뷰 생성 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// RenderTargetView, DepthStencilVeiw 전달
	// Render 출력 버퍼 및 출력 깊이 버퍼 지정
	m_pDeviceContext->OMSetRenderTargets(1,m_RTV.GetAddressOf(),m_DSV.Get());



	// ViewPort
	// 윈도우에 출력 될 프론트버퍼의 위치를 설정, 기본적으로 윈도우 해상도와 1:1로 맞춰줌

	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;

	m_tViewPort.Width = m_vRenderResolution.x;
	m_tViewPort.Height = m_vRenderResolution.y;


	// 투영 좌표
	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;

	
	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);

	if (FAILED(CreateConstBuffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}




int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {}; 


	desc.BufferCount = 1; // 1을 넣어주면 알아서 내부적으로 버퍼를 2장 만들어준다.(프론트,백)
	desc.BufferDesc.Width= (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 만들어진 픽셀의 Format (픽셀이 몇바이트인지 ,어떤구성인지)픽셀당 4바이트
	desc.BufferDesc.RefreshRate.Denominator = 1;	// 스왑체인 구조체안에 구조체가 또있음			분모
	desc.BufferDesc.RefreshRate.Numerator = 60;		// 이렇게해서 프레임을 60Hz를 잡아줌.			분자
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //픽셀 쉐이더의 픽셀 순서(디폴트)

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟 용도
	desc.Flags = 0; 
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑체인인데 백버퍼에 그림을 그리다가 Present함수 호출시켜서 그려놓은 장면을 Front로 바꾸는데
	//그때부터는 원래 Front가 Back, Back->Front가 되서 화면에 송출,이제 백버퍼가 된 애는 송출하던 장면을 가지고 있는데 그 장면을 보존할것인데
	//DXGI_SWAP_EFFECT_DISCARD는 보존하지 않고 그 위에 덮어씌운다.
	desc.SampleDesc.Count = 1; 
	desc.SampleDesc.Quality = 0;
		
	desc.OutputWindow= m_hWnd; //출력하려는 목적지 윈도우
	desc.Windowed = true;// 창모드거나 전체화면





	//아래는 관용적으로 스왑체인을 만드는 구조라 그냥 그런가보다 하고 사용,사용할 변수만 잘알아두면된다

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pDXGIAdaptor = nullptr;
	ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

	//사용자 정의 타입은 만들어질때 자동으로 아이디값이 생성되는게 그 아이디를 찾아오는 QueryInterFace
	//void포인터인 이유는 함수마다 타입이 다를 수 있어서

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdaptor.GetAddressOf());
	pDXGIAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());

	// 스왑체인을 생성
	
	pDXGIFactory->CreateSwapChain(m_pDevice.Get(),&desc,m_pSwapChain.GetAddressOf());
	//원본 디바이스 .Get()은 최상위 부모타입,스왑체인 구조체 주소값,SwapChain의 주소
	
	IDXGISwapChain* p=m_pSwapChain.Get();
	IDXGISwapChain** pp = m_pSwapChain.GetAddressOf();

	if (nullptr == m_pSwapChain)
	{
		return E_FAIL;
	}
	


	return S_OK;
}

int CDevice::CreateView()
{
	// 스왑체인이 들고있는 버퍼를 DX11에서는 리소스로 분류한다.(이미지데이터,리소스를 gpu메모리에 생성)
	// gpu메모리와 gpu연산장치가 있는데 gpu연산장치가 gpu메모리에 찍을 때 임시메모리인 레지스터 메모리에
	// 리소스를 등록하는데 그거를 전달하기 위하는게 View, 리소스마다 View가 달라서 View에 따라 어떤 리소스인지도 알 수 있음

	// gpu 메모리에 올리는 애들은 ID3D11Resource를 부모로 하고 그밑에 Texture2D 클래스와 Buffer 클래스가 있다.
	// Texture2D에도 텍스쳐별로 View가 4종류가 있음
	


	// Render Target View
	// View는 리소스가 먼저 있어야 view를 만들 수있음
	// SwapChain이 가지고 있는 버퍼(렌더 타겟 버퍼)를 전달하는 역할

	ComPtr<ID3D11Texture2D> pBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf());//스왑체인이 들고있는 버퍼를 가져옴

	
	m_pDevice->CreateRenderTargetView(pBuffer.Get(), nullptr, m_RTV.GetAddressOf());

	if (nullptr == m_RTV)
		return E_FAIL;


	// Depth Stencil Texture 만들기
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = (UINT)m_vRenderResolution.x;
	desc.Height = (UINT)m_vRenderResolution.y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	
	desc.CPUAccessFlags = 0; //cpu 접근
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // 역할을 미리 정해준다.(리소스마다 View가 다른데 이 값으로 구분)
	desc.Usage=D3D11_USAGE::D3D11_USAGE_DEFAULT;

	desc.MiscFlags = 0; //추가 옵션
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// D24 -> Depth 24(3byte), S8->Stencil 8 (1byte) 깊이를 Depth에 저장하고 나중에 Stencil를 1byte로 사용

	m_pDevice->CreateTexture2D(&desc, nullptr, m_pDepthStencilTarget.GetAddressOf());

	if (nullptr == m_pDepthStencilTarget)
		return E_FAIL;
	

	// Depth Stencil View 
	// 깊이의 비교가 물체단위가 아닌 픽셀단위로 비교해서 물체의 앞뒤를 비교해야함
	// 버퍼에 있는 텍스처의 위치를 Depth Stencile Texture에 깊이를 저장해서 겹친 텍스처의 깊이를  픽셀단위로 비교함
	m_pDevice->CreateDepthStencilView(m_pDepthStencilTarget.Get(), nullptr, m_DSV.GetAddressOf());

	if (nullptr == m_DSV)
		return E_FAIL;
	

	return S_OK;
}

int CDevice::CreateConstBuffer()
{

	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(Vec4));

	return 0;
}


void CDevice::ClearTarget()
{
	m_pDeviceContext->ClearRenderTargetView(m_RTV.Get(),Vec4(0.65f,0.65f,0.65f,1.f));
	// RGB의 0~255값을 0.f ~ 1.f로 보간해서 변경해서 넣어줌 위에 Vec4는 0,0,0,255.

	m_pDeviceContext->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
