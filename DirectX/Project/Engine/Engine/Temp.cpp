#include "pch.h"
#include "Temp.h"

#include "CDevice.h"

// ����(Vertex) -> �������� �����ϱ����� ���� ,�������� ������ �Ϲ������� �ﰢ��
// ID3D11Buffer�� Ram���� �����ǰ� Gpu�� �޸𸮸� �����ϴ� �����ڱ� ��ü�� ����
ComPtr<ID3D11Buffer> g_pVB;

// ���̴� : GPU�� ���� �� �Լ�

// ���ؽ� ���̴�
//	HLSL (High Level Shader Language)














// Vertex Shader
// Hull Shader			--> Tesselation
// Domain Shader		--> Tesselation
// Geometry Shader		
// Pixel Shader

// Compute Shader		-->	GPGPU


// Rendering Pipleline 
// ��ü�� �׷����� ���� (	3���� ������ 2�������� �׷����� ����)

// IA(Input Assembler) --> �׷��� ��ü�� ����, ������ ���� ������ �ԷµǴ� �ܰ�

// VertexShader		--> ���� �ϳ��� ����Ǵ� �Լ�, ������ NDC(���� ��ǥ��)�� �̵� ��Ű�� ����

// HullShader ,DomainShader (Tesselation) --> ������ ���������� ���߿� �뷮 ������Ŵ. (Dx11)

// GeometryShader --> ������ ���������� ���߿� ���� ������Ŵ. (Dx10)

// Rasterizer --> Culling (�������� �̷�� ������ ���� ������ ���� Culling)
//				  ������ ���� �ȼ��� ���� (�ȼ� ���̴��� ȣ��� �ĺ��� ����)


// PixelShader	--> Pixel ���� ȣ�� �� �Լ�

// OM �ܰ� (OutPut Merge)
// Depth Stencil State Test
// �ȼ��� ���̰��� �����ؽ��Ŀ� ���ؼ� ���������� ����

// Blend State Test
// ȥ�� ���Ŀ� ���� ���� --> ����Ÿ�ٿ� ���




void TestInit()
{
	Vertex arrVtx[3] = {};

	//NDC�� ����ؼ� ��ǥ ǥ��

	arrVtx[0].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[1].vColor= Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vColor= Vec4(1.f, 1.f, 1.f, 1.f);


	// ���� �����͸� ������ ���ؽ� ���۸� �����Ѵ�.
	D3D11_BUFFER_DESC tBufferDesc = {};

	//������ų ������ ������
	tBufferDesc.ByteWidth = sizeof(Vertex) * 3;


	//���� ���� ���Ŀ��� , ������ ������ ���� �� �� �ִ� �ɼ�
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	//	������ �����ϴ� ������ �������� �˸�,�̸� � �����͸� ������ �� �����ؾ� ����ȭ�� ����.
	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	
	tBufferDesc.MiscFlags = 0;
	tBufferDesc.StructureByteStride = 0;
	
	

	//�ʱ� �����͸� �Ѱ��ֱ� ���� ���� ����ü
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = arrVtx;// �ý��� �޸��� �����ּ� 
	
	//�����ּҷκ��� ũ��� ���� ByteWidth���� �̸� ��Ƴ��ұ� ������ �˷����� �ʾƵ� �˾Ƽ� ����ش�.


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
