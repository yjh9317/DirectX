#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"
#include "CDevice.h"
#include "CConstBuffer.h"


CMaterial::CMaterial()
	: m_pShader(nullptr)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::SCALAR_PARAM);
	pCB->SetData(&m_Param, sizeof(tScalarParam));
	pCB->UpdateData();


	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData();
	}
}

void CMaterial::SetShader(CGraphicsShader* _pShader)
{
	m_pShader = _pShader;

	if (nullptr != m_pShader)
	{
		m_vecParamInfo = m_pShader->GetParamInfo();
	}
}

void CMaterial::SetScalarParam(SCALAR_PARAM _eType, void* _pData)
{
	switch (_eType)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
		m_Param.iArr[(UINT)_eType - (UINT)SCALAR_PARAM::INT_0] = *((int*)_pData);
		break;
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
		m_Param.fArr[(UINT)_eType - (UINT)SCALAR_PARAM::FLOAT_0] = *((float*)_pData);
		break;
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
		m_Param.v2Arr[(UINT)_eType - (UINT)SCALAR_PARAM::VEC2_0] = *((Vec2*)_pData);
		break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
		m_Param.v4Arr[(UINT)_eType - (UINT)SCALAR_PARAM::VEC4_0] = *((Vec4*)_pData);
		break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
		m_Param.matArr[(UINT)_eType - (UINT)SCALAR_PARAM::MAT_0] = *((Matrix*)_pData);
		break;
	}
}

void CMaterial::SetScalarParam(const wstring& _strParamName, void* _pData)
{
	for (size_t i = 0; i < m_vecParamInfo.size(); ++i)
	{
		if (m_vecParamInfo[i].strDesc == _strParamName)
		{
			SetScalarParam(m_vecParamInfo[i].eScalarParam, _pData);
			break;
		}
	}
}
