///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	OS Linux
//	2019. 04. 24
//
//	[Operating System]
//		<5. CPU Scheduling>
//			(2. RealTime CPU Scheduling)
//
//				1. Multi-Processor Scheduling
//					1) SMP 시스템에서의 스케줄링 이슈
//						a) 처리기 친화성 (Processor Affinity)
//						   프로세스가 다른 처리기로 이주하려면 캐시 내용을 무효화하고 다시 채워야 하는데, 이 비용이 많이 들기 때문에,
//						   같은 처리기에서 프로세스를 실행시키려고 함
//						b) Load Balancing
//							- push 이주 : '특정 태스크'가 주기적으로 각 처리기의 부하를 검사, 과부하인 처리기에서 덜바쁜 처리기로 push
//							- pull 이주 : '유휴상태의 처리기'가 바쁜 처리기를 기다리고 있는 프로세스를 pull
//					**Load Balancing은 processor affinity의 이익과 상충됨
//					  같은 처리기에서 프로세스를 실행시킬 때 이점은 해당 처리기 캐시에 존재하는 데이터를 활용하는 것인데,
//					  pushing, pulling 함으로써 그 이점을 없애게 됨
//
//					2) 다중코어 프로세서 (Multicore Processor)
//						- 프로세서가 메모리에 접근할 때 데이터가 가용해지길 기다리면서 시간 허비 (Memory Stall)
//						- 각 코어에 둘 이상의 하드웨어 스레드가 할당될 수 있는 '다중스레드 프로세서 코어' 구현
//						- 다중스레드 프로세서 코어에서는 두 단계의 스케줄링 필요
//							1) 각 하드웨어 스레드에서 어느 소프트웨어 스레드가 실행되어야 하는지 결정
//							2) 각 코어가 어떤 하드웨어 스레드를 실행시킬지 결정
//		
//				2. Realtime CPU Scheduling
//					- soft realtime system
//					- hard realtime system
//					1) 지연시간 최소화
//						a) 인터럽트 지연
//							- 인터럽트 불능 시간 최소화
//						b) 디스패치 지연
//							- 선점형 커널 사용
//					2) 우선순위 기반 스케줄링 : 승인제어 (admission control) 알고리즘을 추가해야 함
//					3) Rate-Monotonic 스케줄링
//					4) Earliest Deadline First : EDF
//					5) Proportionate Share 스케줄링
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	POSIX RealTime Scheduling API
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

void *runner(void *param);

int main(int argc, char** argv)
{
	int i, policy;
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;

	//	get defalut attributes
	pthread_attr_init(&attr);

	//	get current scheduling policy
	if (pthread_attr_getschedpolicy(&attr, &policy) != 0)
		fprintf(stderr, "Unable to get policy\n");
	else
	{
		if (policy == SCHED_OTHER)
			pirntf("SHCED_OTHER\n");
		else if (policy == SCHED_RR)
			printf("SCHED_RR\n");
		else if (policy == SCHED_FIFO)
			printf("SCHED_FIFO\n");
	}

	//	set scheduling policy
	if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0)
		fprintf(stderr, "Unable to set scheduling policy to SCHED_OTHER\n");

	//	스레드 생성
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_create(&tid[i], &attr, runner, NULL);

	for (i = 0; i < NUM_THREADS; ++i)
		pthread_join(tid[i], NULL);
}

void *runner(void *param)
{
	pthread_exit(0);
}