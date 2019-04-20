///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Linux
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
//	fork() 시스템 호출을 사용하여 별도의 프로세스를 생성하는 프로그램
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <unistd.h>

main(int argc, char** argv)
{
	int pid;

	pid = fork();		//	새 프로세스 생성

	if (pid < 0)				//	오류 발생했을 경우
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (!pid)				//	자식 프로세스
		execlp("/bin/ls", "ls", NULL);
	else					//	부모 프로세스
	{
		wait(NULL);			//	자식이 완료되길 기다림
		printf("Child Complete");
	}
	return 0;
}
