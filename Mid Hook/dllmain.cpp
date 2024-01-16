// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"
#include <Windows.h>
#include "Hooks.h"
#include <iostream>
#include "Functions.h"

DWORD speedHackAddress;

void InitiateHooks() {
	//009AD380
	DWORD fastSpawningAddress = FindPattern(L"SpellForce.exe", "\x55\x8B\xEC\x66\x8B\x45\x08\x66\x29\x41\x08"); //zwraca adres na poczatek tego ciagu
	fastSpawningAddress += 3; //dodaj 3 bajty, aby nie uciac zadnej instrukcji
	fastSpawningBack = fastSpawningAddress + 0x8; //razem z jumpem zajmiemy 8 bajtow, aby nic nie ucicac
	//reszta jest wypelniana nop-ami
	
	PlaceJMP((BYTE*)fastSpawningAddress, (DWORD)fastSpawning, 8);//008FAEF8

	DWORD resourcesAddress = FindPattern(L"SpellForce.exe", "\x8D\x04\x80\x8D\x04\x82\x0F\xB6\x55\x10");
	resourcesAddress += 10; //jestem w Adzie

	resourcesBack = resourcesAddress + 0x6;

	PlaceJMP((BYTE*)resourcesAddress, (DWORD)resources, 6);

	DWORD speedHackAddress = FindPattern(L"SpellForce.exe", "\x56\x0F\xB7\x8A\x84\x00\x00\x00");
	speedHackAddress += 1;
	speedBack = speedHackAddress + 0x7;
	PlaceJMP((BYTE*)speedHackAddress, (DWORD)speedHack, 7);

	//00B85D59
	DWORD equipmentHackAddress = FindPattern(L"SpellForce.exe", "\xE8\xD2\x94\xFF\xFF\x85\xC0\x0F\x84");
	equipmentHackAddress += 5;
	PlaceTestEdiEdi((BYTE*)equipmentHackAddress);

}

DWORD WINAPI OverwriteValues() {
	for (;; Sleep(100)) {
		
		if (GetAsyncKeyState(VK_F1)) {
			if (bieg < 240) {
				bieg +=10;
			}
			else {
				bieg = 241;
			}
		}
		else if (GetAsyncKeyState(VK_F2)) {
			if (bieg > 11) {
				bieg -= 10;
			}
			else bieg = 11;
		}
	}
	return NULL;
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDll,
	DWORD fwdReason,
	LPVOID lpReserved
) {
	switch (fwdReason) {
	case DLL_PROCESS_ATTACH:
	{
		InitiateHooks();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverwriteValues, NULL, NULL, NULL);
		break;
	}
	}
	return TRUE;
}