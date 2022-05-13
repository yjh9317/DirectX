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
	// ����Ʈ �������� GetAddressOf�� �Լ��� ����Ͽ� �������̽��� �ּҸ� ������ �� �ִ�.

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"��ġ �ʱ�ȭ ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	UINT iQuality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQuality);
	//��Ƽ���ø� �߿� ����� �� �ִ� ǰ�� ������ ���� �����ɴϴ�.

	if (0 == iQuality)
	{
		MessageBox(nullptr, L"��Ƽ���� ����üũ ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}


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
	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	
	m_pDeviceContext->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(),pDSTex->GetDSV().Get());



	// ViewPort
	// �����쿡 ��� �� ����Ʈ������ ��ġ�� ����

	m_tViewPort.TopLeftX = 0;	//����Ʈ ������ �»� X��ǥ
	m_tViewPort.TopLeftY = 0;	//����Ʈ ������ �»� Y��ǥ

	m_tViewPort.Width = m_vRenderResolution.x;	// ����Ʈ ������ ����
	m_tViewPort.Height = m_vRenderResolution.y; // ����Ʈ ������ ����


	// ���� ��ǥ
	m_tViewPort.MinDepth = 0;	//����Ʈ ������ ���̰��� �ּҰ�
	m_tViewPort.MaxDepth = 1;	//����Ʈ ������ ���̰��� �ִ밪


	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);


	//�����Ͷ����� ������Ʈ ����
	if (FAILED(CreateRasterizerState()))
	{
		return E_FAIL;
	}

	// �������ٽ� ������Ʈ ����
	if (FAILED(CreateDepthStencilState()))
	{
		return E_FAIL;
	}
	// ���� ������Ʈ ����
	if (FAILED(CreateBlendState()))
	{
		return E_FAIL;
	}

	// ������� ����
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


	desc.BufferCount = 1; // 1�� �־��ָ� �˾Ƽ� ���������� ���۸� 2�� ������ش�.(����Ʈ,��)
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// ������� �ȼ��� Format (�ȼ��� �����Ʈ���� ,���������)�ȼ��� 4����Ʈ
	desc.BufferDesc.RefreshRate.Denominator = 1;	// ����ü�� ����ü�ȿ� ����ü�� ������			�и�
	desc.BufferDesc.RefreshRate.Numerator = 60;		// �̷����ؼ� �������� 60Hz�� �����.			����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //�ȼ� ���̴��� �ȼ� ����(����Ʈ)

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ�� �뵵
	desc.Flags = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ����ۿ� ����Ʈ ���۰� ���� �ٲ�� ȭ�鿡 �����ߴ� �׸��� ������ �ִ� ����Ʈ������ �׸���
	// ��� ó������ ���Ѵ�. DISCARD�� �׸��� ����.

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.OutputWindow = m_hWnd; //����Ϸ��� ������ ������
	desc.Windowed = true;// â���ų� ��üȭ��





	//�Ʒ��� ���������� ����ü���� ����� ������ �׳� �׷������� �ϰ� ���,����� ������ �߾˾Ƶθ�ȴ�

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pDXGIAdaptor = nullptr;
	ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

	//����� ���� Ÿ���� ��������� �ڵ����� ���̵��� �����Ǵ°� �� ���̵� ã�ƿ��� QueryInterFace
	//void�������� ������ �Լ����� Ÿ���� �ٸ� �� �־

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdaptor.GetAddressOf());
	pDXGIAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());

	// ����ü���� ����

	pDXGIFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf());
	//���� ����̽� .Get()�� �ֻ��� �θ�Ÿ��,����ü�� ����ü �ּҰ�,SwapChain�� �ּ�

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
	// ����ü���� ����ִ� ���۸� DX11������ ���ҽ��� �з��Ѵ�.(�̹���������,���ҽ��� gpu�޸𸮿� ����)
	// gpu�޸𸮿� gpu������ġ�� �ִµ� gpu������ġ�� gpu�޸𸮿� ���� �� �ӽø޸��� �������� �޸𸮿�
	// ���ҽ��� ����ϴµ� �װŸ� �����ϱ� ���ϴ°� View, ���ҽ����� View�� �޶� View�� ���� � ���ҽ������� �� �� ����

	// gpu �޸𸮿� �ø��� �ֵ��� ID3D11Resource�� �θ�� �ϰ� �׹ؿ� Texture2D Ŭ������ Buffer Ŭ������ �ִ�.
	// Texture2D���� �ؽ��ĺ��� View�� 4������ ����



	// Render Target View
	// View�� ���ҽ��� ���� �־�� view�� ���� ������
	// SwapChain�� ������ �ִ� ����(���� Ÿ�� ����)�� �����ϴ� ����


	// RenderTargetTexture 
	ComPtr<ID3D11Texture2D> pBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf());//����ü���� ����ִ� ���۸� ������
	CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", pBuffer,true); //ResMgr���� RTV����



	// Depth Stencil Texture �����
	Ptr<CTexture> pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y,
		DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL ,true);
	


	// Depth Stencil View 
	// ������ �񱳰� ��ü������ �ƴ� �ȼ������� ���ؼ� ��ü�� �յڸ� ���ؾ���
	// ���ۿ� �ִ� �ؽ�ó�� ��ġ�� Depth Stencile Texture�� ���̸� �����ؼ� ��ģ �ؽ�ó�� ���̸�  �ȼ������� ����


	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {}; //�����Ͷ����� ���� ����ü
	HRESULT hr = S_OK;
	// Default State
	// �ݽð�(�޸�) ����, �ð����(�ո�) ���
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;		// Default �̹Ƿ� desc�� ���� �ʾƵ� ��.

	// �ݽð�(�޸�) ���, �ð����(�ո�) ����
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID; //���� ä���� ��ä���� �����ִ� �Ӽ�

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;


	// ��� ��� �׸���, �ַ� �ܸ� ������ �޽��� �� �ڿ��� �� ��
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;

	// ��� ��� �׸���(CULL_NONE), ���� �ȼ��� ������(������ ä���� �ʴ´�)
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
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;		//Default �ɼ�

	D3D11_DEPTH_STENCIL_DESC desc = {};

	// LessEqual
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//���� �����

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
	desc.DepthEnable = false;	//�񱳸� ����
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS; // �׻� ���
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	if (FAILED(DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
		return E_FAIL;


	// NO_WRITE
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //���� ��� ����

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

	desc.AlphaToCoverageEnable = true;		// Ŀ�������� �ɼ� ��� ����
	//�ɼ��� Ű�� ���Ĺ�ü�� ���� ���ļ� ���� ������ �������� ���������� ������� �ذ����ش�.

	desc.IndependentBlendEnable = false;	// ����Ÿ�� ���彺���̵� ��������

	desc.RenderTarget[0].BlendEnable = true;			// ���� ������Ʈ ���
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	// ���� ȥ��
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // SrcRGB ���� ��� ==> (SrcA)
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // DestRGB ���� ��� ==> (1 - SrcA)	

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

	// ADDRESS ��� : uv�� �ʰ��� �� ó���ϴ� ����� ����
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;		//�ȼ��� �ֺ����� ���ġ�� ���� ��

	DEVICE->CreateSamplerState(&tDesc, m_arrSam[0].GetAddressOf());

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;	// ���εǴ� �ȼ� ���� �״�� �����´�.

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
	// RGB�� 0~255���� 0.f ~ 1.f�� �����ؼ� �����ؼ� �־���

	m_pDeviceContext->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
