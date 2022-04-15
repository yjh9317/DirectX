#pragma once
#include "CComponent.h"

// ī�޶�� �ϳ��� ������Ʈ�� ���� ī�޶����� ����
// View Space(Camera Space): ī�޶��� ��ġ,������ �������� ��ü���� Transform���� ����
// View Space������ ī�޶��� ��ġ�� ����, ī�޶� �ٶ󺸴� ������ z���̴�.

enum class PROJ_TYPE // ī�޶��� ���� ���
{
    ORTHOGRAPHIC,    // ��������
    PERSPECTIVE,   // ��������,  ���������� ������ ����ϱ� ����.
};

class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*>    m_vecForward;   // ������ ��ü
    vector<CGameObject*>    m_vecMasked;    // ���� , ������ ��ü
    vector<CGameObject*>    m_vecOpaque;    // ������ ��ü
    
    Matrix                  m_matView;       // View���, �� ����� ���ϸ� View Space�� �Ѿ�´�.
    Matrix                  m_matProj;       // ���� ���(View Space�� �Ѿ�� ��ü�� -1 ~ 1������ ������ �� �ʿ��� ���)

    float                   m_fWidth;       // ���� ���� ����
    float                   m_fAspectRatio; // ��Ⱦ�� : (���� / ����)
    float                   m_fFOV;         // Field of View (�þ߰�)
    float                   m_fFar;         // �ִ� �þ� �Ÿ�
    

    PROJ_TYPE               m_eProjType;


    UINT                    m_iLayerMask;   
    int                     m_iCamIdx;


public:
    void SetCameraAsMain();
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }

    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    PROJ_TYPE GetProjType() { return m_eProjType; }

    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll()  { m_iLayerMask = 0xffffffff; }


    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

private:
    void SortGameObject();  // Shader Domain�� ���� ��ü �з� (�̺�Ʈ ó��)


public:
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_opaque();
    

    CLONE(CCamera)

        
public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;
};

