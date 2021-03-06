#pragma once
#include "CComponent.h"

// 카메라는 하나의 오브젝트를 만들어서 카메라역할을 수행
// View Space(Camera Space): 카메라의 위치,방향을 기준으로 물체들의 Transform값을 적용
// View Space에서는 카메라의 위치가 원점, 카메라가 바라보는 방향은 전방방향이어야 한다.

enum class PROJ_TYPE // 카메라의 투영 모드
{
    ORTHOGRAPHIC,    // 직교투영
    PERSPECTIVE,   // 원근투영,  원근투영은 툴에서 사용하기 좋다.
};

class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*>    m_vecForward;   // 불투명 물체
    vector<CGameObject*>    m_vecMasked;    // 투명 , 불투명 물체
    vector<CGameObject*>    m_vecTranslucent;    // 반투명 물체
    vector<CGameObject*>    m_vecPostProcess;       // 후 처리


protected:
    Matrix                  m_matView;       // View행렬, 이 행렬을 곱하면 View Space로 넘어온다.
    Matrix                  m_matProj;       // 투영 행렬(View Space로 넘어온 물체를 -1 ~ 1범위로 압축할 때 필요한 행렬)

    PROJ_TYPE               m_eProjType;

    float                   m_fWidth;       // 투영 가로 길이
    float                   m_fAspectRatio; // 종횡비 : (가로 / 세로)
    float                   m_fFOV;         // Field of View (시야각)
    float                   m_fFar;         // 최대 시야 거리


    UINT                    m_iLayerMask;
    int                     m_iCamIdx;


public:
    void SetCameraAsMain();
    void SetCameraIndex(int _iIdx);
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetFOV(float _FOV) { m_fFOV = _FOV; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    void SetFar(float _Far) { m_fFar = _Far; }

    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    float GetFOV() { return m_fFOV; }
    float GetFar() { return m_fFar; }

    PROJ_TYPE GetProjType() { return m_eProjType; }

    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll() { m_iLayerMask = 0xffffffff; }


    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

private:
    void SortGameObject();  // Shader Domain에 따른 물체 분류 (이벤트 처리)


public:
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_translucent();
    void render_postprocess();

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CCamera)


public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;
};

