Render Target View
=================
자원 뷰 (Resource View)중 하나로 Texture2D에서는 총 4가지 종류가 있고 그중 하나이다.  
SwapChain에서 생성한 두개의 버퍼를 가지고 Render Target View를 생성하고 실제로 렌더링할 버퍼와 바인딩 한 후  
View Port(화면설정)를 생성하면 바인딩된 버퍼를 렌더 타겟 뷰로 사용해서 화면에 띄울 수 있다.


Code
=================

  	ComPtr<ID3D11Texture2D> pBuffer = nullptr;
  
  	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf());//스왑체인이 들고있는 버퍼를 가져옴
  
  	//바인딩
  	m_pDevice->CreateRenderTargetView(pBuffer.Get(), nullptr, m_RTV.GetAddressOf());
  
  	if (nullptr == m_RTV)
  		return E_FAIL;
