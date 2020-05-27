#pragma once

#define PMU_NATIVE_DEBUG_INFO(format, ...)	DbgPrintEx(0,0,format,__VA_ARGS__) 
		
#define PMU_COMMON_DEBUG_INFO(format, ...)	PMU_NATIVE_DEBUG_INFO("[%s] => [%d] : "format, __FILE__ ,  __LINE__,  __VA_ARGS__)
#define PMU_DEBUG_INFO_LN()					PMU_NATIVE_DEBUG_INFO("\r\n")
#define PMU_DEBUG_INFO_LN_EX(format, ...)	PMU_COMMON_DEBUG_INFO(format"\r\n",__VA_ARGS__)

