/*
 * memory.h
 *
 *  Created on: 2013-6-6
 *      Author: xd
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#define __POSIX__
#ifdef  __POSIX__
#include <pthread.h>
#endif
#define __MINGW__
#define __MEM_DEBUG__

enum memErrCode{
	SETUP_ERR=0,
	MEM_ERR,
	MEM_OK,
	ALREADY_INSTALL,
	UNINSTALL,
	IN_INT,
	SIZE_TOO_BIG,
	NO_ENOUGH_MEMORY,
	ALREADY_MALLOC,
	IMPOSSIBLE_ERR,
	MEM_REGION_CONFLICT,
	NOT_RIGHT_REGION_MEMORY,
	CONTROL_BLOCK_LENGTH_ERR,
	STATE_NOT_EXCEPT,
	CTRL_BLOCK_BREAK,
	IN_CHECK_STATUS,
	IN_SHOW_STATUS,
	CONFIG_ERR,
	ERR_MAX,
};

struct errCodeStr
{
	enum memErrCode err;
	char* str;
};

enum memState {
	INIT_STATE = 0,
	FREE_STATE,
	MALLOC_STATE,
	REMALLOC_STATE,
	BAD_STATE,
	STATE_MAX,
};

struct memStateStr
{
	enum memState state;
	char* str;
};

enum memAction {
	FREE_ACTION=0,
	MALLOC_ACTION,
	ACTION_MAX,
};

struct memActionStr
{
	enum memAction state;
	char* str;
};

enum memPartition {
	BLOCK_64=0,
	BLOCK_128,
	BLOCK_256,
	BLOCK_512,
	BLOCK_1024,
	BLOCK_2048,
	BLOCK_4096,
	BLOCK_MAX,
};



struct memBlock {
	enum memPartition partition;           /* Partition ID��       */
	struct memBlock  *pNext;               /* ָ����һ���ڵ�*/
	unsigned int index;                    /* �ڴ����*/
	enum memState state;                   /* ָʾ�ڴ�״̬*/
	unsigned int taskId;                   /* ��ǰʹ���ڴ������id*/
	unsigned int length;                   /* ����ĳ���*/
	void *pMemory;                         /* ʵ���ڴ��ַ*/
};

struct memControlBlock {
	enum memPartition partition;   /* Partition ID��       */
	unsigned int totalBlocks;      /* ��Partition���Ŀ����*/
	unsigned int size;             /* ÿ����Ĵ�С         */
	unsigned int totalSize;        /* ���������С*/
	unsigned int usedBlocks;       /* ��ʹ�õĿ���*/
	unsigned int freeBlocks;       /* δʹ�õĿ���*/
	unsigned int badBlocks;        /* �ѷ��ֵ��𻵵��ڴ�������*/
	unsigned int reReleaseBlocks;  /* �ظ��ͷŵĿ�*/
	unsigned int unUsedBlocks;     /* ��δʹ�õĿ���*/
	unsigned int usedTimes;        /* �������*/
	unsigned int freeTimes;        /* �ͷŴ���*/
	struct memBlock *pHead;        /* ָ�򱾴�С�ڴ����ͷ */
	struct memBlock *pTail;        /* ָ�򱾴�С�ڴ����β */
	void *startArea;               /* ��ʼ�ڴ���*/
	void *endArea;                 /* �����ڴ���*/
	unsigned int *pBlockState;     /* ��¼�ڴ��״̬*/
};

#define FILE_NAME_LENGTH 256
#define FUNCTION_NAME_LENGTH 32

struct memRecord
{
	unsigned int taskId;                  /* ʹ���ڴ������id*/
	char file[FILE_NAME_LENGTH];          /* ʹ���ڴ���ļ���*/
	char function[FUNCTION_NAME_LENGTH];  /* ʹ���ڴ��������*/
    unsigned int line;                    /* ʹ���ڴ�ĺ���*/
    enum memAction action;                /* ʹ���ڴ�Ķ���*/
    enum memState state;                  /* ʹ���ڴ�Ķ������״̬*/
    enum memErrCode errcode;              /* ʹ���ڴ�Ĵ���*/
    /*unsigned int index;*/               /* �ڴ����*/
    struct memBlock *pMemBlock;           /* ��ʹ�õ��ڴ��*/
    struct memRecord *pNext;              /* ������¼δ���ͷ� ���ڴ�*/
};


struct memRecordlCtrlBlock{
	enum memPartition partition; /* Partition ID��       */
	unsigned int totalBlocks;    /* Ԥ�ȷ���ļ�¼����*/
	unsigned int recordnum;      /* ��ǰ��¼�Ŀ���*/
	struct memRecord *pHead;     /* ��¼ͷ*/
	struct memRecord *pTail;     /* ��¼β*/
	struct memRecord *pCurrent;  /* ��ǰ��¼�Ľڵ�*/
};

struct memLeakRecord
{
	unsigned int taskId;                  /* ʹ���ڴ������id*/
	char file[FILE_NAME_LENGTH];          /* ʹ���ڴ���ļ���*/
	char function[FUNCTION_NAME_LENGTH];  /* ʹ���ڴ��������*/
	unsigned int line;                    /* ʹ���ڴ�ĺ���*/
	struct memBlock *pMemBlock;           /* ��ʹ�õ��ڴ��*/
	struct memLeakRecord *pNext;              /* ������¼δ���ͷ� ���ڴ�*/
	struct memLeakRecord *pPrev;              /* ������¼δ���ͷ� ���ڴ�*/
};

struct memLeakRecordCtrlBlock
{
	enum memPartition partition; /* Partition ID��       */
	unsigned int totalBlocks;    /* Ԥ�ȷ���ļ�¼����*/
	unsigned int recordnum;      /* ��ǰ��¼�Ŀ���*/
	struct memLeakRecord *pStart;    /* ��¼��ʼ������*/
	struct memLeakRecord *pHead;     /* ��¼ͷ*/
	struct memLeakRecord *pTail;     /* ��¼ͷ*/
};


int SYS_MemSetup(void);
int SYS_MemRecordSetup(void);
void SYS_MemInit(void);

void *SYS_MemAllocate(unsigned int size,char * fileName, char* funName,unsigned codeLine);
void SYS_MemFree(void *addr,char * fileName, char* funName,unsigned codeLine);

void showMemAll();
void showMemRecord(unsigned int blockNum,unsigned int num); /*��ʾ�ڴ�����ļ�¼*/
void showMemErrRecord(unsigned int num);
void showMemLeakRecord(unsigned int blcokInex,unsigned int num);

int getIndex(unsigned int size);



#ifdef __VXWORKS__
	extern void intUnlock(int lockKey);
	extern int intLock(void);
	extern int taskIdSelf(void);
	#define TASK_ID_SELF    taskIdSelf

	#define MEM_LOCK_DEFINE
	#define MEM_LOCK_INIT()
	#define MEM_LOCK_START() int key
	#define MEM_LOCK()    key=intLock()
	#define MEM_UNLOCK()  inUnLock(key)

	extern int logMsg (char *fmt, int arg1, int arg2,int arg3, int arg4, int arg5, int arg6);
	#define IN_INT_CONTEXT() intContext()
    #ifdef __MEM_DEBUG__
        #define MEM_DEBUG logMsg
	#endif
#endif



#ifdef __POSIX__

	#define TASK_ID_TYPE    unsigned long int
	#ifdef __MINGW__
		#define TASK_ID_SELF()    \
		({\
			pthread_t temp=pthread_self();\
			temp.p;\
		})
	#else
		#define TASK_ID_SELF    pthread_self
	#endif
	#define MEM_LOCK_DEFINE pthread_mutex_t memMutex
	#define MEM_LOCK_INIT() pthread_mutex_init(&memMutex,NULL)
	#define MEM_LOCK_START()
	#define MEM_LOCK()    pthread_mutex_lock(&memMutex)
	#define MEM_UNLOCK()  pthread_mutex_unlock(&memMutex)

	#ifdef __MEM_DEBUG__
		#define MEM_DEBUG printf
	#endif

	#define IN_INT_CONTEXT() 0
	#define TASK_DELAY

#endif



#define SYS_MALLOC(mallocMem,size)\
{\
	if(IN_INT_CONTEXT() )\
	{\
		MEM_DEBUG("can't alloc mem in int context\n",0,0,0,0,0,0);\
		RECORD_MEM_ERR(MALLOC_ACTION,NULL,STATE_MAX,IN_INT,0,__FILE__,__FUNCTION__,__LINE__);\
		mallocMem=NULL;\
	}\
	else\
	{\
		extern memInstallFlag;\
		while(memInstallFlag==IN_CHECK_STATUS)\
		{\
			/*������Ҫ����ģ��ע�ᣬ˵��ϵͳ��������*/\
			TASK_DELAY(1);\
		}\
		mallocMem=SYS_MemAllocate(size,__FILE__,__FUNCTION__,__LINE__);\
	}\
}

#define SYS_FREE(freeMem)\
{\
	if(IN_INT_CONTEXT() )\
	{\
		MEM_DEBUG("can't free mem in int context\n",0,0,0,0,0,0);\
		RECORD_MEM_ERR(MALLOC_ACTION,freeMem,STATE_MAX,IN_INT,0,__FILE__,__FUNCTION__,__LINE__);\
	}\
	else\
	{\
		extern memInstallFlag;\
		while(memInstallFlag==IN_CHECK_STATUS)\
		{\
			/*������Ҫ����ģ��ע�ᣬ˵��ϵͳ��������*/\
			TASK_DELAY(1);\
		}\
		SYS_MemFree(freeMem,__FILE__,__FUNCTION__,__LINE__);\
	}\
}

#endif /* MEMORY_H_ */
