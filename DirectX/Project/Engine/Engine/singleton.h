#pragma once

template<typename T>
class CSingleton
{
	typedef void(*DESTORY)(void);

private:
	static T* m_Inst;

public:
	static T* GetInst()
	{
		if (nullptr == m_Inst)
		{
			m_Inst = new T;
		}

		return m_Inst;
	}

	static void Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}

public:
	CSingleton()
	{
		atexit((DESTORY)CSingleton<T>::Destroy);
		// 함수포인터를 넣어놓으면 함수포인터가 스택처럼 쌓여서 메인함수가 끝날때
// 등록된 함수를 맨 마지막에 등록된 순서부터 해제한다(Stack), Mgr class의 호출순서 중요(많이 참조할수록 먼저 호출)
	}

	virtual ~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;