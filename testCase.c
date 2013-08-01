/*
 * testCase.c
 *
 *  Created on: Jul 17, 2013
 *      Author: xd
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "mem.h"


typedef void (*fun_ptr)();

extern void quit();
extern void memtest();
extern void memLeaktest();
extern void help();
extern void _showMemRecord();
extern void _showMemErrRecord();
extern void _showMemLeakRecord();
extern void _free();
extern void _malloc();
//extern void showMemAll();

extern void testmemLeak(unsigned int size,unsigned int num, unsigned int  times);
extern void testmem(unsigned int size,unsigned int num,unsigned int times);
extern void showMemRecord(unsigned int blcokInex,unsigned int num);
extern void showMemLeakRecord(unsigned int blcokInex,unsigned int num);

struct sCommand
{
	char* commandStr;
	fun_ptr commandFun;
	char* describe;
};

struct sCommand testCommand[]=
{
	{"q"             ,quit               ,"exit program"},
	{"help"          ,help               ,"help"},
	{"memtest"       ,memtest            ,"normal malloc and free test"},
	{"memLeaktest"   ,memLeaktest        ,"mem leak test"},
	{"malloc"        ,_malloc            ,"malloc mem block"},
	{"free"          ,_free              ,"free mem blocks"},
	{"memAll"        ,showMemAll         ,"show mem pool information"},
	{"memRecord"     ,_showMemRecord     ,"show mem record"},
	{"memErrRecord"  ,_showMemErrRecord  ,"show mem err record"},
	{"memLeakRecord" ,_showMemLeakRecord ,"show mem leak record"},

};

void _showMemRecord()
{
	unsigned int index=0;
	unsigned int num=0;
	printf("please input the block index and the record num\n");
	scanf("%d %d",&index,&num);
	showMemRecord(index,num);
}

void _showMemLeakRecord()
{
	unsigned int index=0;
	unsigned int num=0;
	printf("please input the block index and the record num\n");
	scanf("%d %d",&index,&num);
	showMemLeakRecord(index,num);
}

void _showMemErrRecord()
{
	unsigned int num=0;
	printf("please input num of the err record \n");
	scanf("%d",&num);
	showMemErrRecord(num);

}
void quit()
{
    SYS_MemDestory();
    SYS_MemRecordDestory();
    SYS_MemLeakRecordDestory();
	exit(0);
};

void _free()
{
	long freeMem=0;
	printf("please input the mem you want free\n");
	scanf("%p",&freeMem);
	SYS_MemFree((void*)freeMem,__FILE__,__FUNCTION__,__LINE__);
}

void _malloc()
{
	unsigned int size=0;
	void * addrlog=NULL;
	printf("please input the size of the mem you want malloc\n");
	scanf("%d",&size);
	printf("you input size is %d\n",size);
	addrlog=SYS_MemAllocate(size,__FILE__,__FUNCTION__,__LINE__);
	printf("malloc addr 0x%p\n",addrlog);
}

void _memtest(void *p)
{
    unsigned int* temp=(unsigned int*)p;
    unsigned int size =*temp;
    unsigned int block=*(temp+1);
    unsigned int times=*(temp+2);

    free(p);
    /*printf("you input size:%d,block nums:%d,test times:%d\n",size,block,times);*/
	testmem(size,block,times);
}
void memtest()
{
	printf("please input the size ,the block nums and the test times you want malloc\n");
    unsigned int *temp=(unsigned int*)malloc(3*sizeof(unsigned int));
	scanf("%d %d %d",&temp[0],&temp[1] ,&temp[2]);
	printf("you input size:%d,block nums:%d,test times:%d\n",temp[0],temp[1] ,temp[2]);

	pthread_t t1;
	pthread_create(&t1,NULL,_memtest,(void*)&temp[0]);
	pthread_detach(t1);

//	create_pthread_join();

};

void _memLeaktest(void *p)
{
    unsigned int* temp=(unsigned int*)p;
    unsigned int size =*temp;
    unsigned int block=*(temp+1);
    unsigned int times=*(temp+2);
    free(p);
	/*printf("you input size:%d,block nums:%d,test times:%d\n",size,block,times);*/
	testmemLeak(size,block,times);
}

void memLeaktest()
{
	printf("please input the size ,the block nums and the test times you want malloc\n");
    unsigned int *temp=(unsigned int*)malloc(3*sizeof(unsigned int));
	scanf("%d %d %d",&temp[0],&temp[1] ,&temp[2]);
	printf("you input size:%d,block nums:%d,test times:%d\n",temp[0],temp[1] ,temp[2]);

	pthread_t t1;
    pthread_create(&t1,NULL,_memLeaktest,(void*)temp);
	pthread_detach(t1);

//	create_pthread_join();

}

void help()
{
	int index=0;
	printf("-----------------command list-----------------\n");
	for(index =0 ;index < sizeof(testCommand)/sizeof(struct sCommand) ; index++)
	{
		printf("%-16s : %s\n",testCommand[index].commandStr,testCommand[index].describe);
	}
	printf("----------------------------------------------\n");
}



void runCommand()
{
	    char command[64]={0};
		int index =0;
		printf("--------------------------------------------------------------------------------------------\n");
		help();
		printf("please input the command you want\n");
		while(1)
		{
			scanf("%s",command);
			printf("you input command is :%s\n",command);
			for(index = 0; index < sizeof(testCommand)/sizeof(struct sCommand) ; index++)
			{
				if(strcmp(command,testCommand[index].commandStr)==0)
				{
					testCommand[index].commandFun();
					break;
				}

			}

			if(index == sizeof(testCommand)/sizeof(struct sCommand))
			{
				printf("--------------------------------------------------------------------------------------------\n");
				printf("don't find any command,please type again\n");
			}
			else
			{
				printf("--------------------------------------------------------------------------------------------\n");
				printf("please input the command you want\n");
			}


			memset((void*)command,0,sizeof(command));
		}
}


/**************************************************************************************************************/
/*
 *                          如下为测试代码
 */
/**************************************************************************************************************/

int flag=1;

void* addr;
void alloc1(int size,int num)
{

	int i=0;
		for(i=0;i<num;i++)
		{
		   SYS_MALLOC( addr ,size);
		}
}

void free1(void* addr)
{
	SYS_FREE(addr);
}


void testmem(unsigned int size,unsigned int num,unsigned int times)
{


   	int i=0;
	long * addrlog= NULL;
	int size1= num*sizeof(unsigned long *);
	addrlog=malloc(size1);
 /*       SYS_MALLOC(addrlog,size1);*/


	while(1)
	{


		if(flag==1)
		{

		for(i=0;i<num;i++)
		{
		   SYS_MALLOC( *(addrlog+i) ,size);

		}
		for(i=0;i<num;i++)
		{
			SYS_FREE((void *)*(addrlog+i));
		}

#if 0
		for(i=0;i<num;i++)
		{
			SYS_FREE((void *)*(addrlog+i));
		}
		for(i=0;i<num;i++)
		{
			SYS_FREE((void *)*(addrlog+i));
		}
#endif

		}
		/*taskDelay(1);*/
	}


}


void testmemLeak(unsigned int size,unsigned int num, unsigned int  times)
{
	unsigned int * addrLog= NULL;
	int i=0;
	int flag=0;
	printf("testmemLeak \n");
	while(1)
	{
		if(flag==0)
		{
		for(i=0;i<num;i++)
		{
			SYS_MALLOC(addrLog,size);
			printf("size %d addrlog %p\n",size,addrLog);
		}
		}
		flag=1;
	}
}

void testmem1(int size,int num)
{
	unsigned int * addrLog= NULL;
	while(1)
	{
		if(flag==1)
		{
		SYS_MALLOC(addrLog,size);
		SYS_FREE(addrLog);

		}
	}
}


