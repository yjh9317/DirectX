Rasterizer
===========
래스터라이저(Rasterizer)는 벡터 정보 (도형 또는 기본으로 구성)를 래스터 이미지 (픽셀)로 변환하는 단계다.  
래스터라이저에서 수행되는 작업들은 다음과 같다.  

 * 클리핑(clipping)
 * 원근 나눗셈(perspective division)
 * 뒷면 제거(back-face culling)
 * 뷰포트 변환(view-port transform)
 * 스캔 변환(scan conversion)



Culling
===============
Mesh를 만들때 정점들은 Topology(삼각형)으로 이루어져 Mesh를 만드는데 이 삼각형의 정점 인덱스가 시점에 따라 시계방향,반시계방향으로 그려진다.   
![ㅁㅇㄹ](https://user-images.githubusercontent.com/66587512/160391551-6b13b764-c026-4c90-825e-1eb6afda9b15.png)

desc.CullMode = D3D11_CULL_BACK;  --> 인덱싱이 시계방향이어야만 화면에 그려지게 되고  
desc.CullMode = D3D11_CULL_FRONT; --> 인덱싱이 반시계방향이어야만 화면에 그려지게 된다.  
desc.CullMode = D3D11_CULL_NONE; --> 방향과 상관없이 어느 시점이여도 화면에 출력이 된다.


desc.FillMode는 도형의 안쪽을 채울지 말지를 정해준다.  
  
desc.FillMode = D3D11_FILL_SOLID;  --> 안을 채워주게 되고  
desc.FillMode = D3D11_FILL_WIREFRAME; --> 안을 비우게 된다.  

Code
=======================
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
      
      	hr =DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
      	
      	if (FAILED(hr))
      		return E_FAIL;
      	
      
      	// 양면 모두 그리기, 주로 단면 형태의 메쉬를 앞 뒤에서 볼 때
      	desc.CullMode = D3D11_CULL_NONE;
      	desc.FillMode = D3D11_FILL_SOLID;
      	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
      
      	if (FAILED(hr))
      		return E_FAIL;
      
      	// 양면 모두 그리기(CULL_NONE), 뼈대 픽셀만 렌더링(안쪽을 채우지 않아 뒤쪽이 보인다.)
      	desc.CullMode = D3D11_CULL_NONE;
      	desc.FillMode = D3D11_FILL_WIREFRAME;
      	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
      
      	if (FAILED(hr))
      		return E_FAIL;
      
      	return S_OK;
      }
