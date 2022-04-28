#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleShader>    m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer*  m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    UINT                m_iMaxCount;        // ��ƼŬ �ִ� ����

private:
    virtual void finalupdate() override;
    virtual void render() override;

    
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

