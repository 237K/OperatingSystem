///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	[Operating System]
//
//	OS Linux
//	2019. 04. 21
//
//	<Process>
//		1. IPC : Interprocess Communication
//			1) Shared memory : 최초 공유메모리를 생성할 때만 system call을 하므로 빠를 것 같지만, 캐쉬일관성 문제 때문에 성능 저하
//			2) Message Passing : 시스템의 코어 수가 많을 수록 메시지파싱 방법이 빠름
//				a) 직접 / 간접통신 : 간접통신에서는 mailbox나 port를 통해 메세지를 송수신함
//				b) 동기식 / 비동기식 통신 : 동기식(blocking)에서는 송수신 완료할 때까지 기다림
//				c) 자동 / 명시적 버퍼링 : 기본적으로 메세지는 임시 큐에 들어감. 큐는 무용량 / 유한용량 / 무한용량이 있음
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//	공유메모리를 위한 POSIX API
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
	const int SIZE = 4096;					//	공유메모리 객체 크기 (바이트 단위)
	const char *name = "OS";				//	공유메모리 객체 이름
	const char *message_0 = "Hello";		//	공유메모리에 쓰일 문자열
	const char *message_1 = "World!";

	int shm_fd;		//	공유메모리 파일 기술자
	void *ptr;		//	공유메모리 객체를 가리키는 포인터

	//	공유메모리 객체 생성
	//	shm_open(공유메모리 객체 이름, 객체가 존재하지 않으면 생성(O_CREAT) | 객체가 읽기와 쓰기가 가능한 상태로 열림(O_RDRW), 디렉터리 허가권 부여)
	shm_fd = shm_open(name, O_CREAT | O_RDRW, 0666);	

	//	객체의 크기를 바이트 단위로 설정
	ftruncate(shm_fd, SIZE);

	//	공유메모리 객체를 메모리에 매핑
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	//	공유메모리 객체에 쓰기
	sprintf(ptr, "%s", message_0);
	ptr += strlen(message_0);
	sprintf(ptr, "%s", message_1);
	ptr += strlen(message_1);

	return 0;
}