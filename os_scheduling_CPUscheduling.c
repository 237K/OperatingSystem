///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Linux
//	2019. 04. 23
//
//	<CPU Scheduling>
//		1. CPU I/O Burst Cycle
//		2. CPU Scheduler
//		3. ���� �����ٸ� (Preemptive Scheduling)
//		4. Dispatcher
//			1) Context Switch
//			2) User Mode�� ��ȯ
//			3) ����� ���α׷��� �ٽ� �����ϱ� ���� ���α׷��� ������ ��ġ�� ����
//			* dispatch latency : �� ���μ����� �����ϰ� �ٸ� ���μ��� ������ �����ϴµ� �ҿ�Ǵ� �ð�
//		5. Scheduling Criteria
//			Utilization, Throughput, Turnaround time, Waiting time, Response time
//
//		*6. Scheduling Algorithm
//			1) FCFS : First Come First Serve
//				a) FIFO ť�� ���� ����
//				b) ��մ��ð��� �� �� ���� (convey effect)
//
//			2) SJF : Shortest Job First
//				a) ���μ����� ��ü ���̰� �ƴ϶� ���� CPU Burst ���̿� ���� �����ٸ�
//				b) SJF �˰����� ���� ������ CPU Scheduling������ ������ �� ����. ���� CPU Burst���̸� �� ����� ����
//				c) �׷��� SJF �����ٸ��� ������ ���¸� �����. ���� CPU Burst ���̵��� ��������� ���� Burst ������ �ٻ簪���� ���
//			
//			3) Priority Scheduling
//				a) ���� ����(indefinite blocking) -> �������� �ý��ۿ��� ����ϴ� ���μ����� �켱������ ���������� ������Ŵ(Aging)
//						
//			*4) Round-Robin Scheduling (RR)
//				ready queue�� circular queue�� �����, FCFSó�� ť�� ���鼭 �ð��Ҵ緮(time quantum) ��ŭ�� CPU�� �Ҵ�
//				1) CPU Scheduler�� ť�� ù��° ���μ����� �����ϰ� �ð��Ҵ緮 ���Ŀ� ���ͷ�Ʈ�� �ɵ��� Ÿ�̸Ӹ� ����
//				2-1) �ð��Ҵ緮 > ����ƮŸ��
//					���μ��� �Ϸ� �� ���μ����� �ڹ������� CPU�� ������
//				2-2) �ð��Ҵ緮 < ����ƮŸ��
//					a) Ÿ�̸Ӱ� ������ ���ͷ�Ʈ �߻�
//					b) Context Switch
//					c) �����ϴ� ���μ����� ready queue�� ������ ����
//				3) ready queue�� ���� ���μ����� CPU�� ����
//				*RR�����ٸ��� ������ �ð��Ҵ緮�� ������ ������ ����
//				*�ð��Ҵ緮�� �ʹ� ũ�� �׳� FCFS�����ٷ��� �Ȱ���, �ʹ� ������ ���Ʊ�ȯ ����� �ʹ� Ŀ��
//				*�Ϲ������� CPU����Ʈ�� 80%�� �ð��Ҵ緮���� ª�ƾ� ��
//
//			5) �ٴܰ� ť �����ٸ� (Multilevel Queue Scheduling)
//				1) Q�� ������ ���� �������� �켱������ ������
//				2) ready Q���� ���μ����� Ư���� ���� (�Ϲ������� foreground / background ���μ����� ������) Q�� �������
//				3) �ѹ� Q�� �����Ǹ� ����� �� ���� ������ ���� �� ����
//				4) �� Q�� Ư���� ���� RR scheduling�� ä���� ���� �ְ� FCFS�� ä���� ���� ����.
//					(���� foreground�� RR, background�� FCFS�� ä��)
//				5-1) ���� �켱���� ť�� ���� �켱������ ť�� ������� ������ ����� �� ����
//					 ���� �켱���� ť�� ����ǰ� ���� �� ���� �켱���� ť�� �� ���μ����� ������ �� ���μ����� CPU�� ������
//				5-2) ���� �켱���� ť���� CPU�ð��� 80%�� �Ҵ��ϰ�, ���� �켱���� ť���� 20%�� �Ҵ��ϴ� ������ ������ ���� ����
//
//			*6) �ٴܰ� �ǵ�� ť �����ٸ� (Multilevel Feedback Queue Scheduling)
//				1) ���������� �켱������ �ٸ� Q�� ������ ����. Q���� time quantum�� �ٸ���, �˰����� �ٸ�
//				   (0�� Q : RR, quantum = 8ms / 1�� Q : RR, quantum = 16ms / 2�� Q : FCFS)
//				2) ready queue�� �ִ� ���μ����� ������ 0�� ť�� ��
//				3) 0�� ť���� �Ϸ���� ������ 1�� ť�� �����
//				4) 1�� ť������ �Ϸ���� ������ 2�� ť�� �����
//				5) 2�� ť�� 0�� ť�� 1�� ť�� ��� ����� ���� �����
//				6) 2�� ť�� ���� ���� �� 0�� ť�� ���ο� ���μ����� ������ ��� ���ο� ���μ����� CPU ������
//
//		7. Thread Scheduling
//			1) PCS : Process Contention Scope	PTHREAD_SCOPE_PROCESS
//			*2) SCS : System Contention Scope	PTHREAD_SCOPE_SYSTEM
//			Windows, Linux������ �ϴ��ϸ��� ����ϹǷ� SCS�� �����
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

	//	pthread_attr_getscope(pthread_attr_t* attr, int* scope) : ������� ��å ���� get
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

	//	pthread_attr_setscope(pthread_attr_t* attr, int scope) : ������� ��å ���� setting
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	//	thread ����
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_create(tid[i], &attr, runner, NULL);

	for (i = 0; i < NUM_THREADS; ++i)
		pthread_join(tid[i], NULL);
}

void *runner(void *param)
{
	pthread_exit(0);
}