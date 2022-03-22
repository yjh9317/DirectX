Vertex Buffer를 이용한 이동
============================
아래는 키를 입력한다면 버텍스 버퍼의 정점 위치를 옮겨서 도형을 옮기는 코드이다.  
(인덱스 버퍼를 이용한 사각형이므로 정점의 개수는 4)  
  
    void Update(){
    
  	if (KEY_PRESSED(KEY::LEFT))
  	{
  		for (int i = 0; i < 4; ++i)
  		{
  			arrVtx[i].vPos.x -= DT * 0.5f;
  		}
  	}
  
  	if (KEY_PRESSED(KEY::RIGHT))
  	{
  		for (int i = 0; i < 4; ++i)
  		{
  			arrVtx[i].vPos.x += DT * 0.5f;
  		}
  	}

  	D3D11_MAPPED_SUBRESOURCE tSub = {}; //임시 메모리
  
  	CONTEXT->Map(g_pVB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub); //임시 메모리와 버텍스 버퍼를 맵핑
  	memcpy(tSub.pData, arrVtx, sizeof(VTX) * 4);        	//arrVtx값을 임시 메모리에 전이
  	CONTEXT->Unmap(g_pVB.Get(), 0);
    
    }

  
Constant Buffer
=================
위에서 처럼 버텍스 버퍼를 이용하여 물체를 이동시킬 수 있지만 그렇게 한다면 문제점이 있다.  
버텍스 버퍼자체를 수정하기 때문에 정점의 개수가 많아진다면 모든 정점의 위치값(vPos)를 개별적으로   
값을 바꾸기 때문에 엄청난 비용이 들게 된다.  
  
그렇기에 상수 버퍼를 따로 만들어서 그 버퍼에 상수(이동량)를 저장하고  
모든 정점에 대해 상수값을 적용시켜 물체를 이동시킬 수 있게 된다.  


Constant Buffer를 이용한 이동
=================================
// System 메모리에 있는 데이터를 gpu메모리에 옮길 때 임시 메모리를 만들고 map함수로 맵핑을 시킨다음에
// Unmap을 이용해서 임시메모리를 gpu 메모리에 옮긴다.

      	(HLSL)
      	//상수버퍼 레지스터 , b0:슬롯넘버
      	cbuffer POSITION : register(b0)
      	{
          float4 g_Pos; //전달할 값(이동량)	
      	}
	
	

      	Vec4	g_vPos; //물체의 이동량을 담을 Vec4(16byte)  
      
    	if (KEY_PRESSED(KEY::LEFT))
    	{
    		g_vPos.x -= DT * 0.5f;     //delta Time * 0.5f
    	}
    
    	if (KEY_PRESSED(KEY::RIGHT))
    	{
    		g_vPos.x += DT * 0.5f;      //delta Time * 0.5f
    	}
    
    
    	D3D11_MAPPED_SUBRESOURCE tSub = {};
    
    	CONTEXT->Map(g_pCB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub); //임시메모리와 맵핑
    	memcpy(tSub.pData, &g_vPos, sizeof(Vec4));                   //g_vPos값을 메모리에 이동
    	CONTEXT->Unmap(g_pCB.Get(), 0);                               //상수 버퍼로 이동



상수버퍼생성Code
===============
      
      

      
      ComPtr<ID3D11Buffer>			g_pCB; // 상수 버퍼
      
      

     	// 상수버퍼 반드시 16바이트 단위로 잡혀야된다

	    tBufferDesc = {};
	    tBufferDesc.ByteWidth = sizeof(Vec4); (16byte)
    
      //읽고 쓰기 가능(수정)
	    tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	    tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    
	    tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER; //상수 버퍼임을 알림
	    tBufferDesc.MiscFlags = 0;
	    tBufferDesc.StructureByteStride = 0;
      
      
	    DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pCB.GetAddressOf());
    
