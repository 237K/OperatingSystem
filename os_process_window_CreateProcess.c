///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Windows
//	2019. 04. 20
//
//	<Process>
//		1. Process Scheduling
//			1) Scheduling Queue
//			2) Schedulers
//				a) Job Scheduler
//				b) CPU Scheduler
//				c) Medium-term Scheduler
//					- Swapping
//			3) Context Switch
//
//		2. Operation on Process
//			1) Process Creation
//				- ����Ǵ� ���� ���μ����� �������� ���ο� �ڽ� ���μ������� ������ �� ����
//				- �� ���μ����� ������ ���μ��� �ĺ���(pid)�� ����
//				- pid�� 1�� init ���μ����� ��� ���μ����� �θ� ���μ���(��Ʈ)
//				- �θ� ���μ����� wait()�� ȣ������ �ʰ� �����ϸ� �ڽ� ���μ����� ��� ���μ����� ��(orphan process)
//				- Linux, UNIX���� init ���μ����� �ֱ������� wait()�� ȣ���Ͽ� ��� ���μ����� ���� ���¸� ������
//				- �ڽ� ���μ����� �θ� ���μ����κ��� �ڿ�, Ư��, �����ٸ� �Ӽ� ���� ��ӹ���
//				- *�ڽ� ���μ����� exec()�� ȣ������ �ʰ� �θ� ���μ����� ���纻�� ��� �����ϴ� ���� ���� ���?
//				- ps -el�� ���μ��� ��� Ȯ���� �� ����
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
//	Windows API�� �̿��� ���ο� ���μ����� �����ϴ� ���α׷�
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <window.h>

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//	�޸� �Ҵ�
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//	�ڽ� ���μ��� ����
	if (!CreateProcess(NULL,				//	��ɾ� ���� ���
		"C:\\WINDOWS\system32\mspaint.exe",	//	��ɾ� ����
		NULL,		//	���μ����� ������� �� ��
		NULL,		//	������ �ڵ��� ������� �� ��
		FALSE,		//	�ڵ� ��� �����̺�
		0,			//	���� �÷��� ����
		NULL,		//	�θ� ȯ�� ��� ���
		NULL,		//	�θ� ���μ����� �����ϴ� ���͸� ���
		&si,
		&pi))
	{
		fprintf(stderr, "Create Process Failed");
		return -1;
	}

	//	�θ� ���μ����� �ڽ� ���μ����� ������ ��ٸ�
	WaitForSingleObject(pi, hProcess, INFINITE);
	printf("Child Complete");

	//	�ڵ� �ݱ�
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}