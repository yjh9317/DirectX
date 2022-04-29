#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleUpdateShader>      m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer* m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    UINT                            m_iMaxCount;        // ��ƼŬ �ִ� ����


    int                             m_bPosInherit;      // ������Ʈ ��ǥ�� ����


public:
    virtual void finalupdate() override;
    virtual void render()   override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

