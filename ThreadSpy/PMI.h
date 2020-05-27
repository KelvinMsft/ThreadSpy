#pragma once
#include <ntddk.h>

extern "C" {
	NTSTATUS RegisterPmiInterrupt();
	NTSTATUS UnregisterPmiInterrupt();
	VOID DisablePmi();
	VOID EnablePmi();
}