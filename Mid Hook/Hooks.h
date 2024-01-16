#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#pragma warning(disable:4996)

void PlaceJMP(BYTE* Address, DWORD jumpTo, DWORD lenght) {
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(Address, lenght, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	dwRelAddr = (DWORD)(jumpTo - (DWORD)Address) - 5;
	*Address = 0xE9; //jmp
	*((DWORD*)(Address + 0x1)) = dwRelAddr;

	for (DWORD x = 0x5; x < lenght; x++) {
		*(Address + x) = 0x90; //nop
	}

	VirtualProtect(Address, lenght, dwOldProtect, &dwBkup);

}

void PlaceTestEdiEdi(BYTE* Address) {
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(Address, 2, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	*Address = 0x85; 
	*(Address + 1) = 0xFF;

	//malo miejsca wiec sie nie wetne. musze ustawic ZF = 0, mam 2 bajty miejsca.
	//EDI=0 w tym miejscu, wiec test edi,edi sie nada

	VirtualProtect(Address, 2, dwOldProtect, &dwBkup);

}

MODULEINFO GetModuleInfo(LPCWSTR szModule) {
	MODULEINFO modInfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);

	if (hModule == 0) return modInfo;

	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));

	return modInfo;

}

DWORD FindPattern(LPCWSTR module,const char* pattern) {
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLenght = (DWORD)strlen(pattern);

	for (DWORD i = 0; i < size - patternLenght; i++) {
		bool found = true;
		for (DWORD j = 0; j < patternLenght; j++) {
			found &=  pattern[j] == *(char*)(base + i + j);
		}

		if (found) {
			return base + i;
		}
	}

	return NULL;
}