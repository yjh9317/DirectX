#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"

// TileMap�� Object�� ���� ����� ����� �ʹ� ���� ��� ������ ������ ������Ʈ�� �����
// �׾ȿ��� RectMesh�� �̿��Ͽ� Ÿ���� ǥ��

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_pAtlasTex; // Scene���� ����ϴ� Atlas�� �ٸ��Ƿ� Material�� �ƴ� Tilemap���� ����
    // TileMap�� Material���� Shader�� �����ϰ������Ƿ� Material�� �ƴ϶� Ŭ���� ������ ������ ����

    Vec2            m_vSlicePixel; // Atlas���� ��ĭ���� ����ϴ� �ȼ��� ũ��
    Vec2            m_vSliceUV;     // ��ĭ�� UV
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

