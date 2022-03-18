#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	:m_hWnd(nullptr)
	,m_tSwapChainDesc{}
	,m_tViewPort{}
{

}

CDevice::~CDevice()
{	
}

int CDevice::init(HWND _hWnd, Vec2 _vRenderResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vRenderResolution;

	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG; //������϶� ������ ��� �޼����� ���ش�.�׷��� Debug����� �� ������.
#endif
	D3D_FEATURE_LEVEL iFeautureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;//�׷���ī�尡 �����ϱ⸸ �Ѵٸ� DirectX11.0�������� ����



	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, 0
		, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_pDevice.GetAddressOf(), &iFeautureLevel, m_pDeviceContext.GetAddressOf());


	// D3D_DRIVER_TYPE_HARDWARE -> �׷��� ī�带 ���
	// D3D_DRIVER_TYPE_SOFTWARE -> CPU�� ���
	// D3D11_SDK_VERSION -> ���̺귯�� ������ ��Ÿ��.
	
	// m_pDevice�� m_pDeviceContext�� ID3D11Device ������ ������ ����Ʈ������(Ŭ����)�� ����� ���� ���·�
	// ����Ʈ �������� GetAddressOf�� �Լ��� ����Ͽ� ��������� �ּҸ� ������ �� �ִ�.

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"��ġ �ʱ�ȭ ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	UINT iQuality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQuality);

	if (0 == iQuality)
	{
		MessageBox(nullptr, L"��Ƽ���� ����üũ ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// API������ ������ ���� �����ҷ��� �������۸��� ���
	// DirectX������ �ΰ��� ����ϴµ� ���⼭ SwapChain�� ������ �װ��̴�.
	
	//SwapChain ���� SwapChain�̶� ���� ������ �������� (����Ʈ���ۿ� �����) ����Ʈ���۸� ���� �ִٰ� ����ۿ� �׸��� �ϼ��̵Ǹ�
	//SwapChain �Լ��߿� Present �Լ��� ����ϴ� ���� ����Ʈ���ۿ� ����۰� ���� �ٲ�� ����Ʈ���۷� �ٲ� ����۸� ���Եȴ�.

	if (FAILED(CreateSwapchain()))
	{
		MessageBox(nullptr, L"����ü�� ���� ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// View 
	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"�� ���� ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


	// RenderTargetView, DepthStencilVeiw ����
	// Render ��� ���� �� ��� ���� ���� ����
	m_pDeviceContext->OMSetRenderTargets(1,m_RTV.GetAddressOf(),m_DSV.Get());



	// ViewPort
	// �����쿡 ��� �� ����Ʈ������ ��ġ�� ����, �⺻������ ������ �ػ󵵿� 1:1�� ������

	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;

	m_tViewPort.Width = m_vRenderResolution.x;
	m_tViewPort.Height = m_vRenderResolution.y;


	// ���� ��ǥ
	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;

	
	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);

	return S_OK;
}




int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {}; //SwapChain�� ����ϱ� ���� ����ü

	// SwapChain�ɼ��� �־�����ϴµ� � ������Ʈ�� ������� ����� ������ �ݺ��ȴ�.
	// �׳༮�� ���鶧 �ɼǰ��� ���� ���ٺ��ϱ� Desc ���� �־������.

	desc.BufferCount = 1; // 1�� �־��ָ� �˾Ƽ� ���������� ���۸� 2�� ������ش�.(����Ʈ,��)
	desc.BufferDesc.Width= (UINT)m_vRenderResolution.x; //������� ������ ����
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;	//����
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// ������� �ȼ��� Format (�ȼ��� �����Ʈ���� ,���������)�ȼ��� 4����Ʈ
	desc.BufferDesc.RefreshRate.Denominator = 1;	// ����ü�� ����ü�ȿ� ����ü�� ������			�и�
	desc.BufferDesc.RefreshRate.Numerator = 60;		// �̷����ؼ� �������� 60Hz�� �����.			����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; //����Ʈ �ɼ�
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //�ȼ� ���̴��� �ȼ� ����(����Ʈ)

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ�� �뵵
	desc.Flags = 0; //�ɼǾ��� ����Ʈ
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ����ü���ε� ����ۿ� �׸��� �׸��ٰ� Present�Լ� ȣ����Ѽ� �׷����� ����� Front�� �ٲٴµ�
	//�׶����ʹ� ���� Front�� Back, Back->Front�� �Ǽ� ȭ�鿡 ����,���� ����۰� �� �ִ� �����ϴ� ����� ������ �ִµ� �� ����� �����Ұ��ε�
	//DXGI_SWAP_EFFECT_DISCARD�� �������� �ʰ� �� ���� ������. ���߿� ��Ƽ������ �ذ��Ҽ�����
	desc.SampleDesc.Count = 1; //����Ʈ
	desc.SampleDesc.Quality = 0; //����Ʈ
		
	desc.OutputWindow= m_hWnd; //����Ϸ��� ������ ������
	desc.Windowed = true;// â���ų� ��üȭ��





	//�Ʒ��� ���������� ����ü���� ����� ������ �׳� �׷������� �ϰ� ���,����� ������ �߾˾Ƶθ�ȴ�

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pDXGIAdaptor = nullptr;
	ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

	//����� ���� Ÿ���� ��������� �ڵ����� ���̵��� �����Ǵ°� �� ���̵� ã�ƿ��� QueryInterFace
	//void�������� ������ �Լ����� Ÿ���� �ٸ� �� �־

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdaptor.GetAddressOf());
	pDXGIAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());

	// ����ü���� ����
	
	pDXGIFactory->CreateSwapChain(m_pDevice.Get(),&desc,m_pSwapChain.GetAddressOf());
	//���� ����̽� .Get()�� �ֻ��� �θ�Ÿ��,����ü�� ����ü �ּҰ�,SwapChain�� �ּ�
	
	if (nullptr == m_pSwapChain)
	{
		return E_FAIL;
	}
	


	return S_OK;
}

int CDevice::CreateView()
{
	// ����ü���� ����ִ� ���۸� DX11������ ���ҽ��� �з��Ѵ�.(�̹���������,���ҽ��� gpu�޸𸮿� ����)
	// gpu�޸𸮿� gpu������ġ�� �ִµ� gpu������ġ�� gpu�޸𸮿� ���� �� �ӽø޸��� �������� �޸𸮿�
	// ���ҽ��� ����ϴµ� �װŸ� �����ϱ� ���ϴ°� View, ���ҽ����� View�� �޶� View�� ���� � ���ҽ������� �� �� ����

	// gpu �޸𸮿� �ø��� �ֵ��� ID3D11Resource�� �θ�� �ϰ� �׹ؿ� Texture2D Ŭ������ Buffer Ŭ������ �ִ�.
	// Texture2D���� �ؽ��ĺ��� View�� 4������ ����
	


	// Render Target View
	// View�� ���ҽ��� ���� �־�� view�� ���� ������
	// SwapChain�� ������ �ִ� ����(���� Ÿ�� ����)�� �����ϴ� ����

	ComPtr<ID3D11Texture2D> pBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf());//����ü���� ����ִ� ���۸� ������

	
	m_pDevice->CreateRenderTargetView(pBuffer.Get(), nullptr, m_RTV.GetAddressOf());

	if (nullptr == m_RTV)
		return E_FAIL;


	// Depth Stencil Texture �����
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = (UINT)m_vRenderResolution.x;
	desc.Height = (UINT)m_vRenderResolution.y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	
	desc.CPUAccessFlags = 0; //cpu ����
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // ������ �̸� ������,�̰ɷ� View�� ����,���п��� �Ǵ�
	desc.Usage=D3D11_USAGE::D3D11_USAGE_DEFAULT;

	desc.MiscFlags = 0; //�߰� �ɼ�
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// D24 -> Depth 24(3byte), S8->Stencil 8 (1byte) ���̸� Depth�� �����ϰ� ���߿� Stencil�� 1byte�� ���

	m_pDevice->CreateTexture2D(&desc, nullptr, m_pDepthStencilTarget.GetAddressOf());

	if (nullptr == m_pDepthStencilTarget)
		return E_FAIL;
	

	// Depth Stencil View 
	// ������ �񱳰� ��ü������ �ƴ� �ȼ������� ���ؼ� ��ü�� �յڸ� ���ؾ���
	// ���ۿ� �ִ� �ؽ�ó�� ��ġ�� Depth Stencile Texture�� ���̸� �����ؼ� ��ģ �ؽ�ó�� ���̸�  �ȼ������� ����
	m_pDevice->CreateDepthStencilView(m_pDepthStencilTarget.Get(), nullptr, m_DSV.GetAddressOf());

	if (nullptr == m_DSV)
		return E_FAIL;
	

	return S_OK;
}


void CDevice::ClearTarget()
{
	m_pDeviceContext->ClearRenderTargetView(m_RTV.Get(),Vec4(0.f,0.f,0.f,1.f));
	// RGB�� 0~255���� 0.f ~ 1.f�� �����ؼ� �����ؼ� �־��� ���� Vec4�� 0,0,0,255.

	m_pDeviceContext->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}