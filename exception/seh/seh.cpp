#include <windows.h>
#include <stdio.h>


/*
|next|
|handle|
|next|
|handle|
|next|
|handle|
*/
typedef struct ExceptionStruct
{
	ExceptionStruct *pNext;
	DWORD handler;
}ExceptionStruct, *PExceptionStruct;


//ExceptionContinueExecution, //继续执行
//ExceptionContinueSearch,    //搜索下一个seh处理函数
//ExceptionNestedException,
//ExceptionCollidedUnwind
EXCEPTION_DISPOSITION ExceptionHandle(PEXCEPTION_RECORD pExceptionRecord, PVOID EstablisherFrame, PCONTEXT pContextRecord, PVOID pDispatcherContext)
{
	if (0xC0000094 == pExceptionRecord->ExceptionCode)
	{
		pContextRecord->Eip += 2;
		return ExceptionContinueExecution;
	}
	return ExceptionContinueSearch;
}


void testException()
{
	DWORD dwOrginSehAddr = 0;
	ExceptionStruct exceptionStruct = { 0 };

	//内存地址不能当做目标地址
	__asm
	{
		mov eax, fs:[0];
		mov dwOrginSehAddr, eax;
	}

	exceptionStruct.pNext = (PExceptionStruct)dwOrginSehAddr;
	exceptionStruct.handler = (DWORD)ExceptionHandle;

	__asm
	{
		lea eax, ds:[exceptionStruct];
		mov fs:[0], eax;
	}

	__asm
	{
		xor eax, eax;
		mov eax, 1;
		xor ecx, ecx;
		div ecx;
	}
	
	printf("1111\n");

	__asm
	{
		mov eax, dwOrginSehAddr;
		mov fs : [0], eax;
	}
}


int main(int argc, char *argv[])
{
	testException();

	system("pause");
	return 0;
}

