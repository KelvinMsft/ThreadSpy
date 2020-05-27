
#include <ntddk.h>  
#include "x86.h"
#include <intrin.h>
extern "C" 
{
	//-----------------------------------------------------------------------------//
	void* UtilGetSystemProcAddress(
		_In_ const wchar_t *proc_name
	) 
	{
		PAGED_CODE(); 
		UNICODE_STRING proc_name_U = {};
		RtlInitUnicodeString(&proc_name_U, proc_name);
		return MmGetSystemRoutineAddress(&proc_name_U);
	}
	 

	//-----------------------------------------------------------------------------//
	NTSTATUS UtilForEachProcessor(
		_In_ NTSTATUS(*callback_routine)(void *),
		_In_ void *context)
	{
		const auto number_of_processors =
			KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
		for (ULONG processor_index = 0; processor_index < number_of_processors; processor_index++)
		{
			PROCESSOR_NUMBER processor_number = {};
			auto status = KeGetProcessorNumberFromIndex(processor_index, &processor_number);
			if (!NT_SUCCESS(status))
			{
				return status;
			}

			// Switch the current processor
			GROUP_AFFINITY affinity = {};
			affinity.Group = processor_number.Group;
			affinity.Mask = 1ull << processor_number.Number;
			GROUP_AFFINITY previous_affinity = {};
			KeSetSystemGroupAffinityThread(&affinity, &previous_affinity);

			// Execute callback
			status = callback_routine(context);

			KeRevertToUserGroupAffinityThread(&previous_affinity);
			if (!NT_SUCCESS(status))
			{
				return status;
			}
		}
		return STATUS_SUCCESS;
	}

	//-----------------------------------------------------------------------------//
	NTSTATUS UtilWriteMsr(Msr msr, ULONG64 Value)
	{
		NTSTATUS status = STATUS_SUCCESS;
		__try
		{
			__writemsr(static_cast<ULONG>(msr),Value);
		}
		__except (0)
		{
			status = STATUS_UNSUCCESSFUL;
		}
		return status;
	}
	
	//-----------------------------------------------------------------------------//
	NTSTATUS UtilReadMsr(Msr msr, ULONG64* Value)
	{
		NTSTATUS status = STATUS_SUCCESS;
		if (!Value)
		{
			status = STATUS_INVALID_PARAMETER;
			return status;
		}
		__try
		{	
			*Value = __readmsr(static_cast<ULONG>(msr));
		}
		__except (0) 
		{
			status = STATUS_UNSUCCESSFUL;
		}
		return status;
	}
	//-----------------------------------------------------------------------------//
}