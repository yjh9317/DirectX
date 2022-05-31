#pragma once
#include "UI.h"

class TreeUI;

class TreeNode
{
private:
    TreeNode* m_pParent;
    vector<TreeNode*>   m_vecChild;
    bool                m_bLeaf;

    string              m_strName;
    DWORD_PTR           m_dwData;


public:
    void update();
    void render_update();

public:
    void AddChild(TreeNode* _pChild)
    {
        _pChild->m_pParent = this;
        m_vecChild.push_back(_pChild);
        m_bLeaf = false;
    }

    DWORD_PTR GetData() { return m_dwData; }


public:
    TreeNode();
    TreeNode(const string& _strName, DWORD_PTR _dwData);
    ~TreeNode();

    friend class TreeUI;
};





class TreeUI :
    public UI
{
private:
    TreeNode* m_pRootNode;
    const bool  m_bUseDummyRoot;
    bool        m_bShowDummy;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void ShowDummyRoot(bool _bTrue) { m_bShowDummy = _bTrue; }
    TreeNode* AddTreeNode(TreeNode* _pParentNode, const string& _strName, DWORD_PTR _dwData = 0);


public:
    TreeUI(bool _bDummyRoot);
    ~TreeUI();
};

