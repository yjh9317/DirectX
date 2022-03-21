Input_Assembler
======================
 그려질 물체의 정보(정점의 내부 구조), 렌더링 관련 정보가 입력되는 단계

Code
=====================
  	// 정점 입력 구조 (정점 멤버 개수에 따라 나와야 되기 때문에 vector 사용)
  	vector<D3D11_INPUT_ELEMENT_DESC> arrDesc;
  	UINT iOffset = 0;
  
  	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};
  	
  	tInputDesc.SemanticName = "POSITION";						// Semantic 이름
  	tInputDesc.SemanticIndex = 0;					  			// 중복 이름인 경우, 인덱스로 구분
  	tInputDesc.InputSlot = 0;							    		// 버텍스 버퍼의 슬롯위치
  	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점별 데이터
  	tInputDesc.InstanceDataStepRate = 0;					  	        // 인스턴스별 데이터
  	tInputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;			    // DXGI_FORMAT의 형태로 R32G32B32(12byte)을 알려줌
  
  
  	tInputDesc.AlignedByteOffset = iOffset;       						//다음 멤버 변수 메모리의 시작위치를 알 수 있도록 오프셋 설정
  	iOffset += 12;			//12byte이므로 +12
  
  	arrDesc.push_back(tInputDesc);
  
  	
  	tInputDesc.SemanticName = "COLOR";
  	tInputDesc.SemanticIndex = 0;
  	tInputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// DXGI_FORMAT의 형태로 R32G32B32A32(16byte)을 알려줌
  	tInputDesc.AlignedByteOffset = iOffset;
  	iOffset += 16;
  
  
  	arrDesc.push_back(tInputDesc);
  
  	// 인풋 레이아웃 생성
    입력-버퍼 데이터를 설명하는 입력-레이아웃 개체
    
    //Desc 배열, 배열의 원소의 개수, 컴파일된 셰이더에 대한 포인터, 컴파일된 셰이더의 크기, 저장받을 주소(layout)
  	DEVICE->CreateInputLayout(arrDesc.data(), 2, g_pVSBlob->GetBufferPointer(), g_pVSBlob->GetBufferSize()
  		, g_pInPutLayout.GetAddressOf());
  

도움이 될만한 출처
==========================
https://blog.naver.com/sorkelf/40162437338
