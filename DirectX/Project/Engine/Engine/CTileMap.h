#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"

// TileMap을 Object로 각각 만들면 비용이 너무 많이 들기 때문에 렌더링 컴포넌트로 만들고
// 그안에서 RectMesh를 이용하여 타일을 표현

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_pAtlasTex; // Scene마다 사용하는 Atlas가 다르므로 Material이 아닌 Tilemap에서 관리
    // TileMap은 Material에서 Shader를 공유하고있으므로 Material이 아니라 클래스 내에서 변수로 관리

    Vec2            m_vSlicePixel; // Atlas에서 한칸에서 사용하는 픽셀의 크기
    Vec2            m_vSliceUV;     // 한칸의 UV
    Vec2            m_vLTUV;

    UINT            m_iRowCount;
    UINT            m_iColCount;
    
    int             m_iImgIdx;

public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex) {  m_pAtlasTex = _pAtlasTex;}
    void SetTileSize(Vec2 _vPixelSize) { m_vSlicePixel = _vPixelSize; }

    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

