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
//				- 실행되는 동안 프로세스는 여러개의 새로운 자식 프로세스들을 생성할 수 있음
//				- 각 프로세스는 고유한 프로세스 식별자(pid)를 가짐
//				- pid가 1인 init 프로세스가 모든 프로세스의 부모 프로세스(루트)
//				- 부모 프로세스가 wait()를 호출하지 않고 종료하면 자식 프로세스는 고아 프로세스가 됨(orphan process)
//				- Linux, UNIX에서 init 프로세스는 주기적으로 wait()를 호출하여 고아 프로세스의 종료 상태를 수집함
//				- 자식 프로세스는 부모 프로세스로부터 자원, 특권, 스케줄링 속성 등을 상속받음
//				- *자식 프로세스가 exec()를 호출하지 않고 부모 프로세스의 복사본을 계속 실행하는 것을 막을 방법?
//				- ps -el로 프로세스 목록 확인할 수 있음
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
//	Windows API를 이용해 새로운 프로세스를 생성하는 프로그램
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <window.h>

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//	메모리 할당
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//	자식 프로세스 생성
	if (!CreateProcess(NULL,				//	명령어 라인 사용
		"C:\\WINDOWS\system32\mspaint.exe",	//	명령어 라인
		NULL,		//	프로세스를 상속하지 말 것
		NULL,		//	스레드 핸들을 상속하지 말 것
		FALSE,		//	핸들 상속 디제이블
		0,			//	생성 플래그 없음
		NULL,		//	부모 환경 블록 사용
		NULL,		//	부모 프로세스가 존재하는 디렉터리 사용
		&si,
		&pi))
	{
		fprintf(stderr, "Create Process Failed");
		return -1;
	}

	//	부모 프로세스가 자식 프로세스가 끝나길 기다림
	WaitForSingleObject(pi, hProcess, INFINITE);
	printf("Child Complete");

	//	핸들 닫기
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}