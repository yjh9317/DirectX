#pragma once

class CDevice
{
	SINGLE(CDevice);
private:
	HWND							m_hWnd; // Main Window Handle
	Vec2							m_vRenderResolution; // ������ �ػ�

	//ID3D11Device*	m_pDevice;
	ComPtr<ID3D11Device>			m_pDevice;			// GPU �޸�
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;	// GPU Rendering ���� 

	ComPtr<IDXGISwapChain>			m_pSwapChain;
	ComPtr<ID3D11Texture2D>			m_pRenderTarget;
	ComPtr<ID3D11Texture2D>			m_pDepthStencilTarget;

	ComPtr<ID3D11RenderTargetView>	m_RTV;
	ComPtr<ID3D11DepthStencilView>	m_DSV;

	D3D11_VIEWPORT					m_tViewPort;
	DXGI_SWAP_CHAIN_DESC			m_tSwapChainDesc;

public:
	int init(HWND _hWnd, Vec2 _vRenderResolution);

	void Present() { m_pSwapChain->Present(0, 0); }		// ����ۿ� ����Ʈ ���� �ٲٴ� �Լ�
	void ClearTarget();									// Ÿ��(����)�� Ŭ����

	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_pDeviceContext; }

private:
	int CreateSwapchain();
	int CreateView();

};

