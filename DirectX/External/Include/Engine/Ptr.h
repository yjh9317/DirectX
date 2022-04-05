#pragma once

template<typename T>	//어떤 리소스에서도 접근가능한 템플릿
class Ptr
{
private:
	T*	m_pRes;

public:
	T* Get() { return m_pRes;}
	T* Get() const { return m_pRes; }

	T* GetAdressOf() { return &m_pRes; }
	T** GetAdressOf() const {return &m_pRes;}

	//GetAdressOf로 이중포인터로 받아오면(주소로 받아와서 강제로 바꾸므로) 참조카운트가 작동되지 않는다

public:
	Ptr& operator = (T* _ptr)
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();	// 참조카운트 감소

		m_pRes = _ptr;

		if (nullptr != m_pRes)
			m_pRes->AddRef();	// 참조카운트 증가

		return *this;
	}

	Ptr& operator = (const Ptr<T>& _otherPtr)
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();

		m_pRes = _otherPtr.m_pRes;

		if (nullptr != _otherPtr.m_pRes)
			m_pRes->AddRef();

		return *this;
	}

	T* operator ->()
	{
		return m_pRes;
	}

	bool operator == (T* _pRes)	//리소스의 주소와 스마트포인터 비교
	{
		if (m_pRes == _pRes)
			return true;
		else
			return false;
	}

	bool operator != (T* _pRes)
	{
		return !(*this == _pRes);
	}

	bool operator == (const Ptr<T>& _OtherPtr) //같은 스마트포인터끼리 비교연산자
	{
		if (m_pRes == _OtherPtr.m_pRes)
			return true;
		else
			return false;
	}

	bool operator != (const Ptr<T>& _OtherPtr)
	{
		return !(*this == _OtherPtr);
	}

	/*operator T* ()
	{
		return m_pRes;
	}*/


public:
	Ptr()
		: m_pRes(nullptr)
	{}

	Ptr(T* _pRes)
		: m_pRes(_pRes)
	{
		if (nullptr != m_pRes)
			m_pRes->AddRef();	
	}	

	Ptr(const Ptr<T>& _otherPtr)
		: m_pRes(_otherPtr.m_pRes)
	{
		if (nullptr != m_pRes)
			m_pRes->AddRef();
	}

	~Ptr()
	{
		if (nullptr != m_pRes)
		{
			m_pRes->SubRef();
		}
	}
};


template<typename T>
bool operator == (void* _pRes, const Ptr<T>& _ptr)
{
	if (_pRes == _ptr.Get())
		return true;

	return false;
}

template<typename T>
bool operator != (void* _pRes, const Ptr<T>& _ptr)
{
	if (_pRes == _ptr.Get())
		return false;

	return true;
}