#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"

// ����(Vertex) -> �������� �����ϱ����� ���� ,�������� ������ �Ϲ������� �ﰢ��
// ID3D11Buffer�� Ram���� �����ǰ� Gpu�� �޸𸮸� �����ϴ� �����ڱ� ��ü�� ����
ComPtr<ID3D11Buffer>			g_pVB;

// InputLayout ����(������ ���α��� ����)
ComPtr<ID3D11InputLayout>		g_pInPutLayout; 


// ���̴� : GPU�� ���� �� �Լ�


// ���̴� ������ ���н� ,���� ������ ������ Blob(���ڿ� ����� �����ϴ� ��ü)
ComPtr<ID3DBlob>				g_pErrBlob;


// ���ؽ� ���̴�
//	HLSL (High Level Shader Language)

// g_pVSBlob���� ���ؽ� ���̴��� �ʿ��� ������ �� �����ϰ� �װ� �������� g_pVS�� �����ؼ� ���ؽ� ���̴��� ����
// D12������ �������� g_pVSBlob�θ� ���

ComPtr<ID3DBlob>				g_pVSBlob;
ComPtr<ID3D11VertexShader>		g_pVS;


// �ȼ� ���̴�
ComPtr<ID3DBlob>				g_pPSBlob;
ComPtr<ID3D11PixelShader>		g_pPS;











// Vertex Shader
// Hull Shader			--> Tesselation
// Domain Shader		--> Tesselation
// Geometry Shader		
// Pixel Shader

// Compute Shader		-->	GPGPU


// Rendering Pipleline 
// ��ü�� �׷����� ���� (	3���� ������ 2�������� �׷����� ����)

// IA(Input Assembler) --> �׷��� ��ü�� ����(������ ���� ����), ������ ���� ������ �ԷµǴ� �ܰ�

// VertexShader		--> ���� �ϳ��� ����Ǵ� �Լ�, ������ NDC(���� ��ǥ��)�� �̵� ��Ű�� ���� 

// HullShader ,DomainShader (Tesselation) --> ������ ���������� ���߿� �뷮 ������Ŵ. (Dx11)

// GeometryShader --> ������ ���������� ���߿� ���� ������Ŵ. (Dx10)

// Rasterizer --> Culling (�������� �̷�� ������ ���� ������ ���� Culling)
//						   �ð������ �ո�, �ݽð�� �޸� ,����Ʈ �ɼǿ��� �޸��̸� ȭ�鿡 ��µ��� �ʴ´�.

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

	//�� 3���� ���ؽ� ����(gpu�޸���)�� �ְ� g_pVB���� ����
	DEVICE->CreateBuffer(&tBufferDesc, &tSubDesc, g_pVB.GetAddressOf());
	
	// Vertex Shader ������

	UINT iFlag = 0;

#ifdef  _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif


	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	

	
	
	//���ؽ� ���̴�(HLSL) ������

	// ���ϰ�� , ��ũ��, ��ũ��,�������� �Լ��� , Ÿ�� ����(��������� ���ϰ���), ���ؽ� ���̴� ����,�÷���, �÷��� ,�����ϰ��� ������ ��ü (Blob)�ּ�,

	HRESULT hr = D3DCompileFromFile(wstring(strContentPath+L"shader\\test.fx").c_str(), nullptr
		,D3D_COMPILE_STANDARD_FILE_INCLUDE,"VS_Test","vs_5_0", iFlag,0,g_pVSBlob.GetAddressOf(), g_pErrBlob.GetAddressOf());

		
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)g_pErrBlob->GetBufferPointer(),"Shader Compile Failed!!",MB_OK);
		assert(nullptr);
	}

	//������ �� �ڵ�� VertexShader ��ü �����
	//Blob�� �����ϰ��ִ� �޸� ������ ���� �ּ� ,���� , nullptr ,�������� ������ ���ؽ� ���̴�
	DEVICE->CreateVertexShader(g_pVSBlob->GetBufferPointer(), g_pVSBlob->GetBufferSize(), nullptr,g_pVS.GetAddressOf());


	//�ȼ� ���̴� ������


	 hr = D3DCompileFromFile(wstring(strContentPath + L"shader\\test.fx").c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_Test", "ps_5_0", iFlag, 0, g_pPSBlob.GetAddressOf(), g_pErrBlob.GetAddressOf());


 
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)g_pErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	//������ �� �ڵ�� VertexShader ��ü �����
	//Blob�� �����ϰ��ִ� �޸� ������ ���� �ּ� ,���� , nullptr ,�������� ������ �ȼ� ���̴�
	DEVICE->CreatePixelShader(g_pPSBlob->GetBufferPointer(), g_pPSBlob->GetBufferSize(), nullptr, g_pPS.GetAddressOf());


	// ���� �Է� ���� (���� ��� ������ ���� ���;� �Ǳ� ������ vector ���)
	vector<D3D11_INPUT_ELEMENT_DESC> arrDesc;
	UINT iOffset = 0;

	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};
	
	tInputDesc.SemanticName = "POSITION";						// Semantic �̸�
	tInputDesc.SemanticIndex = 0;								// �ߺ� �̸��� ���, �ε����� ����
	tInputDesc.InputSlot = 0;									// ���ؽ� ������ ������ġ
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// �������� �޸𸮸� ����� �� �ֵ��� ����
	tInputDesc.InstanceDataStepRate = 0;						// �ν��Ͻ����� 
	tInputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;			// DXGI_FORMAT�� ���·� R32G32B32(12byte)�� �˷���


	tInputDesc.AlignedByteOffset = iOffset;						//���� ��� ������ �޸��� ������ġ�� �� �� �ֵ��� ������ ����
	iOffset += 12;			//12byte�̹Ƿ� +12

	arrDesc.push_back(tInputDesc);

	
	tInputDesc.SemanticName = "COLOR";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// DXGI_FORMAT�� ���·� R32G32B32A32(16byte)�� �˷���
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 16;


	arrDesc.push_back(tInputDesc);
	// ��ǲ ���̾ƿ� ����
	//�Է� - ���� �����͸� �����ϴ� �Է� - ���̾ƿ� ��ü

	//Desc �迭, �迭�� ������ ����, �����ϵ� ���̴��� ���� ������, �����ϵ� ���̴��� ũ��, ������� �ּ�(layout)
	DEVICE->CreateInputLayout(arrDesc.data(), 2, g_pVSBlob->GetBufferPointer(), g_pVSBlob->GetBufferSize()
		, g_pInPutLayout.GetAddressOf());
}

void TestUpdate()
{

}

void TestRender()
{
	CDevice::GetInst()->ClearTarget();

	// render (�ؿ��� ������ �������, �� �Լ��� ����(����)�� �ѰŰ� �����Ѱ� �ƴϴ�)
	// ��ǲ ������� ���۵ɶ��� ���� ���ð����̶� Draw�� ȣ��Ǵ� ���� �ؿ��� �����ص� ������ ����Ǹ鼭 ����.
	
	// IA ����
	CONTEXT->IASetInputLayout(g_pInPutLayout.Get());

	
	UINT iStride = sizeof(VTX);
	UINT iOffset = 0;
	// 4��° -> ���������� ����, 5��°-> ������ġ ������
	CONTEXT->IASetVertexBuffers(0,1, g_pVB.GetAddressOf(),&iStride,&iOffset); //���ؽ� ���۸� �������� ���� ���� ����

	// ������ ������ �ﰢ���̶�� �˷��� (TRIANGLELIST)
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	CONTEXT->VSSetShader(g_pVS.Get(),0,0);		// ȣ��� ���ؽ� ���̴�
	CONTEXT->PSSetShader(g_pPS.Get(), 0, 0);	 //ȣ��� �ȼ� ���̴�

	//������ ���� ����
	CONTEXT->Draw(3, 0);


	CDevice::GetInst()->Present();
}
