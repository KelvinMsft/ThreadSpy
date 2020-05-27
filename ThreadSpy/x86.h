#pragma once

#define MSR_IA32_PERF_GLOBAL_STATUS		0x0000038E
#define MSR_IA32_APIC_BASE				0x0000001B			// The APIC base address register

#define MSR_IA32_RTIT_OUTPUT_BASE		0x00000560
#define MSR_IA32_RTIT_OUTPUT_MASK_PTRS	0x00000561
#define MSR_IA32_RTIT_CTL				0x00000570
#define MSR_IA32_RTIT_STATUS			0x00000571
#define MSR_IA32_X2APIC_LVT_PMI			0x00000834

// Filtering by CR3:
#define MSR_IA32_RTIT_CR3_MATCH			0x00000572

// Filtering by IP:
#define MSR_IA32_RTIT_ADDR0_START		0x00000580
#define MSR_IA32_RTIT_ADDR0_END			0x00000581
#define MSR_IA32_RTIT_ADDR1_START		0x00000582
#define MSR_IA32_RTIT_ADDR1_END			0x00000583
#define MSR_IA32_RTIT_ADDR2_START		0x00000584
#define MSR_IA32_RTIT_ADDR2_END			0x00000585
#define MSR_IA32_RTIT_ADDR3_START		0x00000586
#define MSR_IA32_RTIT_ADDR3_END			0x00000587

typedef ULONG DWORD;

/*------------------------------------------------------------------
18.2.1.2 Pre-defined Architectural Performance Events : 
----------------------------------------------------------------
| Bit Position | Event Name              | UMask | Event Select |
| (CPUID.AH.EBX)										        |
 ---------------------------------------------------------------
|  0			UnHalted Core Cycles       00H		 3CH		|
|  1			Instruction Retired        00H		 C0H		|
|  2			UnHalted Reference Cycles  01H		 3CH		|
|  3			LLC Reference			   4FH		 2EH		|
|  4			LLC Misses				   41H		 2EH		|
|  5			Branch Instruction Retired 00H		 C4H		|
|  6			Branch Misses Retired	   00H		 C5H		|
-----------------------------------------------------------------

------------------------------------------------------------------*/
union MSR_IA32_PERFEVTSLX_VERSION1
{
	struct
	{
		ULONG64 EventSelect : 8;	///< [7:0]	 Event Select Field
		ULONG64 UnitMask : 8;		///< [15:8]	 Unit Mask
		ULONG64 Usr : 1;			///< [16]	 User Mode
		ULONG64 Os : 1;				///< [17]	 Operating System Mode
		ULONG64 E : 1;				///< [18]	 Edge Detect
		ULONG64 Pc : 1;				///< [19]	 Pin Control
		ULONG64 Int : 1;			///< [20]	 APIC Interrupt Enable
		ULONG64 Reserved : 1;		///< [21]	 Resevred
		ULONG64 En : 1;				///< [22]	 Enable Counters
		ULONG64 Inv : 1;			///< [23]    Invert
		ULONG64 CounterMask : 8;	///< [31:24] Counter Mask
		ULONG64 Reserved2 : 32;		///< [63:32] Reserved
	}fields;
	ULONG64 all;
}; 
static_assert(sizeof(MSR_IA32_PERFEVTSLX_VERSION1) == 8, "Size check");

union MSR_IA32_PERFEVTSELX_VERSION3
{
	struct
	{
		ULONG64 EventSelect : 8;	///< [7:0]	 Event Select Field
		ULONG64 UnitMask : 8;		///< [15:8]	 Unit Mask
		ULONG64 Usr : 1;			///< [16]	 User Mode
		ULONG64 Os : 1;				///< [17]	 Operating System Mode
		ULONG64 E : 1;				///< [18]	 Edge Detect
		ULONG64 Pc : 1;				///< [19]	 Pin Control
		ULONG64 Int : 1;			///< [20]	 APIC Interrupt Enable
		ULONG64 AnyThread : 1;		///< [21]	 AnyThread
		ULONG64 En : 1;				///< [22]	 Enable Counters
		ULONG64 Inv : 1;			///< [23]    Invert
		ULONG64 CounterMask : 8;	///< [31:24] Counter Mask
		ULONG64 Reserved2 : 32;		///< [63:32] Reserved
	}fields;
	ULONG64 all;
}; 
static_assert(sizeof(MSR_IA32_PERFEVTSELX_VERSION3) == 8, "Size check");


//bit[0]:  0: disable; 1: OS; 2: User; 3: All ring levels 
//bit[1]   Controls for IA32_FIXED_CTRx , x < 3 , Enable for fixed-function PC to increment
union MSR_IA32_FIXED_CTR_CTRL_VERSION2
{
	struct
	{
		ULONG64 En : 2;			 /// < [1:0]	Enable Field,	 
		ULONG64 Reserved1 : 1;   /// < [2]		Reserved1	
		ULONG64 Pmi : 1;		 /// < [3]		PMI Field	,	Enable PMI Overflow	
		ULONG64 En2 : 2;		 /// < [5:4]	Enable Field2
		ULONG64 Reserved2 : 1;	 /// < [6]		Reserved2
		ULONG64 Pmi2 : 1;		 /// < [7]		PMI Field
		ULONG64 En3 : 2;		 /// < [9:8]	Enable Field2
		ULONG64 Reserved4 : 1;	 /// < [10]		Reserved2
		ULONG64 Pmi3 : 1;		 /// < [11]		PMI Field	
		ULONG64 Reserved5 : 52;  /// < [63:12]  Reserved	
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_FIXED_CTR_CTRL_VERSION2) == 8, "Size check");
 

//bit[0]:  0: disable; 1: OS; 2: User; 3: All ring levels 
//bit[1]   Controls for IA32_FIXED_CTRx , x < 3 , Enable for fixed-function PC to increment
union MSR_IA32_FIXED_CTR_CTRL_VERSION3
{
	struct
	{
		ULONG64 En : 2;			 /// < [1:0]	Enable Field,	 
		ULONG64 Any1 : 1;		 /// < [2]		Reserved1	
		ULONG64 Pmi : 1;		 /// < [3]		PMI Field	,	Enable PMI Overflow	
		ULONG64 En2 : 2;		 /// < [5:4]	Enable Field2
		ULONG64 Any12 : 1;		 /// < [6]		Reserved2
		ULONG64 Pmi2 : 1;		 /// < [7]		PMI Field
		ULONG64 En3 : 2;		 /// < [9:8]	Enable Field2
		ULONG64 Any13 : 1;		 /// < [10]		Reserved2
		ULONG64 Pmi3 : 1;		 /// < [11]		PMI Field	
		ULONG64 Reserved5 : 52;  /// < [63:12]  Reserved	
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_FIXED_CTR_CTRL_VERSION3) == 8, "Size check");

union MSR_IA32_PERF_GLOBAL_CTRL_VERSION2
{
	struct
	{
		ULONG64 EnablePmc0 : 1;			 /// < [0]	IA32_PMC0 Enabled
		ULONG64 EnablePmc1 : 1;			 /// < [1]	IA32_PMC0 Enabled
		ULONG64 Reserved   : 30;		 /// < [31:2]
		ULONG64	EnableCTR0 : 1;			 /// < [32] IA32_FIXED_CTR0 enable
		ULONG64	EnableCTR1 : 1;			 /// < [33] IA32_FIXED_CTR1 enable
		ULONG64	EnableCTR2 : 1;			 /// < [34] IA32_FIXED_CTR2 enable
		ULONG64	Reserved2  : 29;		 /// < [63:35]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_CTRL_VERSION2) == 8, "Size check");


union MSR_IA32_PERF_GLOBAL_STATUS_VERSION2
{
	struct
	{
		ULONG64 PMC0Overflow : 1;	 /// < [0]	IA32_PMC0 OverFlow
		ULONG64 PMC1Overflow : 1;	 /// < [1]	IA32_PMC0 OverFlow
		ULONG64 Reserved : 30;		 /// < [31:2]
		ULONG64	CTR0OverFlow : 1;	 /// < [32] IA32_FIXED_CTR0 OverFlow
		ULONG64	CTR1OverFlow : 1;	 /// < [33] IA32_FIXED_CTR1 OverFlow
		ULONG64	CTR2OverFlow : 1;	 /// < [34] IA32_FIXED_CTR2 OverFlow
		ULONG64	Reserved2 : 27;		 /// < [61:35]
		ULONG64 OvfDSBuffer : 1;	 /// < [62]
		ULONG64 CondChgd : 1;		 /// < [63]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_STATUS_VERSION2) == 8, "Size check");


union MSR_IA32_DEBUGCTL
{
	struct
	{
		ULONG64 LBR : 1;				/// < [0]	 
		ULONG64 BTF : 1;				/// < [1] 
		ULONG64 Reserved1 : 4;			/// < [5:2]
		ULONG64	TR : 1;					/// < [6]  
		ULONG64	BTS : 1;				/// < [7]  
		ULONG64	BTINT : 1;				/// < [8]  
		ULONG64	BTS_OFF_OS : 1;			/// < [9]
		ULONG64 BTS_OFF_USR: 1;			/// < [10]
		ULONG64 FRZ_LBRS_ON_PMI : 1	;	/// < [11]
		ULONG64 FRZ_PERFMON_ON_PMI : 1; /// < [12]
		ULONG64 UNCORE_PMI_EN : 1;		/// < [13]
		ULONG64 SMM_FRZ : 1;			/// < [14]
		ULONG64 Reserved2 : 49;			/// < [63:15]
	}fields;	
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_DEBUGCTL) == 8, "Size check");

union MSR_IA32_PERF_GLOBAL_CTRL
{
	struct
	{
		ULONG64 EN_PC0 : 1;				/// < [0]	 
		ULONG64 EN_PC1 : 1;				/// < [1] 
		ULONG64 EN_PC2 : 1;				/// < [2]
		ULONG64	EN_PC3 : 1;				/// < [3]  
		ULONG64	Reserved : 28;			/// < [31:4]  
		ULONG64	EN_FC0 : 1;				/// < [32]  
		ULONG64	EN_FC1 : 1;				/// < [33]
		ULONG64 EN_FC2 : 1;				/// < [34] 
		ULONG64 Reserved2 : 29;			/// < [63:35]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_CTRL) == 8, "Size check");


union MSR_IA32_MISC_ENABLE
{
	struct
	{
		ULONG64 FastStrEnable : 1;				/// < [0]	 
		ULONG64 Reserved : 2;					/// < [2:1] 
		ULONG64 AutoThermalCtrlCirEn: 1;		/// < [3]
		ULONG64	Reserved1 : 3;					/// < [6:4]  
		ULONG64	PerfMonAvaliable : 1;			/// < [7]  
		ULONG64	Reserved2 : 3;					/// < [10:8]  
		ULONG64	BTSUnavaliable : 1;				/// < [11]
		ULONG64 PEBSUnavaiable : 1;				/// < [12] 
		ULONG64 Reserved3 : 3;					/// < [15:13]
		ULONG64 SpeedStepEnable : 1;			/// < [16]
		ULONG64 Reserved5 : 1;					/// < [17]
		ULONG64 MonitorFsmEnable : 1;			/// < [18]
		ULONG64 Reserved6 : 3;					/// < [21:19]
		ULONG64 LimitCpuidMaxval : 1;			/// < [22]
		ULONG64 xTprMsgRwDisable : 1;			/// < [23]
		ULONG64 Reserved7 : 10;					/// < [33:24]
		ULONG64 XdDisable : 1;					/// < [34]
		ULONG64 Reserved8 : 29;					/// < [63:35]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_MISC_ENABLE) == 8, "Size check");


union MSR_IA32_PEBS_ENABLE
{
	struct
	{
		ULONG64 EnablePmc0 : 1;	/// < [0]	 
		ULONG64 EnablePmc1 : 1;	/// < [1]	 
		ULONG64 EnablePmc2 : 1;	/// < [2]	 
		ULONG64 EnablePmc3 : 1;	/// < [3]	 
		ULONG64 Reserved : 28;	/// < [31:4]
		ULONG64 LL_EN_PMC0 : 1;	/// < [32]
		ULONG64 LL_EN_PMC1 : 1;	/// < [33]
		ULONG64 LL_EN_PMC2 : 1;	/// < [34]
		ULONG64 LL_EN_PMC3 : 1;	/// < [35]
		ULONG64 Reserved2 : 28;	/// < [63:36]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PEBS_ENABLE) == 8, "Size check");


union MSR_IA32_PERF_CAPABILITIES
{
	struct {
		ULONG64 LbrFormat : 6;			/// <[5:0]
		ULONG64 PebsTrap : 1;			/// <[6]
		ULONG64 PebsSaveArchRegs : 1;	/// <[7]
		ULONG64 PebsRecordFormat : 4;	/// <[11:8]
		ULONG64 FreezeWhenSmm : 1;		/// <[12]
		ULONG64 FullWidthCounter : 1;	/// <[13]
		ULONG64 Reserved : 50;			/// <[63:14]
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PERF_CAPABILITIES) == 8, "Size check");


union MSR_IA32_PERF_GLOBAL_OVF_CTL
{
	struct
	{
		ULONG64 Ia32Pmc0Ovf : 1;
		ULONG64 Ia32Pmc1Ovf : 1;
		ULONG64 Reserved : 30;
		ULONG64 Ia32FixedCtr0Ovf : 1;
		ULONG64 Ia32FixedCtr1Ovf : 1;
		ULONG64 Ia32FixedCtr2Ovf : 1;
		ULONG64 Reserved1 : 27;
		ULONG64 ClrCondChgd : 1;		
		ULONG64 ClrOvfDSBuffer : 1; 
	}fields;
	ULONG64 all;
};
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_OVF_CTL) == 8, "Size check");

union MSR_IA32_PERF_GLOBAL_OVF_CTRL
{
	struct
	{
		ULONG64 OvfPmc0 : 1;			/// <[0]
		ULONG64 OvfPmc1 : 1;			/// <[1]
		ULONG64 OvfPmc2 : 1;			/// <[2]
		ULONG64 OvfPmcn : 1;			/// <[3]
		ULONG64 Reserved : 28;			/// <[n:4] , n If CPUID.0AH: EAX[15:8] > n
		ULONG64 OvfFixedCtr0 : 1;		/// <[32]
		ULONG64 OvfFixedCtr1 : 1;		/// <[33]
		ULONG64 OvfFixedCtr2 : 1;		/// <[34]
		ULONG64 Reserved2 : 20;			/// <[54:35]
		ULONG64 TraceToPaPmi : 1;		/// <[55]
		ULONG64 Reserved3 : 5;			/// <[60:56]
		ULONG64  OvfUnCore : 1;			/// <[61]
		ULONG64  OvfBuf : 1;			/// <[62]
		ULONG64  CondChgd : 1;			/// <[63]
	}fields;
	ULONG64 all;
}; 
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_OVF_CTRL) == 8, "Size check");


// A local vector table (LVT) entry
union LVT_Entry {
	struct {
		USHORT Vector : 8;					// [0:7] - The Vector number
		USHORT Reserved1 : 4;				// [8:11] - Reserved
		USHORT DeliveryStatus : 1;			// [12] - Delivery status: 0 - Idle; 1 - Send Pending;
		USHORT Reserved2 : 3;				// [13:15] - Reserved
		USHORT Masked : 1;					// [16] - Masked: 0 - Not Masked; 1 - Masked
		USHORT TimerMode : 2;				// [17:18] - Timer mode: 00 - One-shot; 01 - Periodic; 10 - TSC-Deadline;
		USHORT Reserved3 : 13;				// [19:31] - Reserved
	} Fields;
	DWORD All;
};
static_assert(sizeof(LVT_Entry) == 4, "Size check");

// The APIC Base physical address MSR in xAPIC Mode
union MSR_IA32_APIC_BASE_DESC {
	struct {
		ULONGLONG Reserved1 : 8;			// [0:7] - Reserved
		ULONGLONG Bsp : 1;					// [8] - Indicates if the processor is the bootstrap processor (BSP)
		ULONGLONG Reserved2 : 1;			// [9] - Reserved
		ULONGLONG EXTD : 1;					// [10] - Enable x2APIC mode
		ULONGLONG EN : 1;					// [11] - APIC global enable/disable
		ULONGLONG ApicBase : 24;			// [12:35] - Base Physical Address
	} Fields;
	ULONGLONG All;
};
static_assert(sizeof(MSR_IA32_APIC_BASE_DESC) == 8, "Size check");

// The IA32_PERF_GLOBAL_STATUS descriptor of Intel Broadwell microarchitecture 
union MSR_IA32_PERF_GLOBAL_STATUS_DESC {
	struct {
		DWORD PMC0_OVF : 1;					// [0] - Read only
		DWORD PMC1_OVF : 1;					// [1] - Read only 
		DWORD PMC2_OVF : 1;					// [2] - Read only
		DWORD PMC3_OVF : 1;					// [3] - Read only
		DWORD PMC4_OVF : 1;					// [4] - Read only (if PMC4 present)
		DWORD PMC5_OVF : 1;					// [5] - Read only (if PMC5 present) 
		DWORD PMC6_OVF : 1;					// [6] - Read only (if PMC6 present)
		DWORD PMC7_OVF : 1;					// [7] - Read only (if PMC7 present)
		DWORD Reserved : 24;				// [8:31] - Reserved
		DWORD FIXED_CTR0 : 1;				// [32] - FIXED_CTR0 Overflow (RO)
		DWORD FIXED_CTR1 : 1;				// [33] - FIXED_CTR1 Overflow (RO)
		DWORD FIXED_CTR2 : 1;				// [34] - FIXED_CTR2 Overflow (RO)
		DWORD Reserved2 : 20;				// [35:54] - Reserved
		DWORD TraceToPAPMI : 1;				// [55] - The ToPA PMI Interrupt status
		DWORD Reserved3 : 5;				// [56:60] - Reserved
		DWORD Ovf_UncorePMU : 1;			// [61]
		DWORD Ovf_Buffer : 1;				// [62]
		DWORD CondChgd : 1;					// [63]
	} Fields;
	ULONGLONG All;
};
static_assert(sizeof(MSR_IA32_PERF_GLOBAL_STATUS_DESC) == 8, "Size check");

#pragma pack(push,1)
typedef struct IDTDESC
{
	USHORT limit;
	ULONG64 BASE;
}IDTDESC, *PIDTDESC;
static_assert(sizeof(IDTDESC) == 10, "Size check");


typedef union _KIDTENTRY64
{
	struct
	{
		USHORT OffsetLow;
		USHORT Selector;
		USHORT IstIndex : 3;
		USHORT Reserved0 : 5;
		USHORT Type : 5;
		USHORT Dpl : 2;
		USHORT Present : 1;
		USHORT OffsetMiddle;
		ULONG  OffsetHigh;
		ULONG  Reserved1;
	}u;
	UINT64 Alignment;
} KIDTENTRY64, *PKIDTENTRY64;
#pragma pack(pop) 
/// See: MODEL-SPECIFIC REGISTERS (MSRS)
enum class Msr : unsigned int {
	Ia32ApicBase = 0x01B,

	Ia32FeatureControl = 0x03A,
	Ia32PMCx = 0xC1,
	Ia32SysenterCs = 0x174,
	Ia32SysenterEsp = 0x175,
	Ia32SysenterEip = 0x176,
	Ia32PerfEvtseLx = 0x186,
	Ia32Debugctl = 0x1D9,

	//PMU Related
	Ia32FixedCtrl0 = 0x309,
	Ia32FixedCtrl1 = 0x30A,
	Ia32FixedCtrl2 = 0x30B,
	Ia32PerfCaps = 0x345,
	Ia32FixedCtrl = 0x38D,
	Ia32PerfGlobalStatus = 0x38E,	// allows software to query counter overflow conditions on any combination of fixed - function PMCs or general - purpose PMCs via a single RDMSR.
	Ia32PerfGlobalCtrl = 0x38F,	// allows software to enable/disable event counting of all or any combination of fixed - function PMCs(IA32_FIXED_CTRx) or any general - purpose PMCs via a single WRMSR.
	Ia32PerfGlobalOvfCtrl = 0x390,  // allows software to clear counter overflow conditions on any combination of fixed - function PMCs or general - purpose PMCs via a single WRMSR.


	//PMU PEBS Related
	Ia32MiscEnable = 0x1A0,
	Ia32PebsEnable = 0x3F1,
	Ia32DsArea = 0x600,

	Ia32VmxBasic = 0x480,
	Ia32VmxPinbasedCtls = 0x481,
	Ia32VmxProcBasedCtls = 0x482,
	Ia32VmxExitCtls = 0x483,
	Ia32VmxEntryCtls = 0x484,
	Ia32VmxMisc = 0x485,
	Ia32VmxCr0Fixed0 = 0x486,
	Ia32VmxCr0Fixed1 = 0x487,
	Ia32VmxCr4Fixed0 = 0x488,
	Ia32VmxCr4Fixed1 = 0x489,
	Ia32VmxVmcsEnum = 0x48A,
	Ia32VmxProcBasedCtls2 = 0x48B,
	Ia32VmxEptVpidCap = 0x48C,
	Ia32VmxTruePinbasedCtls = 0x48D,
	Ia32VmxTrueProcBasedCtls = 0x48E,
	Ia32VmxTrueExitCtls = 0x48F,
	Ia32VmxTrueEntryCtls = 0x490,
	Ia32VmxVmfunc = 0x491,

	Ia32x2ApivIvtPmi = 0x00000834,

	Ia32Efer = 0xC0000080,
	Ia32Star = 0xC0000081,
	Ia32Lstar = 0xC0000082,

	Ia32Fmask = 0xC0000084,

	Ia32FsBase = 0xC0000100,
	Ia32GsBase = 0xC0000101,
	Ia32KernelGsBase = 0xC0000102,
	Ia32TscAux = 0xC0000103,
	 
};


enum class CpuMircoArchModel : unsigned int
{
	SandyBridge = 0x2A,
	SandyBridge_E = 0x2D,
	IvyBridge = 0x3A,

	Arrandale = 0x25,
	Gulftown  = 0x2C,
	Westmere_Ex = 0x2F,

};