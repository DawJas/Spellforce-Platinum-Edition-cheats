DWORD fastSpawningBack = 0;
DWORD resourcesBack = 0;
DWORD speedBack = 0;

int resourc = 12345;
short bieg = 253;

__declspec(naked) void fastSpawning() {
	__asm mov ax, [ebp+8]
	__asm sub [ecx+8], ax
	__asm mov [ecx+8], 0 //czas spawnowania, czyli 0
	__asm mov ax, [ecx+8]
	__asm jmp fastSpawningBack
}

__declspec(naked) void resources() {
	__asm add eax,edx
	__asm push ebx
	__asm mov ebx, [resourc]
	__asm mov [ecx+eax*4+0x0B], ebx
	__asm pop ebx
	__asm mov eax, [ecx+4*eax+0x0B]
	__asm jmp resourcesBack

}

__declspec(naked) void speedHack() {
	__asm movzx ecx, [bieg]
	__asm jmp speedBack
}
