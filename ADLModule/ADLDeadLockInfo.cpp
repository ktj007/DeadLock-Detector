#include "StdAfx.h"
#include "ADLDeadLockInfo.h"

ADLDeadLockInfo* ADLDeadLockInfo::m_pDeadLockInfo = NULL;

ADLDeadLockInfo::ADLDeadLockInfo(void)
: m_iTopThreadPos(0)
{	
	memset(m_pDeadLockThreadInfo, 0, sizeof(stDeadLockThreadInfo) * ADL_MAX_THREAD);	
}

ADLDeadLockInfo::~ADLDeadLockInfo(void)
{
}

void ADLDeadLockInfo::SetInfomation(BYTE btEnterCritical)
{
	INT iIndex = 0;
	BOOL bNewThread = TRUE;
	INT iTopThreadPos = m_iTopThreadPos;

	DWORD dwThreadID = GetCurrentThreadId();
	DWORD dwTickCount = GetTickCount();

	if(m_iTopThreadPos >= ADL_MAX_THREAD)
		return;

	for(iIndex = 0; iIndex < iTopThreadPos; iIndex++)
	{
		if(m_pDeadLockThreadInfo[iIndex].dwThreadID == dwThreadID)
		{
			bNewThread = FALSE;
			break;
		}
	}

	if(ADL_MAX_THREAD <= iIndex)
		return;

	if(TRUE == bNewThread) //�� �������� ������ ����ϴ� ��Ȳ
	{
	
		DWORD Value = InterlockedCompareExchange((LONG*)&m_iTopThreadPos, iTopThreadPos + 1, iTopThreadPos);

		if(Value != iTopThreadPos) // �������� ������ �Լ� ���� �Ͼ �� �ٽ� �õ�����. ���� Ȯ���� 0%������ �������ڸ��� ������� �ɸ��ٸ�...
		{
			return;	
		}
	}

	m_pDeadLockThreadInfo[iIndex].dwThreadID = dwThreadID;
	m_pDeadLockThreadInfo[iIndex].dwTickCount = dwTickCount;
	m_pDeadLockThreadInfo[iIndex].btEnterCriticalSection = btEnterCritical;
}


BOOL ADLDeadLockInfo::IsDeadLockDetected(DWORD dwTickCount)
{
	INT iIndex = 0;
	BOOL bRet = FALSE;
	
	INT iTopThreadPos = m_iTopThreadPos;

	for(iIndex = 0; iIndex < iTopThreadPos; iIndex++)
	{
		if(m_pDeadLockThreadInfo[iIndex].dwThreadID == 0)
			continue;
		
//LeaveCriticalSection�� ��Ȳ. ������ EnterCriticalSection�� ��Ȳ���� ���ߴϱ� �� ���� ������ �ʴ� ��Ȳ�̶�� �Ǵ�
//������ Ǯ ���¿����� �翬�� btEnterCriticalSection = 0;
		if(m_pDeadLockThreadInfo[iIndex].btEnterCriticalSection == 0) 
			continue;

//���� ��� �ְ� �� ���ķ� �Լ� ��ŷ�� �Ͼ�� �ʾҴٴ� ���� ������� �Ͼ�ٴ� ����..
//�극��ũ ����Ʈ�� ������ Ǯ�� �翬�� �ð����� Ŀ���Ƿ� ��������� ������ �� �ִ�.
//�� ����� Ȱ���Ѵٸ� �ǵ��� ������� ���� �� ��
		if(dwTickCount - m_pDeadLockThreadInfo[iIndex].dwTickCount > ADL_DEADLOCK_TIMEOUT)
		{
			m_pDeadLockThreadInfo[iIndex].bIsDeadLockThread = TRUE;
			bRet = TRUE;
		}
	}

	return bRet;
}