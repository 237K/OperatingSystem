////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	OS Linux
//	2019. 04. 20
//	
//	[ Operating System ]
//		<3. 프로세스>
//			(1. 프로세스 개념)
//
//				1. Process concept
//				2. Process state
//			   *3. Process Control Block (PCB)
//					- Linux의 PCB는 C 구조체 task_struct로 표현됨
//					- header file : linux/sched.h
//					- 프로세스의 상태, 스케줄링, 메모리 관리 정보, 열린 파일, 부모 프로세스와 자식, 형제 프로세스의 리스트를 가리키는 포인터 등의 정보를 가짐
//				4. Threads
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <linux/sched.h>

long state;					//	프로세스 상태
struct sched_entity se;		//	스케줄링 정보
struct task_struct *parent;	//	부모 프로세스
struct list_head children;	//	자식 프로세스
struct files_struct *files;	//	오픈 파일 리스트
struct mm_struct *mm;		//	이 프로세스의 주소공간

//	Linux 커널 안에서 모든 활성 프로세스들은 task_struct의 양방향 연결 리스트로 표현됨
//	커널은 현재 실행 중인 프로세스를 가리키는 포인터를 유지

//	커널이 현재 실행 중인 프로세스의 상태를 new_state로 바꾸려면
current->state = new_state;