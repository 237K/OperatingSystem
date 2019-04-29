///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	OS Windows
//	2019. 04. 22
//
//	[Operating System]
//		<4. ������>
//			(1. �����ھ� ���α׷���
//			 2. ���� ������ ��
//			 3. ������ ���̺귯��
//			 4. �Ϲ��� ������
//			 5. ������ ���� �̽�
//			 6. ���)
//
//				1. ���� ������ ���α׷����� ����
//					1) ���伺(Responsiveness)
//					   �ð��� ���� �ɸ��� ������ ������ �����忡�� ����ǵ��� �����ν� ����ڿ� ���� ���伺 ����
//					2) �ڿ� ����(Resource Sharing)
//			   		   ���μ����� �޸� ��������� �ڵ����� ���μ����� �ڿ��� ������
//					3) ������
//					   �翬�� ���ο� ���μ����� ����� �ͺ��� �޸𸮳� ������带 �ξ� ���� ����
//					4) Scalability
//				   	   Multiprocessor �������� �� �����尡 �ٸ� ó���⿡�� ���ķ� ����� �� ����
//				2. ���� ������ ��
//					1) �� �� �� �� : ����ó���ھ���� ������ �츱 �� ��� �� �Ⱦ�
//					2) �� �� �� ��
//					   �� ����ڽ����带 �ϳ��� Ŀ�ν������ ����
//					   �� �����尡 ������ �ý������� ȣ���ص� �ٸ� ��������� ����� �� ����
//					   ������ ����ڽ����带 ������ �� ���� �׿� �����ϴ� Ŀ�ν����带 �����ؾ��ؼ� �������� ���� ���α׷� ������ ������ �� ����
//					   ���� ������ �� �����Ǵ� ������ ���� ������
//					3) �� �� �� ��
//					   �������� ����ڽ����带 �׺��� ���� �� Ȥ�� ���� ���� Ŀ�ν������ ��Ƽ�÷�����Ŵ
//				3. �񵿱�� / ����� ������
//					1) �񵿱�� : �θ� �ڽĽ����带 �����ϰ� �ٷ� ������ �簳�ؼ� �θ�,�ڽĽ����尡 �����ϰ� �����
//					2) ����� : �θ� �ڽĽ����带 �����ϰ� �ڽĽ����尡 ����� �� ���� ��ٷȴٰ� ������ �簳�� (fork-join strategy)
//								��� ������ �� ���� ���� ������ ������ ������
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