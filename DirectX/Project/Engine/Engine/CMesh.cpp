#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"


CMesh::CMesh()
	: CRes(RES_TYPE::MESH)
	, m_tVBDesc{}
	, m_tIBDesc{}
	, m_iVtxCount(0)
	, m_iIdxCount(0)
	, m_pVtxSys(nullptr)
	, m_pIdxSys(nullptr)
{
}


CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSys); // 시스템 메모리에 있는 버텍스 버퍼 삭제
	SAFE_DELETE(m_pIdxSys); // 시스템 메모리에 있는 인덱스 버퍼 삭제
}



int CMesh::Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount)
{
	m_iVtxCount = _iVtxCount;
	m_iIdxCount = _iIdxCount;

	// 버텍스 버퍼의 개수만큼 생성
	m_tVBDesc.ByteWidth = sizeof(Vertex) * _iVtxCount;

	// 버텍스 버퍼 수정 X
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;

	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.MiscFlags = 0;
	m_tVBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = _pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_tIBDesc.ByteWidth = sizeof(UINT) * _iIdxCount;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	m_tIBDesc.CPUAccessFlags = 0;
	m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	m_tIBDesc.MiscFlags = 0;
	m_tIBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체
	tSubDesc = {};
	tSubDesc.pSysMem = _pIdxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSubDesc, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}


	//시스템메모리의 지역변수를 받아오면 그 함수가 끝날 때 주소가 날라가므로 내부적으로 복사해서 관리
	m_pVtxSys = new Vtx[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, sizeof(Vtx) * m_iVtxCount);

	m_pIdxSys = new UINT[m_iIdxCount];
	memcpy(m_pIdxSys, _pIdxSys, sizeof(UINT) * m_iIdxCount);

	return S_OK;
}

void CMesh::render()
{
	UpdateData();

	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);	//버텍스 버퍼의 데이터를 인덱스 버퍼를 사용하여 렌더링
}

void CMesh::render_particle(UINT _iCount)
{
	UpdateData();
	
	// 한번에 _iCount만큼 render하고 끝나고 나서 돌아옴(한번에 처리하므로 여러번 호출X)
	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::UpdateData()
{


	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	//버퍼를 gpu 메모리에 전달 
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}
