///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	OS Linux
//	2019. 05. 05
//
//	[Operating System]
//		<7. 데드락>
//			1. System model
//				- 한 프로세스 집합 내의 모든 프로세스가 그 집합 내의 다른 프로세스에 의해서만 발생될 수 있는 사건을 기다린다면,
//				  그 프로세스 집합은 데드락 상태에 있음
//			2. 데드락의 특징
//				1) 데드락이 발생하는데 반드시 필요한 조건
//					a) Mutual exclusion
//					b) Hold-and-wait
//					c) No preemption
//					d) Circular wait
//				2) 자원 할당 그래프 (Resource-Allocation Graph)
//					- 그래프가 사이클을 포함하지 않으면 시스템 내 어떤 프로세스도 데드락이 아님
//					- 사이클을 포함하면 데드락이 존재할 수 있음
//				3) 데드락 처리 방법
//					a) 데드락 예방
//					b) 데드락 회피
//					c) 데드락을 허용한 다음 탐지하고 회복
//					d) 비용 관점에서 생각해봤을 때 무시해도 괜찮음
//				4) 데드락 예방
//					a) 상호배제 조건을 이용
//						- 공유 가능한 자원들(ex 읽기전용 파일)은 mutual exclusive한 접근을 요구하지 않으며, 따라서 데드락이 발생할 수 없음
//						- 하지만 일반적으로 상호 배제 조건을 거부함으로써 데드락을 예방하는 것은 불가능
//						  근본적으로 공유가 불가능한 자원들이 있기 때문(mutex lock은 동시에 여러 프로세스가 공유할 수 없음)
//					b) 점유하며 대기(Hold and wait) 조건을 이용
//						- 프로세스가 자원을 요청할 때는 다른 자원들을 점유하지 않을 것을 보장해야 함
//						a) 각 프로세스가 실행되기 전에 자신의 모든 자원을 요청하고 할당받을 것을 요구함
//						b) 프로세스가 자원을 소유하고 있지 않을 때만 자원을 요청할 수 있도록 허용
//						   프로세스가 추가 자원을 요청하려면 자신에게 할당된 모든 자원을 먼저 방출하도록 함
//						-> 당연히 기아 상태가 발생할 수 있음
//					c) 비선점(No preemption) 조건 이용					
//						- 이미 할당된 자원이 선점되지 않아야 함
//						a) 어떤 자원을 점유하고 있는 프로세스가 즉시 할당할 수 없는 다른 자원을 요청하면, 현재 점유하고 있는 모든 자원들이 선점됨
//						   즉 이들 자원들이 묵시적으로 방출됨
//						b) 한 프로세스가 어떤 자원들을 요청하면 이들이 사용 가능한지 검사
//					d) 순환대기(Circular wait) 조건 이용
//						a) 모든 자원 타입에 순서를 부여하여 각 프로세스가 열거된 순서대로 오름차순으로 자원을 요청하도록 요구
//						b) 순서를 지키는 프로그램을 작성하는 것은 application 프로그래머에게 달려 있음
//				5) 데드락 회피
//					- 각 프로세스의 요청과 방출에 대한 완전한 순서를 파악하고 있다면, 각 요청에 대해 가능한 미래의 교착상태를 피하기 위해
//					  프로세스가 대기해야 하는지 여부를 결정할 수 있음
//					a) 안전상태
//						- 시스템이 어떤 순서로든 프로세스들이 요청하는 모든 자원을 데드락을 야기하지 않고 모두 할당해 줄 수 있는 상태
//					b) 자원 할당 그래프 알고리즘
//						- 자원 할당 그래프에 예약 간선을 추가
//						- 사이클 탐지 알고리즘으로 안정성 검사(O(n^2))
//					c) 은행원 알고리즘
//						- 프로세스가 시작할 때 프로세스가 가지고 있어야 할 자원의 최대개수를 자원 종류마다 미리 신고
//						1) 안정성 알고리즘
//						2) 자원 요청 알고리즘
//				6) 데드락 탐지
//				7) 데드락 회복
//					a) 프로세스 종료
//					b) 자원 선점
//					
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Mutex Lock 사용 시의 Deadlock
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	뮤텍스락을 생성하고
pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

//	초기화
pthread_mutex_init(&first_mutex, NULL);
pthread_mutex_init(&second_mutex, NULL);

//	first thread 실행 함수
//	first_mutex -> second_mutex 순으로 mutex lock 획득 시도
//	first thread가 first_mutex를 획득하고, second thread가 second_mutex를 획득하면 데드락 발생 가능
void *do_work_one(void *param)
{
	pthread_mutex_lock(&first_mutex);
	pthread_mutex_lock(&second_mutex);

	pthread_mutex_unlock(&second_mutex);
	pthread_mutex_unlock(&first_mutex);

	pthread_exit(0);
}

//	second thread 실행 함수
//	second_mutex -> first_mutex 순으로 mutex lock 획득 시도
void *do_work_two(void *param)
{
	pthread_mutex_lock(&second_mutex);
	pthread_mutex_lcok(&first_mutex);

	pthread_mutex_unlock(&first_mutex);
	pthread_mutex_unlock(&second_mutex);

	pthread_exit(0);
}