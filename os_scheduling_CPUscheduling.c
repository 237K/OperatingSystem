///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Linux
//	2019. 04. 23
//
//	<CPU Scheduling>
//		1. CPU I/O Burst Cycle
//		2. CPU Scheduler
//		3. 선점 스케줄링 (Preemptive Scheduling)
//		4. Dispatcher
//			1) Context Switch
//			2) User Mode로 전환
//			3) 사용자 프로그램을 다시 시작하기 위해 프로그램의 적절한 위치로 점프
//			* dispatch latency : 한 프로세스를 정지하고 다른 프로세스 수행을 시작하는데 소요되는 시간
//		5. Scheduling Criteria
//			Utilization, Throughput, Turnaround time, Waiting time, Response time
//
//		*6. Scheduling Algorithm
//			1) FCFS : First Come First Serve
//				a) FIFO 큐로 쉽게 구현
//				b) 평균대기시간이 길 수 있음 (convey effect)
//
//			2) SJF : Shortest Job First
//				a) 프로세스의 전체 길이가 아니라 다음 CPU Burst 길이에 의해 스케줄링
//				b) SJF 알고리즘이 좋긴 하지만 CPU Scheduling에서는 구현할 수 없음. 다음 CPU Burst길이를 알 방법이 없음
//				c) 그래서 SJF 스케줄링과 유사한 형태를 사용함. 이전 CPU Burst 길이들의 지수평균을 다음 Burst 길이의 근사값으로 사용
//			
//			3) Priority Scheduling
//				a) 무한 봉쇄(indefinite blocking) -> 오랫동안 시스템에서 대기하는 프로세스의 우선순위를 점진적으로 증가시킴(Aging)
//						
//			*4) Round-Robin Scheduling (RR)
//				ready queue를 circular queue로 만들고, FCFS처럼 큐를 돌면서 시간할당량(time quantum) 만큼씩 CPU를 할당
//				1) CPU Scheduler는 큐의 첫번째 프로세스를 선택하고 시간할당량 이후에 인터럽트를 걸도록 타이머를 맞춤
//				2-1) 시간할당량 > 버스트타임
//					프로세스 완료 후 프로세스가 자발적으로 CPU를 방출함
//				2-2) 시간할당량 < 버스트타임
//					a) 타이머가 끝나고 인터럽트 발생
//					b) Context Switch
//					c) 실행하던 프로세스는 ready queue의 꼬리에 넣음
//				3) ready queue의 다음 프로세스가 CPU를 선점
//				*RR스케줄링의 성능은 시간할당량에 지대한 영향을 받음
//				*시간할당량이 너무 크면 그냥 FCFS스케줄러랑 똑같고, 너무 작으면 문맥교환 비용이 너무 커짐
//				*일반적으로 CPU버스트의 80%는 시간할당량보다 짧아야 함
//
//			5) 다단계 큐 스케줄링 (Multilevel Queue Scheduling)
//				1) Q를 여러개 만들어서 절대적인 우선순위를 결정함
//				2) ready Q에서 프로세스의 특성에 따라 (일반적으로 foreground / background 프로세스로 구분함) Q에 집어넣음
//				3) 한번 Q가 결정되면 종료될 때 까지 영원히 떠날 수 없음
//				4) 각 Q는 특성에 따라 RR scheduling을 채택할 수도 있고 FCFS를 채택할 수도 있음.
//					(보통 foreground는 RR, background는 FCFS를 채택)
//				5-1) 낮은 우선순위 큐는 높은 우선순위의 큐가 비어있지 않으면 실행될 수 없음
//					 낮은 우선순위 큐가 실행되고 있을 때 높은 우선순위 큐에 새 프로세스가 들어오면 새 프로세스가 CPU를 선점함
//				5-2) 높은 우선순위 큐에는 CPU시간의 80%를 할당하고, 낮은 우선순위 큐에는 20%를 할당하는 식으로 구현할 수도 있음
//
//			*6) 다단계 피드백 큐 스케줄링 (Multilevel Feedback Queue Scheduling)
//				1) 마찬가지로 우선순위가 다른 Q가 여러개 있음. Q마다 time quantum이 다르고, 알고리즘이 다름
//				   (0번 Q : RR, quantum = 8ms / 1번 Q : RR, quantum = 16ms / 2번 Q : FCFS)
//				2) ready queue에 있던 프로세스는 무조건 0번 큐로 들어감
//				3) 0번 큐에서 완료되지 않으면 1번 큐로 강등됨
//				4) 1번 큐에서도 완료되지 않으면 2번 큐로 강등됨
//				5) 2번 큐는 0번 큐와 1번 큐가 모두 비었을 때만 실행됨
//				6) 2번 큐가 실행 중일 때 0번 큐로 새로운 프로세스가 들어오면 즉시 새로운 프로세스가 CPU 선점함
//
//		7. Thread Scheduling
//			1) PCS : Process Contention Scope	PTHREAD_SCOPE_PROCESS
//			*2) SCS : System Contention Scope	PTHREAD_SCOPE_SYSTEM
//			Windows, Linux에서는 일대일모델을 사용하므로 SCS만 사용함
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Pthreads Scheduling API
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

void *runner(void *param);

int main(int argc, char** argv)
{
	int i, scope;
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;

	//	pthread_attr_getscope(pthread_attr_t* attr, int* scope) : 경쟁범위 정책 정보 get
	if (pthread_attr_getscope(&attr, &scope) != 0)
	{
		fprintf(stderr, "Unable to get scheduling scope\n");
	}
	else
	{
		if (scope == PTHREAD_SCOPE_PROCESS)
			printf("PTHREAD_SCOPE_PROCESS");
		else if (scope == PTHREAD_SCOPE_SYSTEM)
			printf("PTHREAD_SCOPE_SYSTEM");
		else
			fprintf(stderr, "Illegal scope value\n");
	}

	//	pthread_attr_setscope(pthread_attr_t* attr, int scope) : 경쟁범위 정책 정보 setting
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	//	thread 생성
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_create(tid[i], &attr, runner, NULL);

	for (i = 0; i < NUM_THREADS; ++i)
		pthread_join(tid[i], NULL);
}

void *runner(void *param)
{
	pthread_exit(0);
}