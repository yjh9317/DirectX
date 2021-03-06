#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"
#include "CStructuredBuffer.h"

// Tile을 Object로 각각 만들면 비용이 너무 많이 들기 때문에 렌더링 컴포넌트로 하나의 오브젝트로 만들고
// 그안에서 RectMesh를 이용하여 타일을 표현

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_pAtlasTex; // Scene마다 사용하는 Atlas가 다르므로 Material이 아닌 Tilemap에서 관리
    // TileMap은 Material에서 Shader를 공유하고있으므로 Material이 아니라 클래스 내에서 변수로 관리

    Vec2              m_vSlicePixel; // Atlas에서 한칸에서 사용하는 픽셀의 크기
    Vec2              m_vSliceUV;     // 한칸의 UV
    

    UINT              m_iRowCount;
    UINT              m_iColCount;
    
    UINT              m_iTileCountX;
    UINT              m_iTileCountY;
    vector<tTileData> m_vecTileData; //타일마다 각각 정보를 갖게 하기위해 벡터로 관리
    CStructuredBuffer* m_pBuffer; // 타일맵마다 사이즈가 다르므로 각각 따로 들고 있어야 한다.

    bool                m_bBufferUpDated; //업데이트의 유무, 버퍼의 변경이 없을 경우 데이터 세팅을 하지않아(기존의 데이터사용) 프레임을 올림

public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;


public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex) { m_pAtlasTex = _pAtlasTex;
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }
    void SetTileSize(Vec2 _vPixelSize) {
        m_vSlicePixel = _vPixelSize;
        if(nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }
    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void ClearTileData();



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CTileMap);
public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};
