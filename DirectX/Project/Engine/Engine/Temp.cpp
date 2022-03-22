#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

//���� ����
Vertex	arrVtx[4] = {};
Vec4	g_vPos;

ComPtr<ID3D11Buffer>			g_pVB; // ���� ����
ComPtr<ID3D11Buffer>			g_pIB; // �ε��� ����

ComPtr<ID3D11Buffer>			g_pCB; // ��� ����

// InputLayout ����(������ ���α��� ����)
ComPtr<ID3D11InputLayout>		g_pInPutLayout; 


// ���̴� : GPU�� ���� �� �Լ�


// ���̴� ������ ���н� ,���� ������ ������ Blob(���ڿ� ����� �����ϴ� ��ü)
ComPtr<ID3DBlob>				g_pErrBlob;


// ���ؽ� ���̴�

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
	

	//NDC�� ����ؼ� ��ǥ ǥ��
	//�ε��� ���۸� ����ؼ� �׸� ǥ��

	arrVtx[0].vPos	= Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);

	arrVtx[1].vPos	= Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor= Vec4(0.2f, 1.f, 0.2f, 1.f);

	arrVtx[2].vPos	= Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor= Vec4(0.2f, 0.2f, 1.f, 1.f);

	arrVtx[3].vPos	= Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);





	// ���� �����͸� ������ ���ؽ� ���۸� �����Ѵ�.
	D3D11_BUFFER_DESC tBufferDesc = {};

	//������ų ������ ������
	tBufferDesc.ByteWidth = sizeof(Vertex) * 6;


	// ���� ���۴� ó�� ���� ���Ŀ� ���۸� �������� �ʴ´�.
	tBufferDesc.CPUAccessFlags = 0;	
	tBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

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



	//�ε��� ����

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


	// ������� �ݵ�� 16����Ʈ ������ �����ߵȴ�

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




	// System �޸𸮿� �ִ� �����͸� gpu�޸𸮿� �ű� �� �ӽ� �޸𸮸� ����� map�Լ��� ������ ��Ų������
	// Unmap�� �̿��ؼ� �ӽø޸𸮸� gpu �޸𸮿� �ű��.
	
	// �̷�������� ��ġ�� �ű� �� ������ Ű�� �Է����� �ʾƵ� ���� ����⵵ �ϰ�
	// ���ؽ� ������ü�� �����ϱ� ������ ������ ������ �������ٸ�
	// �������Ӹ��� ��� ������ �����ؾ߱⵵ �ϰ� ���� ������ �����ϴ� �ٸ� ��ü�� ���µ� �ٲ��
	
	// ���ʷ� ������� ���۴� ���(����)�� �����ϴ� ���̰� 
	// �׷��⿡ ���ؽ� ���̴����� ��� ȣ���� �� ��� ������ ���� ���� �̵����� �شٸ�
	// ���� ����� �����ϸ鼭 ��ġ�� �̵���ų �� ����.

	// ���ؽ� ���� ����
	// ���۰� ���������� �� cpuAccessFlag�� write�̿��߸� ��.

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

	// ������ۿ��� �߰����� �̵����� �� ����
	// �� �̵�����ŭ VB�� �����༭ �����Ͷ������� �ű���

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(g_pCB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub);
	memcpy(tSub.pData, &g_vPos, sizeof(Vec4));
	CONTEXT->Unmap(g_pCB.Get(), 0);



	//D3D11_MAPPED_SUBRESOURCE tSub = {};

	//CONTEXT->Map(g_pVB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub); //�ӽ� �޸𸮿� ���ؽ� ���۸� ����
	//memcpy(tSub.pData, arrVtx, sizeof(VTX) * 4);	//arrVtx���� �ӽ� �޸𸮿� ����
	//CONTEXT->Unmap(g_pVB.Get(), 0);

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

	//�ε��� ���� �ּ�, ������ 4byte UINT , ������
	CONTEXT->IASetIndexBuffer(g_pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	// ������ ������ �ﰢ���̶�� �˷��� (TRIANGLELIST)
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->VSSetConstantBuffers(0, 1, g_pCB.GetAddressOf());//���ؽ� ���̴� ������ ������ ����(�������)
	
	CONTEXT->VSSetShader(g_pVS.Get(),0,0);		// ȣ��� ���ؽ� ���̴�
	CONTEXT->PSSetShader(g_pPS.Get(), 0, 0);	 //ȣ��� �ȼ� ���̴�

	//������ ���� ����
	//CONTEXT->Draw(6, 0);  Draw�� ���ؽ� ���۸� ���
	CONTEXT->DrawIndexed(6, 0, 0);


	CDevice::GetInst()->Present();
}
