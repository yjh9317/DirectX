#include "pch.h"
#include "CDevice.h"


#include "CConstBuffer.h"

#include "CResMgr.h"

CDevice::CDevice()
	:m_hWnd(nullptr)
	, m_tSwapChainDesc{}
	, m_tViewPort{}
	, m_arrRS{}
	, m_arrCB{}
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
	g_global.vResolution = m_vRenderResolution;

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
	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	
	m_pDeviceContext->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(),pDSTex->GetDSV().Get());



	// ViewPort
	// 윈도우에 출력 될 프론트버퍼의 위치를 설정

	m_tViewPort.TopLeftX = 0;	//뷰포트 영역의 좌상 X좌표
	m_tViewPort.TopLeftY = 0;	//뷰포트 영역의 좌상 Y좌표

	m_tViewPort.Width = m_vRenderResolution.x;	// 뷰포트 영역의 넓이
	m_tViewPort.Height = m_vRenderResolution.y; // 뷰포트 영역의 높이


	// 투영 좌표
	m_tViewPort.MinDepth = 0;	//뷰포트 영역의 깊이값의 최소값
	m_tViewPort.MaxDepth = 1;	//뷰포트 영역의 깊이값의 최대값


	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);


	//래스터라이저 스테이트 생성
	if (FAILED(CreateRasterizerState()))
	{
		return E_FAIL;
	}

	// 뎁스스텐실 스테이트 생성
	if (FAILED(CreateDepthStencilState()))
	{
		return E_FAIL;
	}
	// 블렌드 스테이트 생성
	if (FAILED(CreateBlendState()))
	{
		return E_FAIL;
	}

	// 상수버퍼 생성
	if (FAILED(CreateConstBuffer()))
	{
		return E_FAIL;
	}

	CreateSamplerState();

	return S_OK;
}




int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};


	desc.BufferCount = 1; // 1을 넣어주면 알아서 내부적으로 버퍼를 2장 만들어준다.(프론트,백)
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 만들어진 픽셀의 Format (픽셀이 몇바이트인지 ,어떤구성인지)픽셀당 4바이트
	desc.BufferDesc.RefreshRate.Denominator = 1;	// 스왑체인 구조체안에 구조체가 또있음			분모
	desc.BufferDesc.RefreshRate.Numerator = 60;		// 이렇게해서 프레임을 60Hz를 잡아줌.			분자
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //픽셀 쉐이더의 픽셀 순서(디폴트)

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟 용도
	desc.Flags = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 백버퍼와 프론트 버퍼가 서로 바뀌고 화면에 송출했던 그림을 가지고 있는 프론트버퍼의 그림을
	// 어떻게 처분할지 정한다. DISCARD는 그림을 삭제.

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.OutputWindow = m_hWnd; //출력하려는 목적지 윈도우
	desc.Windowed = true;// 창모드거나 전체화면





	//아래는 관용적으로 스왑체인을 만드는 구조라 그냥 그런가보다 하고 사용,사용할 변수만 잘알아두면된다

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pDXGIAdaptor = nullptr;
	ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

	//사용자 정의 타입은 만들어질때 자동으로 아이디값이 생성되는게 그 아이디를 찾아오는 QueryInterFace
	//void포인터인 이유는 함수마다 타입이 다를 수 있어서

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdaptor.GetAddressOf());
	pDXGIAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());

	// 스왑체인을 생성

	pDXGIFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf());
	//원본 디바이스 .Get()은 최상위 부모타입,스왑체인 구조체 주소값,SwapChain의 주소

	//IDXGISwapChain* p = m_pSwapChain.Get();
	//IDXGISwapChain** pp = m_pSwapChain.GetAddressOf();

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


	// RenderTargetTexture 
	ComPtr<ID3D11Texture2D> pBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf());//스왑체인이 들고있는 버퍼를 가져옴
	CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", pBuffer,true); //ResMgr에서 RTV관리



	// Depth Stencil Texture 만들기
	Ptr<CTexture> pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y,
		DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL ,true);
	


	// Depth Stencil View 
	// 깊이의 비교가 물체단위가 아닌 픽셀단위로 비교해서 물체의 앞뒤를 비교해야함
	// 버퍼에 있는 텍스처의 위치를 Depth Stencile Texture에 깊이를 저장해서 겹친 텍스처의 깊이를  픽셀단위로 비교함


	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {}; //래스터라이저 정보 구조체
	HRESULT hr = S_OK;
	// Default State
	// 반시계(뒷면) 제외, 시계방향(앞면) 통과
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;		// Default 이므로 desc를 넣지 않아도 됨.

	// 반시계(뒷면) 통과, 시계방향(앞면) 제외
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID; //안을 채울지 안채울지 정해주는 속성

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;


	// 양면 모두 그리기, 주로 단면 형태의 메쉬를 앞 뒤에서 볼 때
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;

	// 양면 모두 그리기(CULL_NONE), 뼈대 픽셀만 렌더링(안쪽을 채우지 않는다)
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{

	// Less(Default)
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;		//Default 옵션

	D3D11_DEPTH_STENCIL_DESC desc = {};

	// LessEqual
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//깊이 기록함

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf())))
		return E_FAIL;

	//Greater
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::GREATER].GetAddressOf())))
		return E_FAIL;

	//Greater_Equal
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf())))
		return E_FAIL;


	// NO_TEST
	desc.DepthEnable = false;	//비교를 안함
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS; // 항상 통과
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
		return E_FAIL;


	// NO_WRITE
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //깊이 기록 안함

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf())))
		return E_FAIL;


	// NO_TEST_NO_WRITE
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
		return E_FAIL;





	return S_OK;
}


int CDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;


	D3D11_BLEND_DESC desc = {};

	desc.AlphaToCoverageEnable = true;		// 커버레이지 옵션 사용 유무
	//옵션을 키면 알파물체가 서로 겹쳐서 깊이 문제가 생겼을때 내부적으로 어느정도 해결해준다.

	desc.IndependentBlendEnable = false;	// 렌더타겟 블랜드스테이드 독립실행

	desc.RenderTarget[0].BlendEnable = true;			// 블랜딩 스테이트 사용
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	// 가산 혼합
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // SrcRGB 블랜드 계수 ==> (SrcA)
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // DestRGB 블랜드 계수 ==> (1 - SrcA)	

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform));

	m_arrCB[(UINT)CB_TYPE::SCALAR_PARAM] = new CConstBuffer(CB_TYPE::SCALAR_PARAM);
	m_arrCB[(UINT)CB_TYPE::SCALAR_PARAM]->Create(sizeof(tScalarParam));

	m_arrCB[(UINT)CB_TYPE::ANIM2D] = new CConstBuffer(CB_TYPE::ANIM2D);
	m_arrCB[(UINT)CB_TYPE::ANIM2D]->Create(sizeof(tAnim2D));
	
	m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
	m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(tGlobal));

	return S_OK;
}

void CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};

	// ADDRESS 모드 : uv를 초과할 때 처리하는 방식을 설정
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;		//픽셀의 주변색의 평균치를 내서 찍어냄

	DEVICE->CreateSamplerState(&tDesc, m_arrSam[0].GetAddressOf());

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;	// 맵핑되는 픽셀 색을 그대로 가져온다.

	DEVICE->CreateSamplerState(&tDesc, m_arrSam[1].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
}


void CDevice::ClearTarget()
{
	static CTexture* pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex").Get();
	static CTexture* pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex").Get();

	m_pDeviceContext->ClearRenderTargetView(pRTTex->GetRTV().Get(), Vec4(0.65f, 0.65f, 0.65f, 1.f));
	// RGB의 0~255값을 0.f ~ 1.f로 보간해서 변경해서 넣어줌

	m_pDeviceContext->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
