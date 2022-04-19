#include "pch.h"
#include "CAnimation2D.h"

#include "CAnimator2D.h"
#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CAnimation2D::CAnimation2D()
	:m_iCurFrmIdx(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
{
}

void CAnimation2D::finalupdate()
{
	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrmIdx].fDuration < m_fAccTime)
	{
		m_fAccTime -= m_vecFrm[m_iCurFrmIdx].fDuration;

		if (m_vecFrm.size() - 1 <= m_iCurFrmIdx)
		{
			m_bFinish = true;
		}
		else
		{
			++m_iCurFrmIdx;
		}
	}
}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::ANIM2D);

	tAnim2D info = {};
	info.useAnim2D = 1;
	info.Atlas_Width = m_pAtlasTex->Width();
	info.Atlas_Height = m_pAtlasTex->Height();

	info.vLT = m_vecFrm[m_iCurFrmIdx]._vLT;
	info.vSlice = m_vecFrm[m_iCurFrmIdx]._vSlice;
	
	pBuffer->SetData(&info, sizeof(tAnim2D));
	pBuffer->UpdateData();

	m_pAtlasTex->UpdateData((UINT)PIPELINE_STAGE::PS,10);
}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration, int _iFrameCount)
{
	assert(_Atlas.Get());
	m_pAtlasTex = _Atlas;

	// �ȼ���ǥ�� 500,600�̶�� �ص� UV���� �ؽ�ó ��üũ�Ⱑ ������ ������ ����

	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();


	//�ȼ� ��ǥ�� 0~1 UV�� ��ȯ
	Vec2 vLT = _vLT / Vec2(fWidth, fHeight);
	Vec2 vSlice = _vSlice / Vec2(fWidth, fHeight);
	Vec2 vStep = _vStep / Vec2(fWidth, fHeight);
	

	//������ ���� ����
	for (int i = 0; i < _iFrameCount; ++i)
	{
		tAnim2DFrame frm = {};
	
		//_vLT += (_vStep * (float)i);
		frm._vLT = _vLT + (vStep * (float)i);
		frm._vSlice = vSlice;
		frm.fDuration = _fDuration;

		m_vecFrm.push_back(frm);	
	}
}

CAnimation2D::~CAnimation2D()
{
}
