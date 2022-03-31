#pragma once

template<typename T>	//� ���ҽ������� ���ٰ����� ���ø�
class Ptr
{
private:
	T*	m_pRes;
	
public:
	T* Get() { return m_pRes; }
	T* Get() const { return m_pRes; }

	T* GetAdressOf() { return &m_pRes; }
	T** GetAdressOf() const { return &m_pRes; }


	//GetAdressOf�� ���������ͷ� �޾ƿ���(�ּҷ� �޾ƿͼ� ������ �ٲٹǷ�) ����ī��Ʈ�� �۵����� �ʴ´�

public:
	Ptr& operator = (T* _ptr)
	{
		if (nullptr != m_pRes)
			m_pRes->SubRef();	//������ ����Ű�� �ִ� �ּ��� ����ī��Ʈ�� �����ش�.


		m_pRes = _ptr;

		if (nullptr != m_pRes)
			m_pRes->AddRef();	// ���� ����Ű�� �ִ� �ּ��� ����ī��Ʈ�� �÷��ش�.

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

	bool operator == (T* _pRes)	//���ҽ��� �ּҿ� ����Ʈ������ ��
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
	
	bool operator == (const Ptr<T>& _OtherPtr) //���� ����Ʈ�����ͳ��� �񱳿�����
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

	// ����Ʈ������ Ÿ���� ��ȯ�޴� Get�Լ��� ����ּ�(m_pRes)�� ��ȯ���Ѽ� �޾ƿ��� ���۷�����, ������ �Ǽ��� ������ ã�� ���ϹǷ� ������ ����.
	//operator T* ()
	//{
	//	return m_pRes;
	//}

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