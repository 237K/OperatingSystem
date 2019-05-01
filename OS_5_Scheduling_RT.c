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
//					1) SMP �ý��ۿ����� �����ٸ� �̽�
//						a) ó���� ģȭ�� (Processor Affinity)
//						   ���μ����� �ٸ� ó����� �����Ϸ��� ĳ�� ������ ��ȿȭ�ϰ� �ٽ� ä���� �ϴµ�, �� ����� ���� ��� ������,
//						   ���� ó���⿡�� ���μ����� �����Ű���� ��
//						b) Load Balancing
//							- push ���� : 'Ư�� �½�ũ'�� �ֱ������� �� ó������ ���ϸ� �˻�, �������� ó���⿡�� ���ٻ� ó����� push
//							- pull ���� : '���޻����� ó����'�� �ٻ� ó���⸦ ��ٸ��� �ִ� ���μ����� pull
//					**Load Balancing�� processor affinity�� ���Ͱ� �����
//					  ���� ó���⿡�� ���μ����� �����ų �� ������ �ش� ó���� ĳ�ÿ� �����ϴ� �����͸� Ȱ���ϴ� ���ε�,
//					  pushing, pulling �����ν� �� ������ ���ְ� ��
//
//					2) �����ھ� ���μ��� (Multicore Processor)
//						- ���μ����� �޸𸮿� ������ �� �����Ͱ� ���������� ��ٸ��鼭 �ð� ��� (Memory Stall)
//						- �� �ھ �� �̻��� �ϵ���� �����尡 �Ҵ�� �� �ִ� '���߽����� ���μ��� �ھ�' ����
//						- ���߽����� ���μ��� �ھ���� �� �ܰ��� �����ٸ� �ʿ�
//							1) �� �ϵ���� �����忡�� ��� ����Ʈ���� �����尡 ����Ǿ�� �ϴ��� ����
//							2) �� �ھ � �ϵ���� �����带 �����ų�� ����
//		
//				2. Realtime CPU Scheduling
//					- soft realtime system
//					- hard realtime system
//					1) �����ð� �ּ�ȭ
//						a) ���ͷ�Ʈ ����
//							- ���ͷ�Ʈ �Ҵ� �ð� �ּ�ȭ
//						b) ����ġ ����
//							- ������ Ŀ�� ���
//					2) �켱���� ��� �����ٸ� : �������� (admission control) �˰����� �߰��ؾ� ��
//					3) Rate-Monotonic �����ٸ�
//					4) Earliest Deadline First : EDF
//					5) Proportionate Share �����ٸ�
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

	//	������ ����
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_create(&tid[i], &attr, runner, NULL);

	for (i = 0; i < NUM_THREADS; ++i)
		pthread_join(tid[i], NULL);
}

void *runner(void *param)
{
	pthread_exit(0);
}