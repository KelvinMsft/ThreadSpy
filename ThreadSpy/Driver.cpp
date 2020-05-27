  
#include <ntddk.h> 
#include "Apic.h"
#include "x86.h"
#include "PMU.h"
#include "Common.h"
#include "PMI.h"
#include "Log.h"
extern "C"
{
  
	//////////////////////////////////////////////////////////////////
	////	Prototype	
	////	
	////
 

	//////////////////////////////////////////////////////////////////
	////	Global Variable	
	////	
	////
 
	PMUINFO			   g_EnvironmentInfo;

	//////////////////////////////////////////////////////////////////
	////	Marco
	////	
	////
 
	 
 
	//--------------------------------------------------------------//
	VOID DrvUnload(
			_In_ struct _DRIVER_OBJECT *DriverObject)
	{
		UNREFERENCED_PARAMETER(DriverObject);
		UnregisterPmiInterrupt();
		return;
	} 
 
	//--------------------------------------------------------------//
	NTSTATUS DriverEntry(
		_In_	PDRIVER_OBJECT DrvObj,
		_In_	PCUNICODE_STRING RegistryString)
	{ 
		UNREFERENCED_PARAMETER(RegistryString);
		NTSTATUS status = STATUS_SUCCESS;
		START_DO_WHILE
		
		status = RegisterPmiInterrupt();
		if (!NT_SUCCESS(status))
		{ 
			break;
		}
		
		PMU_DEBUG_INFO_LN_EX("Starting Environment Check... ");

		status = PMUEnvironmentCheck(&g_EnvironmentInfo);
		if (!NT_SUCCESS(status))
		{ 
			break;
		}
		 
		status = UtilForEachProcessor(
			PMUInitiailization, &g_EnvironmentInfo
		);
		if (!NT_SUCCESS(status))
		{ 
			break;
		}

		PMU_DEBUG_INFO_LN_EX("----------------------------------------------------------------------");
		PMU_DEBUG_INFO_LN_EX("SupportedVersion:%-30d",g_EnvironmentInfo.SupportedVersion		);
		PMU_DEBUG_INFO_LN_EX("SupportedFixedFunction:%-30d",g_EnvironmentInfo.SupportedFixedFunction	);
		PMU_DEBUG_INFO_LN_EX("SupportedBitWidth:%-30d "	,g_EnvironmentInfo.SupportedBitWidth		);
		PMU_DEBUG_INFO_LN_EX("SupportedAnyThread:%-30d "	,g_EnvironmentInfo.SupportedAnyThread		);
		PMU_DEBUG_INFO_LN_EX("SupportedNumOfPMCs:%-30d"	,g_EnvironmentInfo.SupportedNumOfPMCs		);
		PMU_DEBUG_INFO_LN_EX("SupporteWidthPerPMCs:%-30d"	,g_EnvironmentInfo.SupporteWidthPerPMCs	);
		PMU_DEBUG_INFO_LN_EX("SupportedPerfEvents:%-30d"	,g_EnvironmentInfo.SupportedPerfEvents		);
		PMU_DEBUG_INFO_LN_EX("IsSupportPebs:%-30d"		,g_EnvironmentInfo.IsSupportPebs				); 
		PMU_DEBUG_INFO_LN_EX("IsSupportEmon:%-30d", g_EnvironmentInfo.IsSupportEmon);
		 
		PMU_DEBUG_INFO_LN_EX("-----------------------------------------------------------------------");
		
		DrvObj->DriverUnload = DrvUnload;

		END_DO_WHILE

		return status;
	}
	//--------------------------------------------------------------//
}