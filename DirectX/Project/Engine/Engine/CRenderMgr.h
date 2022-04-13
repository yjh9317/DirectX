#pragma once
#include "singleton.h"

class CRenderMgr :
	 public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:

public:
	void init();

};

