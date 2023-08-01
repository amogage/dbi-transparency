{% raw %}
/*
ARCH: Windows x86/x64
This example scans the entire process memory, searching for DBI vm-leave instruction sequences
Each DBI has its own DBI piece of code, and sometimes this gets updated in incremental versions of a DBI, making new versions to pass, where the previous ones failed
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo
*/

#include <Windows.h>
#include <stdio.h>

#pragma warning (disable:4996)

#define NUM_PATTERNS 7

const BYTE dynamorio_vmleave_32_1[] = {
    0x89,0x1D,'?', '?', '?', '?',   //mov [...], ebx
    0x89,0x0D,'?', '?', '?', '?',   //mov [...], ecx
    0x89,0x15,'?', '?', '?', '?',   //mov [...], edx
    0x89,0x35,'?', '?', '?', '?',   //mov [...], esi
    0x89,0x3D,'?', '?', '?', '?',   //mov [...], edi    
    0x89,0x2D,'?', '?', '?', '?',   //mov [...], ebp
    0x89,0x25,'?', '?', '?', '?',   //mov [...], esp
    0x8B,0x25,'?', '?', '?', '?',   //mov esp, [...]
    0x9C,                           //pushf
    0x5B,                           //pop ebx
    0x89,0x1D,'?', '?', '?', '?',   //mov [...], ebx
    0x6A,0x00,                      //push 0
    0x9D                            //popf
};

const BYTE dynamorio_vmleave_32_2[] = {
    0x64,0x89,0x3D,'?','?',0x00,0x00,   //mov fs:[0xef0], edi
    0x64,0x8B,0x3D,'?','?',0x00,0x00,   //mov edi, fs:[0xef4]
    0x89,0x5F,'?',                      //mov [edi+0x10], ebx
    0x64,0x8B,0x1D,'?','?',0x00,0x00,   //mov ebx, fs:[0xee4]
    0x89,0x5F,'?',                      //mov [edi+0x1c], ebx
    0x64,0x8B,0x1D,'?','?',0x00,0x00,   //mov ebx, fs:[0xef0]
    0x89,0x1F,                          //mov [edi], ebx
    0x89,0x4F,'?',                      //mov [edi+0x18], ecx
    0x89,0x57,'?',                      //mov [edi+0x14], edx
    0x89,0x77,'?',                      //mov [edi+4], esi
    0x89,0x6F,'?',                      //mov [edi+8], ebp
    0x89,0x67,'?',                      //mov [edi+0xc], esp
    0x8B,0xA7,'?','?',0x00,0x00,        //mov esp, [edi+0x2a4]
    0x9C,                               //pushf
    0x5B,                               //pop ebx
    0x89,0x5F,'?',                      //mov [edi+0x20], ebx
    0x6A,0x00,                          //push 0
    0x9D                                //popf
};

const BYTE flow_vmleave_64[] = {
    0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x5F, 0x5E, 0x5D, 0x5A, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5A, 0x59, 0x58, 0x9D
};

const BYTE cobai_vmleave_32[] = {
    0x9C,
    0x60,
    0xE8,0x00,0x00,0x00,0x00,
    0x5A,
    0x81,0xE2,0x00,0x00,0xFF,0xFF,
    0x89,0xA2,0x04,0x06,0x00,0x00,
    0x0F,0xAE,0x82,0x00,0x0A,0x00,0x00,
    0x8B,0xA2,0x00,0x06,0x00,0x00,
    0x0F,0xAE,0x8A,0x00,0x08,0x00,0x00,
    0x61,
    0x9D,
    0xE8,0x00,0x00,0x00,0x00,
    0x5A,
    0x81,0xE2,0x00,0x00,0xFF,0xFF,
    0x8B,0x92,0x04,0x06,0x00,0x00,
    0x8B,0x4D,0x08,
    0x89,0x11,
    0x89,0xEC,
    0x5D,
    0xC3
};

const BYTE pin_vmleave_32_1[] = {
    0x89,0x63,'?',                  //mov [ebx+0x4c], esp
    0x8B,0xA3,'?','?',0x00,0x00,    //mov esp, [ebx+0x514]
    0xFF,0x93,'?','?',0x00,0x00     //call [ebx+0x53c]  
};

const BYTE pin_vmleave_32_2[] = {
    0x9F,                           //lahf
    0x0F,0x90,0xC0,                 //seto al
    0x66,0x89,0x43,'?',             //mov [ebx+0x78],ax
    0x8B,0x83,'?','?',0x00,0x00,    //mov eax, [ebx+0x524]
    0xC1,0xE8,'?',                  //shr eax, 4
    0x25,'?','?',0x00,0x00,         //and eax, 0x3fff
    0x66,0x0F,0x1F,0x44,'?','?',    //nop word [eax+eax+0]
    0xFF,0x24,0x85,'?','?','?','?', //jmp [eax*4+0xa550010]
    0x00,0x8B,'?','?','?','?',      //add [ebx+0x52c8b], cl
    0x00,0x8D,'?','?','?','?',      //add [ebp+0x21ede289], cl
    0x88,0xE3,                      //mov bl, bh
    0x07,                           //pop es
    0x66,                           //db 0x66
    0x90,                           //nop
    0xE9,'?','?','?','?'            //jmp B1    
};

const BYTE qbdi_vmleave_32[] = {
    0x89,0x05,'?','?','?','?',
    0x89,0x1D,'?','?','?','?',
    0x89,0x0D,'?','?','?','?',
    0x89,0x15,'?','?','?','?',
    0x89,0x35,'?','?','?','?',
    0x89,0x3D,'?','?','?','?',
    0x89,0x2D,'?','?','?','?',
    0x89,0x25,'?','?','?','?',
    0x8B,0x25,'?','?','?','?',
    0x9C,
    0x58,
    0x89,0x05,'?','?','?','?',
    0x8B,0x05,'?','?','?','?',
    0xF7,0xC0,0x02,0x00,0x00,0x00,
    0x0F,0x84,0x07,0x00,0x00,0x00,
    0x0F,0xAE,0x05,'?','?','?','?',
    0xF7,0xC0,0x01,0x00,0x00,0x00,
    0x0F,0x84,0x50,0x00,0x00,0x00,
    0xC4,
    0xE3,0x7D,
    0x19,0x05,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x0D,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x15,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x1D,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x25,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x2D,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x35,'?','?','?','?',
    0x01,0xC4,
    0xE3,0x7D,
    0x19,0x3D,'?','?','?','?',
    0x01,0xC3
};

#ifndef _M_X64
#define MAX_MEM_ADDRESS 0x7FFFFFFF
#else
#define MAX_MEM_ADDRESS 0x7FFFFFFFFFFFFFFFULL
#endif


const BYTE* needles[NUM_PATTERNS] = { pin_vmleave_32_1, pin_vmleave_32_2, qbdi_vmleave_32, cobai_vmleave_32, flow_vmleave_64, dynamorio_vmleave_32_1, dynamorio_vmleave_32_2 };
const size_t needles_sizes[NUM_PATTERNS] = { sizeof(pin_vmleave_32_1), sizeof(pin_vmleave_32_1), sizeof(qbdi_vmleave_32), sizeof(cobai_vmleave_32), sizeof(flow_vmleave_64), sizeof(dynamorio_vmleave_32_1), sizeof(dynamorio_vmleave_32_2)};

MEMORY_BASIC_INFORMATION* enumerate_memory(size_t* total_count)
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    MEMORY_BASIC_INFORMATION* regions = (MEMORY_BASIC_INFORMATION*)malloc(1000 * sizeof(MEMORY_BASIC_INFORMATION));
    memset(regions, 0, 1000 * sizeof(MEMORY_BASIC_INFORMATION));

    const PBYTE MaxAddress = (PBYTE)MAX_MEM_ADDRESS;

    PBYTE addr = 0;
    size_t i = 0;

    while (addr < MaxAddress)
    {
        memset(&crt, 0, sizeof(crt));
        if (VirtualQuery(addr, &crt, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
            break;
        addr += crt.RegionSize;
        if (crt.State == MEM_FREE)
            continue;
        if ((crt.State & MEM_COMMIT) == MEM_COMMIT && ((crt.Protect == PAGE_EXECUTE_READ) || (crt.Protect == PAGE_EXECUTE_READWRITE)))
        {
            memcpy(&regions[i], &crt, sizeof(crt));
            i++;
        }
    }

    total_count[0] = i;

    return regions;
}

void DumpMemoryRange(void* BaseAddress, size_t RegionSize)
{
    printf("payload at [%p] size:%X\n", BaseAddress, RegionSize);
    char dump_name[MAX_PATH];
    DWORD write;
    sprintf(dump_name, "_dump_%p_%X.bin", BaseAddress, RegionSize);
    HANDLE f = CreateFileA(dump_name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(f, BaseAddress, RegionSize, &write, 0);
    CloseHandle(f);
}

int main()
{
    size_t total_count = 0;
    int DBI_type[7] = { 0, 0, 0, 0, 0, 0, 0 };
    HMODULE hm;
    char mname[MAX_PATH];
    char dbis[7][20] = {"pin_vmleave_1", "pin_vmleave_2", "qbdi_vmleave", "cobai_vmleave", "flow_vmleave", "dynamorio_vmleave_1", "dynamorio_vmleave_2"};

    MEMORY_BASIC_INFORMATION* memoryRegions = enumerate_memory(&total_count);
    printf("[%d] ranges \n", total_count);

    printf("Search: ... \n");
    for (size_t i = 0; i < total_count; i++)
    {
        if (memoryRegions[i].Protect != PAGE_EXECUTE_READ && memoryRegions[i].Protect != PAGE_EXECUTE_READWRITE)
            continue;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (char*)memoryRegions[i].BaseAddress, &hm))
        {
            GetModuleFileNameA(hm, mname, MAX_PATH);
            //printf("[%s]\n", mname);
            for (int j = 0; j < strlen(mname); j++)
                if (mname[j] >= 'A' && mname[j] <= 'Z')
                    mname[j] |= 0x20;
            if (strstr(mname, "windows"))
                continue;
        }
        //else
            //DumpMemoryRange(memoryRegions[i].BaseAddress, memoryRegions[i].RegionSize);

        //printf("[P]");
        PBYTE addr = (PBYTE)memoryRegions[i].BaseAddress;
        PBYTE regionEnd = addr + memoryRegions[i].RegionSize;

        while (addr < regionEnd - 10)
        {
            bool no_mem = false;
            bool update = false;
            size_t match_dr = 0;
            for (size_t j = 0; j < NUM_PATTERNS; j++)
            {
                switch (j)
                {
                    case 3:
                    case 4:
                        __try {
                            if (!memcmp(addr, needles[j], needles_sizes[j]) && addr != needles[j])
                            {
                                update = true;
                                addr += needles_sizes[j];
                                DBI_type[j]++;
                                //printf("X");
                            }
                        }
                        __except (EXCEPTION_EXECUTE_HANDLER) {
                            //printf("!");
                            no_mem = true;
                        }
                        break;
                    case 0:
                    case 1:
                    case 2:
                    case 5:
                    case 6:
                        __try {
                            match_dr = 0;
                            for (size_t k = 0; k < needles_sizes[j]; k++)
                            {
                                if (needles[j][k] == '?' || needles[j][k] == addr[k])
                                    match_dr++;
                            }
                            if (match_dr == needles_sizes[j] && addr != needles[j])
                            {
                                update = true;
                                addr += needles_sizes[j];
                                DBI_type[j]++;
                                //printf("Y");
                            }                            
                        }
                        __except (EXCEPTION_EXECUTE_HANDLER) {
                            //printf("!");
                            no_mem = true;
                        }
                        break;
                }
                if (update)
                    break;
            }
            if (!update)
                addr++;
            if (no_mem)
                break;
        }
    }
    printf("\n");
    free(memoryRegions);

    bool flag = false;
    for (int i = 0; i < 7; i++)
    {
        if (!DBI_type[i])
            continue;
        printf("VMLeave was found for [%s]\n", dbis[i]);
        flag = true;
    }

    if (!flag)
        printf("\nPASSED");
    else
        printf("\nFAILED");

    return 0;
}
{% endraw %}