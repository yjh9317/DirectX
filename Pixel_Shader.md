변수
======================
      // InputLayout 생성(정점의 내부구조 저장)
      ComPtr<ID3D11InputLayout>		g_pInPutLayout; 
      
      
      // 픽셀 쉐이더
      ComPtr<ID3DBlob>				g_pPSBlob;
      ComPtr<ID3D11PixelShader>		g_pPS;



Pixel_Shader
=====================

        
  	//픽셀 쉐이더 컴파일
  
    //Vertex Shader와 동일
    
  	 hr = D3DCompileFromFile(wstring(strContentPath + L"shader\\test.fx").c_str(), nullptr
  		, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_Test", "ps_5_0", iFlag, 0, g_pPSBlob.GetAddressOf(), g_pErrBlob.GetAddressOf());
  
  
    //실패 했으면
  	if (FAILED(hr))
  	{
  		MessageBoxA(nullptr, (char*)g_pErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
  		assert(nullptr);
  	}
  
  	//컴파일 된 코드로 VertexShader 객체 만들기
  	//Blob이 관리하고있는 메모리 버퍼의 시작 주소 ,길이 , nullptr ,목적지로 저장할 픽셀 쉐이더
  	DEVICE->CreatePixelShader(g_pPSBlob->GetBufferPointer(), g_pPSBlob->GetBufferSize(), nullptr, g_pPS.GetAddressOf());
      
      

컴파일 함수
========================
픽셀마다 정보를 받아서 픽셀에 대한 정보를 저장하는? 함수
      
      float4 PS_Test(VTX_OUT _in) : SV_Target  // 픽셀마다 호출되는 함수
      {
	float4 vOutColor = (float4) 0.f;

	vOutColor = _in.vColor;			// 픽셀마다 모든 정점에 대한 거리값을 계산해서 가까운 정점일수록 그 정점이  색의 비율을 높여준다. (선형보간)

	return vOutColor;
      }

