///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Linux
//	2019. 04. 25
//
//	<Process Synchronization>
//		공유데이터에 대한 동시 접근은 데이터의 일관성을 해칠 수 있음
//		논리 주소 공간을 공유하는 협력적 프로세스들에게 규칙을 제공하여 데이터 일관성을 유지할 수 있도록 해야 함
//	
//		* 경쟁 상황 : 여러 개의 프로세스가 동일한 자료에 접근하여 조작하고 그 실행 결과가 접근이 발생한 순서에 의존하는 상황
//		1. 임계구역 문제 (The Critical-Section Problem)
//			* 임계구역 (critical section) : 다른 프로세스와 공유하는 변수를 변경하거나 테이블을 갱신하거나 파일을 쓰는 등의 작업을 수행
//			* 진입구역 (entry section)
//			* 퇴출구역 (exit section)
//			* 나머지구역 (remainder section)
//			임계구역 문제를 해결하기 위해선 반드시 다음의 세 가지 조건을 충족해야 함
//			1) Mutual exclusion : 한 프로세스가 자신의 임계구역에서 실행되면 다른 프로세스들은 그들 자신의 임계구역에서 실행될 수 없음
//			2) Progress : 자기 임계구역에서 실행되는 프로세스가 없고 그들 자신의 임계구역으로 진입하려는 프로세스들이 있다면,
//						  나머지 구역에서 실행 중이지 않은 프로세스들만 누가 그 임계구역으로 진입할 지 결정하는데 참여할 수 있음
//						  * 이 선택은 무한히 연기될 수 없음
//			3) Bounded waiting : 프로세스가 자신의 임계구역에 진입하려는 요청을 한 후부터 그 요청이 허용될 때 까지
//								 다른 프로세스들이 그들 자신의 임계구역에 진입하도록 허용되는 횟수에 한계가 있어야 함
//
//		2) Peterson's Solution
//		3) Synchronization Hardware
//			a) test_and_set()
//			b) compare_and_swap()
//		4) Mutex Locks
//			a) acquire() / release()
//			b) 프로세스가 임계구역에 있는 동안 임계구역에 들어가기 원하는 다른 프로세스들은 acquire() 함수를 호출하는 반복문을 실행
//			   -> spinlock : CPU사이클을 낭비하긴 하지만 context switch는 하지 않아도 된다는 것이 장점
//		5) Semaphores
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Process Synchronization
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Peterson's Solution
do
{
	flag[i] = TRUE;					//	flag[i]가 TRUE이면
	turn = j;						//	반드시 turn은 j여야 함

	//	j프로세스 실행
	while (flag[j] && turn == j);	//	flag[j]가 TRUE이고 j turn일 동안

	critical_section;				//	임계구역 실행

	flag[i] = FALSE;

	remainder_section;

} while (TRUE);

//	Mutex Locks

bool available;

do
{
	//	락 획득
	acquire()
	{
		while (!available);
		available = false;
	}

	critical_section;

	//	락 반환
	release()
	{
		available = true;
	}

	remainder_section;
}