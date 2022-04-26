#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"
#include "CStructuredBuffer.h"

// Tile�� Object�� ���� ����� ����� �ʹ� ���� ��� ������ ������ ������Ʈ�� �ϳ��� ������Ʈ�� �����
// �׾ȿ��� RectMesh�� �̿��Ͽ� Ÿ���� ǥ��

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_pAtlasTex; // Scene���� ����ϴ� Atlas�� �ٸ��Ƿ� Material�� �ƴ� Tilemap���� ����
    // TileMap�� Material���� Shader�� �����ϰ������Ƿ� Material�� �ƴ϶� Ŭ���� ������ ������ ����

    Vec2              m_vSlicePixel; // Atlas���� ��ĭ���� ����ϴ� �ȼ��� ũ��
    Vec2              m_vSliceUV;     // ��ĭ�� UV
    

    UINT              m_iRowCount;
    UINT              m_iColCount;
    
    UINT              m_iTileCountX;
    UINT              m_iTileCountY;
    vector<tTileData> m_vecTileData; //Ÿ�ϸ��� ���� ������ ���� �ϱ����� ���ͷ� ����
    CStructuredBuffer* m_pBuffer; // Ÿ�ϸʸ��� ����� �ٸ��Ƿ� ���� ���� ��� �־�� �Ѵ�.

    bool                m_bBufferUpDated; //������Ʈ�� ����, ������ ������ ���� ��� ������ ������ �����ʾ�(������ �����ͻ��) �������� �ø�

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




    CLONE(CTileMap);
public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};
