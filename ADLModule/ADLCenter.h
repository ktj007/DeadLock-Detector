#pragma once
//20100403 ADL(Anti DeadLock) Module Center
//Made by Park Ju-Hang
//E-Mail Address : juhang3@daum.net
//���ڿ� ��� ��ġ�� �����ϰ� ���� ��� �� Ŭ������ ����ؼ� OnOutput�� ������ �ֽø� �˴ϴ�.

#include <windows.h>
#include <vector>
#include "ADLLock.h"

class ADLDeadLockDetectionThread;

enum REPORT_MODE
{
	ADL_STACKREPORT_PDB = 0,
	ADL_STACKREPORT_ADDRESS,
};

#define ADL_THREAD_INTERVAL 10000

class ADLCenter
{
	friend class ADLDeadLockDetectionThread;

public:
	ADLCenter(void);
	virtual ~ADLCenter(void);

	virtual void OnOutput(LPCSTR szText);	

	void SetReportMode(REPORT_MODE eReportMode){m_eReportMode = eReportMode;}
	REPORT_MODE GetReportMode(){return m_eReportMode;}
	
	BOOL KillDeadLockThread();
	BOOL IsDeadLocked(){return 0 != m_vecDeadLockThreadList.size();}

	BOOL Begin();
	BOOL End();

protected:
	void PushDeadLockThreadID(DWORD dwThreadID);

private:
	ADLDeadLockDetectionThread* m_pDeadLockDetectionThread;
	std::vector<DWORD> m_vecDeadLockThreadList;
	ADLLock m_ADCLock;
	REPORT_MODE m_eReportMode;
};
