///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	OS Windows
//	2019. 04. 22
//
//	[Operating System]
//		<4. 스레드>
//			(1. 다중코어 프로그래밍
//			 2. 다중 스레드 모델
//			 3. 스레드 라이브러리
//			 4. 암묵적 스레딩
//			 5. 스레드 관련 이슈
//			 6. 사례)
//
//				1. 다중 스레드 프로그래밍의 장점
//					1) 응답성(Responsiveness)
//					   시간이 많이 걸리는 연산을 별도의 스레드에서 실행되도록 함으로써 사용자에 대한 응답성 증가
//					2) 자원 공유(Resource Sharing)
//			   		   프로세스와 달리 스레드들은 자동으로 프로세스의 자원을 공유함
//					3) 경제성
//					   당연히 새로운 프로세스를 만드는 것보다 메모리나 오버헤드를 훨씬 적게 먹음
//					4) Scalability
//				   	   Multiprocessor 구조에서 각 스레드가 다른 처리기에서 병렬로 수행될 수 있음
//				2. 다중 스레드 모델
//					1) 다 대 일 모델 : 다중처리코어구조의 이점을 살릴 수 없어서 잘 안씀
//					2) 일 대 일 모델
//					   각 사용자스레드를 하나의 커널스레드로 매핑
//					   한 스레드가 봉쇄적 시스템콜을 호출해도 다른 스레드들이 실행될 수 있음
//					   단점은 사용자스레드를 생성할 때 마다 그에 대응하는 커널스레드를 생성해야해서 오버헤드로 인해 프로그램 성능을 제한할 수 있음
//					   보통 구현할 때 지원되는 스레드 수를 제한함
//					3) 다 대 다 모델
//					   여러개의 사용자스레드를 그보다 적은 수 혹은 같은 수의 커널스레드로 멀티플렉스시킴
//				3. 비동기식 / 동기식 스레딩
//					1) 비동기식 : 부모가 자식스레드를 생성하고 바로 실행을 재개해서 부모,자식스레드가 병행하게 실행됨
//					2) 동기식 : 부모가 자식스레드를 생성하고 자식스레드가 종료될 때 까지 기다렸다가 실행을 재개함 (fork-join strategy)
//								통상 스레드 간 많은 양의 데이터 공유를 수반함
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Windows Thread
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

DWORD Sum;

DWORD WINAPI Summation(LPVOID Param)
{
	DWORD Upper = *(DWORD *)Param;
	for (DWORD i = 0; i <= Upper; ++i)
		Sum += i;
	return 0;
}

int main(int argc, char** argv)
{
	DWORD ThreadID;
	HANDLE ThreadHandle;
	int Param;

	if (argc != 2)
	{
		fprintf(stderr, "An integer parameter is required\n");
		return -1;
	}
	Param = atoi(argv[1]);
	if (Param < 0)
	{
		fprintf(stderr, "An integer >= 0 is required\n");
		return -1;
	}

	ThreadHandle = CreateThread(
		NULL,		//	default security attributes
		0,			//	default stack size
		Summation,	//	Thread function
		&Param,		//	parameter to thread function
		0,			//	default creation flags
		&ThreadID);	//	returns the thread identifier

	if (ThreadHandle != NULL)
	{
		WaitForSingleObject(ThreadHandle, INFINITE);

		CloseHandle(ThreadHandle);

		printf("Sum = %d\n", Sum);
	}
}