#include "pch.h"
#include "Temp.h"

#include "CDevice.h"

// 정점(Vertex) -> 폴리곤을 구성하기위한 단위 ,폴리곤의 단위는 일반적으로 삼각형
// ID3D11Buffer는 Ram에서 생성되고 Gpu의 메모리를 관리하는 관리자급 객체란 느낌
ComPtr<ID3D11Buffer> g_pVB;

// 쉐이더 : GPU가 실행 할 함수

// 버텍스 쉐이더
//	HLSL (High Level Shader Language)














// Vertex Shader
// Hull Shader			--> Tesselation
// Domain Shader		--> Tesselation
// Geometry Shader		
// Pixel Shader

// Compute Shader		-->	GPGPU


// Rendering Pipleline 
// 물체가 그려지는 과정 (	3차원 정보가 2차원으로 그려지는 과정)

// IA(Input Assembler) --> 그려질 물체의 정보, 렌더링 관련 정보가 입력되는 단계

// VertexShader		--> 정점 하나당 실행되는 함수, 정점을 NDC(투영 좌표계)로 이동 시키는 역할

// HullShader ,DomainShader (Tesselation) --> 정점을 파이프라인 도중에 대량 생성시킴. (Dx11)

// GeometryShader --> 정점을 파이프라인 도중에 조금 생성시킴. (Dx10)

// Rasterizer --> Culling (폴리곤을 이루는 정점의 접근 순서에 따른 Culling)
//				  폴리곤 내부 픽셀을 검출 (픽셀 쉐이더가 호출될 후보를 수색)


// PixelShader	--> Pixel 마다 호출 될 함수

// OM 단계 (OutPut Merge)
// Depth Stencil State Test
// 픽셀의 깊이값을 깊이텍스쳐와 비교해서 가려지는지 판정

// Blend State Test
// 혼합 공식에 따른 블렌딩 --> 렌더타겟에 출력




void TestInit()
{
	Vertex arrVtx[3] = {};

	//NDC를 사용해서 좌표 표현

	arrVtx[0].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[1].vColor= Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vColor= Vec4(1.f, 1.f, 1.f, 1.f);


	// 정점 데이터를 저장할 버텍스 버퍼를 생성한다.
	D3D11_BUFFER_DESC tBufferDesc = {};

	//생성시킬 버퍼의 사이즈
	tBufferDesc.ByteWidth = sizeof(Vertex) * 3;


	//버퍼 생성 이후에도 , 버퍼의 내용을 수정 할 수 있는 옵션
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	//	정점을 저장하는 목적의 버퍼임을 알림,미리 어떤 데이터를 저장할 지 설정해야 최적화가 가능.
	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	
	tBufferDesc.MiscFlags = 0;
	tBufferDesc.StructureByteStride = 0;
	
	

	//초기 데이터를 넘겨주기 위한 정보 구조체
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = arrVtx;// 시스템 메모리의 시작주소 
	
	//시작주소로부터 크기는 위에 ByteWidth에서 미리 잡아놓았기 때문에 알려주지 않아도 알아서 잡아준다.


	DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pVB.GetAddressOf());
	
	
}

void TestUpdate()
{

}

void TestRender()
{
	CDevice::GetInst()->ClearTarget();

	//render
	
	
	CDevice::GetInst()->Present();
}
