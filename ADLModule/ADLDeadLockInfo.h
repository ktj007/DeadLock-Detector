#pragma once

typedef struct tag_DeadLockThreadInfo
{
	DWORD dwThreadID;
	DWORD dwTickCount;
	BYTE btEnterCriticalSection;
	BOOL bIsDeadLockThread;

	tag_DeadLockThreadInfo()
	{
		dwThreadID = 0;
		dwTickCount = 0;
		btEnterCriticalSection = 0;
		bIsDeadLockThread = FALSE;
	}
}stDeadLockThreadInfo;

#define ADL_DEADLOCK_TIMEOUT 5000 //����� �ɷȴٰ� �ǴܵǴ� �ð� 5�ʵ��� �ð� ������Ʈ�� �������� Lock ī��Ʈ�� �ϳ��� ������ ������̴�.
#define ADL_MAX_THREAD 1000 //�׽�Ʈ�� ���� ������ �ִ� ����. �׳� ũ�� �����

class ADLDeadLockInfo
{
public:
	ADLDeadLockInfo(void);
	virtual ~ADLDeadLockInfo(void);

	static ADLDeadLockInfo* GetInstance()
	{
		if(NULL == m_pDeadLockInfo)
		{
			m_pDeadLockInfo = new ADLDeadLockInfo();
		}

		return m_pDeadLockInfo;
	}

	BOOL IsDeadLockDetected(DWORD dwTickCount);
	void SetInfomation(BYTE btEnterCritical);	

	stDeadLockThreadInfo* GetThreadInfo(){return m_pDeadLockThreadInfo;}
	INT GetThreadSize(){return m_iTopThreadPos;}

protected:

private:
	static ADLDeadLockInfo* m_pDeadLockInfo;
	stDeadLockThreadInfo m_pDeadLockThreadInfo[ADL_MAX_THREAD];
	INT m_iTopThreadPos;
};
