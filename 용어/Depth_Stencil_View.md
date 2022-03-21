Depth Stencil
=======================
Depth Stencil은 물체에 대한 깊이를 픽셀단위로 텍스쳐에 저장한 후 물체끼리 깊이를 비교한다. 
설정이 Default고 겹치는 물체가 있으면 깊이가 작은 물체를 먼저 앞에 그려준다





Code
=========
	// Depth Stencil Texture 만들기(깊이를 저장하는 텍스쳐)
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
