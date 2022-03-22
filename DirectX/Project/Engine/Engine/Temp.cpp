#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

//정점 정보
Vertex	arrVtx[4] = {};
Vec4	g_vPos;

ComPtr<ID3D11Buffer>			g_pVB; // 정점 버퍼
ComPtr<ID3D11Buffer>			g_pIB; // 인덱스 버퍼

ComPtr<ID3D11Buffer>			g_pCB; // 상수 버퍼

// InputLayout 생성(정점의 내부구조 저장)
ComPtr<ID3D11InputLayout>		g_pInPutLayout; 


// 쉐이더 : GPU가 실행 할 함수


// 쉐이더 컴파일 실패시 ,실패 원인을 저장할 Blob(문자열 덩어리를 저장하는 객체)
ComPtr<ID3DBlob>				g_pErrBlob;


// 버텍스 쉐이더

// g_pVSBlob에서 버텍스 쉐이더에 필요한 정보를 다 저장하고 그걸 바탕으로 g_pVS에 전달해서 버텍스 쉐이더를 생성
// D12에서는 없어지고 g_pVSBlob로만 사용

ComPtr<ID3DBlob>				g_pVSBlob;
ComPtr<ID3D11VertexShader>		g_pVS;


// 픽셀 쉐이더
ComPtr<ID3DBlob>				g_pPSBlob;
ComPtr<ID3D11PixelShader>		g_pPS;











// Vertex Shader
// Hull Shader			--> Tesselation
// Domain Shader		--> Tesselation
// Geometry Shader		
// Pixel Shader

// Compute Shader		-->	GPGPU


// Rendering Pipleline 
// 물체가 그려지는 과정 (	3차원 정보가 2차원으로 그려지는 과정)

// IA(Input Assembler) --> 그려질 물체의 정보(정점의 내부 구조), 렌더링 관련 정보가 입력되는 단계

// VertexShader		--> 정점 하나당 실행되는 함수, 정점을 NDC(투영 좌표계)로 이동 시키는 역할 

// HullShader ,DomainShader (Tesselation) --> 정점을 파이프라인 도중에 대량 생성시킴. (Dx11)

// GeometryShader --> 정점을 파이프라인 도중에 조금 생성시킴. (Dx10)

// Rasterizer --> Culling (폴리곤을 이루는 정점의 접근 순서에 따른 Culling)
//						   시계방향은 앞면, 반시계는 뒷면 ,디폴트 옵션에선 뒷면이면 화면에 출력되지 않는다.

//				  폴리곤 내부 픽셀을 검출 (픽셀 쉐이더가 호출될 후보를 수색)


// PixelShader	--> Pixel 마다 호출 될 함수

// OM 단계 (OutPut Merge)
// Depth Stencil State Test
// 픽셀의 깊이값을 깊이텍스쳐와 비교해서 가려지는지 판정

// Blend State Test
// 혼합 공식에 따른 블렌딩 --> 렌더타겟에 출력






void TestInit()
{
	

	//NDC를 사용해서 좌표 표현
	//인덱스 버퍼를 사용해서 네모를 표현

	arrVtx[0].vPos	= Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);

	arrVtx[1].vPos	= Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor= Vec4(0.2f, 1.f, 0.2f, 1.f);

	arrVtx[2].vPos	= Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor= Vec4(0.2f, 0.2f, 1.f, 1.f);

	arrVtx[3].vPos	= Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);





	// 정점 데이터를 저장할 버텍스 버퍼를 생성한다.
	D3D11_BUFFER_DESC tBufferDesc = {};

	//생성시킬 버퍼의 사이즈
	tBufferDesc.ByteWidth = sizeof(Vertex) * 6;


	// 정점 버퍼는 처음 생성 이후에 버퍼를 수정하지 않는다.
	tBufferDesc.CPUAccessFlags = 0;	
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	//	정점을 저장하는 목적의 버퍼임을 알림,미리 어떤 데이터를 저장할 지 설정해야 최적화가 가능.
	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	
	tBufferDesc.MiscFlags = 0;
	tBufferDesc.StructureByteStride = 0;
	
	

	//초기 데이터를 넘겨주기 위한 정보 구조체
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = arrVtx;// 시스템 메모리의 시작주소 
	
	//시작주소로부터 크기는 위에 ByteWidth에서 미리 잡아놓았기 때문에 알려주지 않아도 알아서 잡아준다.

	//점 3개를 버텍스 버퍼(gpu메모리쪽)에 넣고 g_pVB에서 관리
	DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pVB.GetAddressOf());



	//인덱스 버퍼

	UINT arrIDx[6] = { 0,2,3,0,1,2 };
	tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(UINT) * 6;


	tBufferDesc.CPUAccessFlags = 0;
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	tBufferDesc.MiscFlags = 0;
	tBufferDesc.StructureByteStride = 0;

	
	tSubDesc = {};
	tSubDesc.pSysMem = arrIDx;

	DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pIB.GetAddressOf());


	// 상수버퍼 반드시 16바이트 단위로 잡혀야된다

	tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(Vec4);

	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	tBufferDesc.MiscFlags = 0;
	tBufferDesc.StructureByteStride = 0;

	DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pCB.GetAddressOf());


	UINT iFlag = 0;

#ifdef  _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif


	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	

	
	
	//버텍스 쉐이더(HLSL) 컴파일

	// 파일경로 , 매크로, 매크로,컴파일할 함수명 , 타겟 문자(어느시점에 쓰일건지), 버텍스 쉐이더 버전,플래그, 플래그 ,컴파일값을 저장할 객체 (Blob)주소,

	HRESULT hr = D3DCompileFromFile(wstring(strContentPath+L"shader\\test.fx").c_str(), nullptr
		,D3D_COMPILE_STANDARD_FILE_INCLUDE,"VS_Test","vs_5_0", iFlag,0,g_pVSBlob.GetAddressOf(), g_pErrBlob.GetAddressOf());

		
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)g_pErrBlob->GetBufferPointer(),"Shader Compile Failed!!",MB_OK);
		assert(nullptr);
	}

	//컴파일 된 코드로 VertexShader 객체 만들기
	//Blob이 관리하고있는 메모리 버퍼의 시작 주소 ,길이 , nullptr ,목적지로 저장할 버텍스 쉐이더
	DEVICE->CreateVertexShader(g_pVSBlob->GetBufferPointer(), g_pVSBlob->GetBufferSize(), nullptr,g_pVS.GetAddressOf());


	//픽셀 쉐이더 컴파일


	 hr = D3DCompileFromFile(wstring(strContentPath + L"shader\\test.fx").c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_Test", "ps_5_0", iFlag, 0, g_pPSBlob.GetAddressOf(), g_pErrBlob.GetAddressOf());


 
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)g_pErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	//컴파일 된 코드로 VertexShader 객체 만들기
	//Blob이 관리하고있는 메모리 버퍼의 시작 주소 ,길이 , nullptr ,목적지로 저장할 픽셀 쉐이더
	DEVICE->CreatePixelShader(g_pPSBlob->GetBufferPointer(), g_pPSBlob->GetBufferSize(), nullptr, g_pPS.GetAddressOf());


	// 정점 입력 구조 (정점 멤버 갯수에 따라 나와야 되기 때문에 vector 사용)
	vector<D3D11_INPUT_ELEMENT_DESC> arrDesc;
	UINT iOffset = 0;

	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};
	
	tInputDesc.SemanticName = "POSITION";						// Semantic 이름
	tInputDesc.SemanticIndex = 0;								// 중복 이름인 경우, 인덱스로 구분
	tInputDesc.InputSlot = 0;									// 버텍스 버퍼의 슬롯위치
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점마다 메모리를 사용할 수 있도록 선언
	tInputDesc.InstanceDataStepRate = 0;						// 인스턴스마다 
	tInputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;			// DXGI_FORMAT의 형태로 R32G32B32(12byte)을 알려줌


	tInputDesc.AlignedByteOffset = iOffset;						//다음 멤버 변수의 메모리의 시작위치를 알 수 있도록 오프셋 설정
	iOffset += 12;			//12byte이므로 +12

	arrDesc.push_back(tInputDesc);

	
	tInputDesc.SemanticName = "COLOR";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// DXGI_FORMAT의 형태로 R32G32B32A32(16byte)을 알려줌
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 16;


	arrDesc.push_back(tInputDesc);
	// 인풋 레이아웃 생성
	//입력 - 버퍼 데이터를 설명하는 입력 - 레이아웃 개체

	//Desc 배열, 배열의 원소의 개수, 컴파일된 셰이더에 대한 포인터, 컴파일된 셰이더의 크기, 저장받을 주소(layout)
	DEVICE->CreateInputLayout(arrDesc.data(), 2, g_pVSBlob->GetBufferPointer(), g_pVSBlob->GetBufferSize()
		, g_pInPutLayout.GetAddressOf());
}

void TestUpdate()
{




	// System 메모리에 있는 데이터를 gpu메모리에 옮길 때 임시 메모리를 만들고 map함수로 맵핑을 시킨다음에
	// Unmap을 이용해서 임시메모리를 gpu 메모리에 옮긴다.
	
	// 이런방식으로 위치를 옮길 수 있지만 키를 입력하지 않아도 덮어 씌우기도 하고
	// 버텍스 버퍼자체를 수정하기 때문에 정점의 개수가 많아진다면
	// 매프레임마다 모든 정점을 참조해야기도 하고 같은 정점을 참조하는 다른 물체의 형태도 바뀐다
	
	// 최초로 만들어진 버퍼는 모양(형태)를 유지하는 식이고 
	// 그렇기에 버텍스 쉐이더에서 모두 호출할 때 모든 정점에 대해 같은 이동량을 준다면
	// 원본 모양을 유지하면서 위치만 이동시킬 수 있음.

	// 버텍스 버퍼 수정
	// 버퍼가 수정가능한 값 cpuAccessFlag가 write이여야만 함.

	if (KEY_PRESSED(KEY::LEFT))
	{
		//for (int i = 0; i < 4; ++i)
		//{
		//	arrVtx[i].vPos.x -= DT * 0.5f;
		//}
		g_vPos.x -= DT * 0.5f;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		//for (int i = 0; i < 4; ++i)
		//{
		//	arrVtx[i].vPos.x += DT * 0.5f;
		//}
		g_vPos.x += DT * 0.5f;
	}

	// 상수버퍼에는 추가적인 이동량이 들어가 있음
	// 그 이동량만큼 VB에 더해줘서 래스터라이저에 옮기면됨

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(g_pCB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub);
	memcpy(tSub.pData, &g_vPos, sizeof(Vec4));
	CONTEXT->Unmap(g_pCB.Get(), 0);



	//D3D11_MAPPED_SUBRESOURCE tSub = {};

	//CONTEXT->Map(g_pVB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub); //임시 메모리와 버텍스 버퍼를 맵핑
	//memcpy(tSub.pData, arrVtx, sizeof(VTX) * 4);	//arrVtx값을 임시 메모리에 전이
	//CONTEXT->Unmap(g_pVB.Get(), 0);

}

void TestRender()
{
	CDevice::GetInst()->ClearTarget();

	// render (밑에서 순서는 상관없음, 밑 함수는 설정(예약)을 한거고 시작한게 아니다)
	// 인풋 어셈블러가 시작될때에 대한 세팅값들이라 Draw가 호출되는 순간 밑에서 설정해둔 값들이 적용되면서 시작.
	
	
	// IA 전달
	CONTEXT->IASetInputLayout(g_pInPutLayout.Get());

	
	UINT iStride = sizeof(VTX);
	UINT iOffset = 0;
	// 4번째 -> 정점끼리의 간격, 5번째-> 시작위치 오프셋
	CONTEXT->IASetVertexBuffers(0,1, g_pVB.GetAddressOf(),&iStride,&iOffset); //버텍스 버퍼를 여러개로 받을 수도 있음

	//인덱스 버퍼 주소, 단위가 4byte UINT , 오프셋
	CONTEXT->IASetIndexBuffer(g_pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	// 정점의 구조가 삼각형이라고 알려줌 (TRIANGLELIST)
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->VSSetConstantBuffers(0, 1, g_pCB.GetAddressOf());//버텍스 셰이더 시점에 전달할 버퍼(상수버퍼)
	
	CONTEXT->VSSetShader(g_pVS.Get(),0,0);		// 호출될 버텍스 쉐이더
	CONTEXT->PSSetShader(g_pPS.Get(), 0, 0);	 //호출될 픽셀 쉐이더

	//파이프 라인 시작
	//CONTEXT->Draw(6, 0);  Draw는 버텍스 버퍼만 사용
	CONTEXT->DrawIndexed(6, 0, 0);


	CDevice::GetInst()->Present();
}
