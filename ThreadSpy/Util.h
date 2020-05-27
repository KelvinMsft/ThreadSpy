#pragma once
#include<ntddk.h>
#include "x86.h"
extern "C" { 
	void*	 UtilGetSystemProcAddress(
		_In_ const wchar_t *proc_name
	);
	
	NTSTATUS UtilForEachProcessor(
		_In_ NTSTATUS(*callback_routine)(void *), 
		_In_ void *context
	);

	NTSTATUS UtilReadMsr(
		Msr msr, 
		ULONG64* Value
	);

	NTSTATUS UtilWriteMsr(
		Msr msr,
		ULONG64 Value
	);
}