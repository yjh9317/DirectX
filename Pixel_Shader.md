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
