#include "hook.h"

bool CHook::SetHook(tAttached process, const char * func, const char * dll)
{
	// Resolve export address of target function
	void * address = (void*)GetProcAddress(GetModuleHandleA(dll), func);

	if (!address)
	{
		dbg.print(LOG_FAILED, " - Failed to resolve %s!%s\n", dll, func);
		return false;
	}

	BYTE jmp[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

}
