#pragma once

#include <intrin.h>
#include "x86.h"
#include "Apic.h"
#include "Util.h" 

#define START_DO_WHILE do{
#define END_DO_WHILE   }while(FALSE); 

#define DRV_NAME	  "PerfMon"

extern "C" 
{
	typedef ULONG64 u64;

	typedef struct _PMU_INFORMATION
	{
		UCHAR SupportedVersion;
		UCHAR SupportedFixedFunction;
		UCHAR SupportedBitWidth;
		UCHAR SupportedAnyThread;
		UCHAR SupportedNumOfPMCs;
		UCHAR SupporteWidthPerPMCs;
		UCHAR SupportedPerfEvents;
		UCHAR IsSupportPebs;
		UCHAR IsSupportEmon;
	}PMUINFO, *PPMUINFO;



	struct pebs_v1 {
		u64 flags;
		u64 ip;
		u64 ax;
		u64 bx;
		u64 cx;
		u64 dx;
		u64 si;
		u64 di;
		u64 bp;
		u64 sp;
		u64 r8;
		u64 r9;
		u64 r10;
		u64 r11;
		u64 r12;
		u64 r13;
		u64 r14;
		u64 r15;
		u64 status;
		u64 dla;
		u64 dse;
		u64 lat;
	};

	struct pebs_v2 {
		struct pebs_v1 v1;
		u64 eventing_ip;
		u64 tsx;
	};

	struct pebs_v3 {
		struct pebs_v2 v2;
		u64 tsc;
	};


	struct debug_store {
		u64 bts_base;
		u64 bts_index;
		u64 bts_max;
		u64 bts_thresh;

		u64 pebs_base;
		u64 pebs_index;
		u64 pebs_max;
		u64 pebs_thresh;
		u64 pebs_reset[4];
	};


	typedef struct _HASH_TABLE
	{
		ULONG_PTR ID;
		ULONG	  Value;
	}HASHTABLE, *PHASHTABLE;

	void SetHash(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID, ULONG count);
	BOOLEAN GetHashIndexById(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID, ULONG* count);
	void ClearHashById(HASHTABLE* HashTable, ULONG TableSize, ULONG_PTR ID);
	ULONG GetIndexByID(ULONG_PTR ID, ULONG TableSize);

	NTSTATUS UtilSleep(LONG Millisecond);
}