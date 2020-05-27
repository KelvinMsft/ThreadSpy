#include <ntddk.h>
#include "PMI.h"
#include "Apic.h"
#include "Common.h"
#include "Log.h"
#include "x86.h"  
#include "ntimage.h"
#include "PMU.h"
extern "C"
{
	//////////////////////////////////////////////////////////////////
	////	Types
	////	 

	PCHAR PsGetProcessImageFileName(PEPROCESS);
	
 
	// The PMI Handler function prototype
	typedef VOID(*PMIHANDLER)(
		_In_ PKTRAP_FRAME TrapFrame
		);

	typedef struct _DRIVER_GLOBAL_DATA {
		BOOLEAN bPtSupported;								// TRUE if Intel PT is supported 
		BOOLEAN bCpuX2ApicMode;								// TRUE if the system processors are in x2Apic Mode   
		PMIHANDLER pOldPmiHandler;							// The OLD PMI handler routine (if any)   
		ULONG* lpApicBase;									// The APIC I/O memory VA    
	}DRIVER_GLOBAL_DATA, *PDRIVER_GLOBAL_DATA;
 

 

	///////////////////////////////////////////////////////////////////////
	//// Global Variable 
	////
	 
	DRIVER_GLOBAL_DATA			g_pDrvData = { 0 };
	bool						g_IsUninit = false;
	HASHTABLE					g_inst_table[10000] = { 0 };
	ULONG_PTR					g_InterruptFuncTable[256] = { 0 };
 
	ULONG_PTR					g_IntCount = 0;

	///////////////////////////////////////////////////////////////////////
	//// Marco
	//// 
 
 
	 
	//--------------------------------------------------------------//
	NTSTATUS DispatchPmiEvent(PKTRAP_FRAME pTrapFrame)
	{
		pTrapFrame;
		NTSTATUS status = STATUS_SUCCESS;
		g_IntCount++;
		
		if (!g_IntCount || ((g_IntCount % 1000000) != 0)) {
			return status;
		}

		PMU_DEBUG_INFO_LN_EX("[%d] Process= %s \t PID= %p \t TID= %p ", KeGetCurrentProcessorNumber(), PsGetProcessImageFileName(PsGetCurrentProcess()), PsGetCurrentProcessId(), PsGetCurrentThreadId());
		PMU_DEBUG_INFO_LN_EX("RIP= %p RAX= %p RBX= %p RCX= %p \r\n RDX= %p RSI= %p RSP= %p RBP= %p ", pTrapFrame->Rip, pTrapFrame->Rax, pTrapFrame->Rbx, pTrapFrame->Rcx,
			pTrapFrame->Rdx, pTrapFrame->Rsi, pTrapFrame->Rsp, pTrapFrame->Rbp);
		return status;
	} 


	//--------------------------------------------------------------//
	NTSTATUS InitInterrupt()
	{
		IDTDESC info = { 0 };
		PKIDTENTRY64 IdtEntry;
		__sidt(&info);
		IdtEntry = (PKIDTENTRY64)info.BASE;
		for (int i = 0; i < 256; i++)
		{
			ULONG64 handler = 0;
			handler = ((ULONG64)IdtEntry[i].u.OffsetLow | ((ULONG64)IdtEntry[i].u.OffsetMiddle << 16) | ((ULONG64)IdtEntry[i].u.OffsetHigh << 32));
			g_InterruptFuncTable[i] = handler;
			PMU_DEBUG_INFO_LN_EX("Idt[%x]: %p", i, handler);
		}
		return STATUS_SUCCESS;

	}
	//--------------------------------------------------------------//
	NTSTATUS InitApic()
	{
		// First of all we need to search for HalpLocalApic symbol
		MSR_IA32_APIC_BASE_DESC ApicBase = { 0 };				// In Multi-processors systems this address could change
		NTSTATUS status = STATUS_SUCCESS;
		status = UtilReadMsr(Msr::Ia32ApicBase, &ApicBase.All);			// In Windows systems all the processors LVT are mapped at the same physical address

		if (!NT_SUCCESS(status))
		{
			return status;
		}

		if (!ApicBase.Fields.EXTD)
		{
			ULONG* lpdwApicBase = NULL;
			PHYSICAL_ADDRESS apicPhys = { 0 };

			apicPhys.QuadPart = ApicBase.All & (~0xFFFi64);
			lpdwApicBase = (ULONG*)MmMapIoSpace(apicPhys, 0x1000, MmNonCached);

			if (lpdwApicBase)
			{
				DbgPrintEx(0, 0, "[" DRV_NAME "] Successfully mapped the local APIC to 0x%llX.\r\n", lpdwApicBase);
				g_pDrvData.lpApicBase = lpdwApicBase;
			}
			else
			{
				status = STATUS_NOT_SUPPORTED;
				return status;
			}
		}
		else
		{
			// Current system uses x2APIC mode, no need to map anything
			g_pDrvData.bCpuX2ApicMode = TRUE;
		}
		return status;
	}
	//--------------------------------------------------------------//
	NTSTATUS SetUpPerformanceInterrutpHandler(PMIHANDLER Handler)
	{
		PMIHANDLER pNewPmiHandler = Handler;
		NTSTATUS ntStatus = HalSetSystemInformation(HalProfileSourceInterruptHandler, sizeof(PMIHANDLER), (PVOID)&pNewPmiHandler);
		if (NT_SUCCESS(ntStatus))
		{
			DbgPrintEx(0, 0, "[" DRV_NAME "] Successfully registered system PMI handler to function 0x%llX.\r\n", (PVOID)pNewPmiHandler);
		}
		return ntStatus;
	}
	//--------------------------------------------------------------//
	NTSTATUS ResetApic()
	{
		LVT_Entry perfMonDesc = { 0 };
		PULONG lpdwApicBase = g_pDrvData.lpApicBase;
		NTSTATUS status = STATUS_SUCCESS;
		if (g_pDrvData.bCpuX2ApicMode)
		{
			// Check Intel Manuals, Vol. 3A section 10-37
			ULONGLONG perfMonEntry = 0;
			UtilReadMsr(Msr::Ia32x2ApivIvtPmi, &perfMonEntry);
			perfMonDesc.All = (ULONG)perfMonEntry;
			perfMonDesc.Fields.Masked = 0;
			perfMonEntry = (ULONGLONG)perfMonDesc.All;
			UtilWriteMsr(Msr::Ia32x2ApivIvtPmi, perfMonEntry);
		}
		else
		{
			if (!lpdwApicBase)
				// XXX: Not sure how to continue, No MmMapIoSpace at this IRQL (should not happen)
				KeBugCheckEx(INTERRUPT_EXCEPTION_NOT_HANDLED, NULL, NULL, NULL, NULL);

			perfMonDesc.All = lpdwApicBase[0x340 / 4];
			perfMonDesc.Fields.Masked = 0;
			lpdwApicBase[0x340 / 4] = perfMonDesc.All;
		}
		return status;
	}
	//--------------------------------------------------------------//
	VOID IntelPerformanceMonitorInterrupt(PKTRAP_FRAME pTrapFrame)
	{

		START_DO_WHILE

		if (g_IsUninit)
		{
			break;
		}

		//DisablePmi();

		DispatchPmiEvent(pTrapFrame);

		UtilWriteMsr(Msr::Ia32PerfEvtseLx, 0);

		UtilWriteMsr(Msr::Ia32PMCx, (ULONG)0xFFFFFFF0);

		MSR_IA32_PERFEVTSELX_VERSION3 PerfEvtSelx = { 0 };
		PerfEvtSelx.fields.Usr = true;
		PerfEvtSelx.fields.Os = false;
		PerfEvtSelx.fields.E = false;
		PerfEvtSelx.fields.Int = true;
		PerfEvtSelx.fields.CounterMask = 0;
		PerfEvtSelx.fields.En = true;
		PerfEvtSelx.fields.AnyThread = false;
		PerfEvtSelx.fields.EventSelect = 0xC0;
		PerfEvtSelx.fields.UnitMask = 0x00;
		PerfEvtSelx.fields.Inv = false;
		PerfEvtSelx.fields.Pc = false;

		UtilWriteMsr(Msr::Ia32PerfEvtseLx, PerfEvtSelx.all);


		UtilWriteMsr(Msr::Ia32PerfGlobalOvfCtrl, 0x1);
		//EnablePmi();

		END_DO_WHILE

			 
		ResetApic();

		return;
	}
	//--------------------------------------------------------------//
	VOID ClearPmuEnvironment()
	{
		UtilWriteMsr(Msr::Ia32PerfGlobalCtrl, 0);

		UtilWriteMsr(Msr::Ia32PerfGlobalOvfCtrl, 0);

		UtilWriteMsr(Msr::Ia32PMCx, 0);
		 
	}
	//--------------------------------------------------------------//
	NTSTATUS RegisterPmiInterrupt()
	{
		NTSTATUS ntStatus = STATUS_SUCCESS;
	 
		ntStatus = InitInterrupt();
		if (!NT_SUCCESS(ntStatus))
		{
			return ntStatus;
		}

		ntStatus = InitApic();
		if (!NT_SUCCESS(ntStatus))
		{
			return ntStatus;
		}
		ntStatus = SetUpPerformanceInterrutpHandler(IntelPerformanceMonitorInterrupt);
		if (!NT_SUCCESS(ntStatus))
		{
			return ntStatus;
		}

		return ntStatus;
	}
	//--------------------------------------------------------------//
	// Unregister and remove the LVT PMI interrupt 
	NTSTATUS UnregisterPmiInterrupt()
	{
		NTSTATUS ntStatus = STATUS_SUCCESS;						// Returned NTSTATUS
		PMIHANDLER pOldPmiHandler = g_pDrvData.pOldPmiHandler;	// The old PMI handler 

		ClearPmuEnvironment();

		g_IsUninit = TRUE;
 
		// This is currently not restoring old PMI handler since we don't know how to retrieve it, just nulling it out
		ntStatus = HalSetSystemInformation(HalProfileSourceInterruptHandler, sizeof(PMIHANDLER), (PVOID)&pOldPmiHandler);

		if (NT_SUCCESS(ntStatus))
		{
			return ntStatus;
		}
		 
		if (g_pDrvData.lpApicBase)
		{
			MmUnmapIoSpace(g_pDrvData.lpApicBase, 0x1000);
		}


		 
		return ntStatus;
	}
}
