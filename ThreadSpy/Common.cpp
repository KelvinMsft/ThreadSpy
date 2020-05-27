#include <ntddk.h>
#include "Common.h"
#include "Log.h"
extern "C"
{ 
	NTSTATUS UtilSleep(LONG Millisecond) 
	{
		PAGED_CODE();

		LARGE_INTEGER interval = {};
		interval.QuadPart = -(10000 * Millisecond);  // msec
		return KeDelayExecutionThread(KernelMode, FALSE, &interval);
	}

	//----------------------------------------------------------------------------------------//
	ULONG GetIndexByID(ULONG_PTR ID, ULONG TableSize)
	{
		return ID % TableSize;
	}
	//----------------------------------------------------------------------------------------//
	void ClearHashById(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID)
	{
		ULONG Index = GetIndexByID(ID, TableSize);
		HashTable[Index].ID = 0;
		HashTable[Index].Value = NULL;
		PMU_DEBUG_INFO_LN_EX("Clear - Index: %x Id: %I64x ", Index, ID);
	}

	//----------------------------------------------------------------------------------------//
	void SetHash(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID, ULONG lpNode)
	{
		ULONG Index = GetIndexByID(ID, TableSize);
		HashTable[Index].ID = ID;
		HashTable[Index].Value = lpNode;
		PMU_DEBUG_INFO_LN_EX("Add - Index: %x Id: %I64x ", Index, ID);
	}

	//----------------------------------------------------------------------------------------//
	BOOLEAN GetHashIndexById(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID, ULONG* count)
	{
		ULONG_PTR Index = GetIndexByID(ID, TableSize);
		if (HashTable[Index].ID == ID)
		{
			if (count)
			{
				*count = HashTable[Index].Value;
			}

			return TRUE;
		}
		return FALSE;
	}
	//--------------------------------------------------------------------------------------------//
	 
}