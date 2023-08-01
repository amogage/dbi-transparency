{% raw %}
/*
ARCH: Windows x86
This example sequence a few loops summing up to 2.1 billion iterations, adding a huge overhead to instrumentation
API instrumentation for DBIs diving into OS libraries, increase this overhead exponentially, while a native execution should be "gone in 60s", instrumentation goes at least 10x higher:
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>

double powerOfTen(int num)
{
    double rst = 1.0;
    if (num >= 0)
    {
        for (int i = 0; i < num; i++)
            rst *= 10.0;
    }
    else
    {
        for (int i = 0; i < (0 - num); i++)
            rst *= 0.1;
    }

    return rst;
}

double squareRoot(double a)
{
    double z = a;
    double rst = 0.0;
    int max = 8;
    int i;
    double j = 1.0;
    for (i = max; i > 0; i--)
    {
        if (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0)
        {
            while (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0)
            {
                j++;
                if (j >= 10) break;

            }
            j--;
            z -= ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)); //find value of z

            rst += j * powerOfTen(i);

            j = 1.0;
        }
    }

    for (i = 0; i >= 0 - max; i--)
    {
        if (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0)
        {
            while (z - ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i)) >= 0)
            {
                j++;
                if (j >= 10) break;
            }
            j--;
            z -= ((2 * rst) + (j * powerOfTen(i))) * (j * powerOfTen(i));

            rst += j * powerOfTen(i);
            j = 1.0;
        }
    }
    return rst;
}

void MegaSqrt()
{
    LARGE_INTEGER start, stop, freq;
    double timp;
    DWORD curent;
    double r;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (size_t i = 0; i < 10000000; i++)
    {
        if ((i % 100000) == 0)
            printf(".");
        curent = (DWORD)__rdtsc();
        r = squareRoot(curent);
    }
    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("time = %.3lf\n", timp);
}

int f001(int a, int b) { return b + a * 2 + 1001; }
int f002(int a, int b) { return a + a * 2 + 1002; }
int f003(int a, int b) { return b + a * 2 + 1003; }
int f004(int a, int b) { return a + a * 2 + 1004; }
int f005(int a, int b) { return b + a * 2 + 1005; }
int f006(int a, int b) { return a + a * 2 + 1006; }
int f007(int a, int b) { return b + a * 2 + 1007; }
int f008(int a, int b) { return a + a * 2 + 1008; }
int f009(int a, int b) { return b + a * 2 + 1009; }
int f010(int a, int b) { return a + a * 2 + 1010; }
int f011(int a, int b) { return b + a * 2 + 1011; }
int f012(int a, int b) { return a + a * 2 + 1012; }
int f013(int a, int b) { return b + a * 2 + 1013; }
int f014(int a, int b) { return a + a * 2 + 1014; }
int f015(int a, int b) { return b + a * 2 + 1015; }
int f016(int a, int b) { return a + a * 2 + 1016; }
int f017(int a, int b) { return b + a * 2 + 1017; }
int f018(int a, int b) { return a + a * 2 + 1018; }
int f019(int a, int b) { return b + a * 2 + 1019; }
int f020(int a, int b) { return a + a * 2 + 1020; }
int f021(int a, int b) { return b + a * 2 + 1021; }
int f022(int a, int b) { return a + a * 2 + 1022; }
int f023(int a, int b) { return b + a * 2 + 1023; }
int f024(int a, int b) { return a + a * 2 + 1024; }
int f025(int a, int b) { return b + a * 2 + 1025; }
int f026(int a, int b) { return a + a * 2 + 1026; }
int f027(int a, int b) { return b + a * 2 + 1027; }
int f028(int a, int b) { return a + a * 2 + 1028; }
int f029(int a, int b) { return b + a * 2 + 1029; }
int f030(int a, int b) { return a + a * 2 + 1030; }
int f031(int a, int b) { return b + a * 2 + 1031; }
int f032(int a, int b) { return a + a * 2 + 1032; }
int f033(int a, int b) { return b + a * 2 + 1033; }
int f034(int a, int b) { return a + a * 2 + 1034; }
int f035(int a, int b) { return b + a * 2 + 1035; }
int f036(int a, int b) { return a + a * 2 + 1036; }
int f037(int a, int b) { return b + a * 2 + 1037; }
int f038(int a, int b) { return a + a * 2 + 1038; }
int f039(int a, int b) { return b + a * 2 + 1039; }
int f040(int a, int b) { return a + a * 2 + 1040; }
int f041(int a, int b) { return b + a * 2 + 1041; }
int f042(int a, int b) { return a + a * 2 + 1042; }
int f043(int a, int b) { return b + a * 2 + 1043; }
int f044(int a, int b) { return a + a * 2 + 1044; }
int f045(int a, int b) { return b + a * 2 + 1045; }
int f046(int a, int b) { return a + a * 2 + 1046; }
int f047(int a, int b) { return b + a * 2 + 1047; }
int f048(int a, int b) { return a + a * 2 + 1048; }
int f049(int a, int b) { return b + a * 2 + 1049; }
int f050(int a, int b) { return a + a * 2 + 1050; }
int f051(int a, int b) { return b + a * 2 + 1051; }
int f052(int a, int b) { return a + a * 2 + 1052; }
int f053(int a, int b) { return b + a * 2 + 1053; }
int f054(int a, int b) { return a + a * 2 + 1054; }
int f055(int a, int b) { return b + a * 2 + 1055; }
int f056(int a, int b) { return a + a * 2 + 1056; }
int f057(int a, int b) { return b + a * 2 + 1057; }
int f058(int a, int b) { return a + a * 2 + 1058; }
int f059(int a, int b) { return b + a * 2 + 1059; }
int f060(int a, int b) { return a + a * 2 + 1060; }
int f061(int a, int b) { return b + a * 2 + 1061; }
int f062(int a, int b) { return a + a * 2 + 1062; }
int f063(int a, int b) { return b + a * 2 + 1063; }
int f064(int a, int b) { return a + a * 2 + 1064; }
int f065(int a, int b) { return b + a * 2 + 1065; }
int f066(int a, int b) { return a + a * 2 + 1066; }
int f067(int a, int b) { return b + a * 2 + 1067; }
int f068(int a, int b) { return a + a * 2 + 1068; }
int f069(int a, int b) { return b + a * 2 + 1069; }
int f070(int a, int b) { return a + a * 2 + 1070; }
int f071(int a, int b) { return b + a * 2 + 1071; }
int f072(int a, int b) { return a + a * 2 + 1072; }
int f073(int a, int b) { return b + a * 2 + 1073; }
int f074(int a, int b) { return a + a * 2 + 1074; }
int f075(int a, int b) { return b + a * 2 + 1075; }
int f076(int a, int b) { return a + a * 2 + 1076; }
int f077(int a, int b) { return b + a * 2 + 1077; }
int f078(int a, int b) { return a + a * 2 + 1078; }
int f079(int a, int b) { return b + a * 2 + 1079; }
int f080(int a, int b) { return a + a * 2 + 1080; }
int f081(int a, int b) { return b + a * 2 + 1081; }
int f082(int a, int b) { return a + a * 2 + 1082; }
int f083(int a, int b) { return b + a * 2 + 1083; }
int f084(int a, int b) { return a + a * 2 + 1084; }
int f085(int a, int b) { return b + a * 2 + 1085; }
int f086(int a, int b) { return a + a * 2 + 1086; }
int f087(int a, int b) { return b + a * 2 + 1087; }
int f088(int a, int b) { return a + a * 2 + 1088; }
int f089(int a, int b) { return b + a * 2 + 1089; }
int f090(int a, int b) { return a + a * 2 + 1090; }
int f091(int a, int b) { return b + a * 2 + 1091; }
int f092(int a, int b) { return a + a * 2 + 1092; }
int f093(int a, int b) { return b + a * 2 + 1093; }
int f094(int a, int b) { return a + a * 2 + 1094; }
int f095(int a, int b) { return b + a * 2 + 1095; }
int f096(int a, int b) { return a + a * 2 + 1096; }
int f097(int a, int b) { return b + a * 2 + 1097; }
int f098(int a, int b) { return a + a * 2 + 1098; }
int f099(int a, int b) { return b + a * 2 + 1099; }
int f100(int a, int b) { return a + a * 2 + 1100; }

typedef int (*GENERIC_F)(int, int);
typedef void(__stdcall *GENERIC_API)(size_t);

GENERIC_F f_table[100] = {
    f051,f052,f053,f054,f055,f056,f057,f058,
    f059,f060,f061,f062,f063,f064,f065,f066,
    f019,f020,f021,f022,f023,f024,f025,f026,
    f035,f036,f037,f038,f039,f040,f041,f042,
    f075,f076,f077,f078,f079,f080,f081,f082,
    f083,f084,f085,f086,f087,f088,f089,f090,
    f027,f028,f029,f030,f031,f032,f033,f034,
    f043,f044,f045,f046,f047,f048,f049,f050,
    f091,f092,f093,f094,f095,f096,f097,f098,
    f067,f068,f069,f070,f071,f072,f073,f074,
    f011,f012,f013,f014,f015,f016,f017,f018,
    f001,f002,f003,f004,f005,f006,f007,f008,
    f099,f100,f009,f010
};

GENERIC_API a_table[20] = {
    (GENERIC_API)GetModuleHandle, (GENERIC_API)CloseHandle, (GENERIC_API)LoadLibrary, (GENERIC_API)IsProcessorFeaturePresent,
    (GENERIC_API)GetProcessId, (GENERIC_API)OutputDebugString, (GENERIC_API)DisableThreadLibraryCalls, (GENERIC_API)ApplicationRecoveryInProgress,
    (GENERIC_API)DeleteFile, (GENERIC_API)GetFileAttributes, (GENERIC_API)GetFileType, (GENERIC_API)SetSearchPathMode,
    (GENERIC_API)CancelIo, (GENERIC_API)FlushFileBuffers, (GENERIC_API)SetEndOfFile, (GENERIC_API)GetPhysicallyInstalledSystemMemory, 
    (GENERIC_API)RemoveSecureMemoryCacheCallback, (GENERIC_API)SetProcessDEPPolicy, (GENERIC_API)UnmapViewOfFile, (GENERIC_API)HeapDestroy
};

void MegaCallAPI()
{
    LARGE_INTEGER start, stop, freq;
    double timp;
    DWORD curent;    

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < 60000000; i++)
    {
        if ((i % 600000) == 0)
            printf(".");
        curent = (DWORD)__rdtsc();
        a_table[curent % 20](0);
    }
    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("time = %.3lf\n", timp);
}

void MegaCallCBK()
{
    LARGE_INTEGER start, stop, freq;
    double timp;
    DWORD curent;
    int r;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (size_t i = 0; i < 1000000000; i++)
    {
        if ((i % 10000000) == 0)
            printf(".");
        curent = (DWORD)__rdtsc();
        r = f_table[curent % 100](curent, i);
    }
    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("time = %.3lf\n", timp);
}

void MegaJmpCBK()
{
    LARGE_INTEGER start, stop, freq;
    double timp;
    int r;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (size_t i = 0; i < 1000000000; i++)
    {
        if ((i % 10000000) == 0)
            printf(".");
        void* crt = f_table[i % 100];
#ifndef _M_X64
        __asm {
            rdtsc
            push i
            push eax
            push label
            jmp crt
            label :
            add esp, 8
        }
#endif
    }
    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("time = %.3lf\n", timp);
}

int main()
{
    LARGE_INTEGER start, stop, freq;
    double timp;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    printf("\nSqrt(random) loop --> 10M iterations ");
    MegaSqrt();
    printf("\nLocal CBK call loop --> 1G iterations (100 unique) ");
    MegaCallCBK();
    printf("\nLocal CBK jmp loop --> 1G iterations (100 unique) ");
    MegaJmpCBK();
    printf("\nAPI call loop --> 60M iterations (21 unique) ");
    MegaCallAPI();

    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("time = %.3lf\n", timp);

    if (timp > 50 && timp < 80)
        printf("PASSED");
    else
        printf("FAILED");

    return 0;
}


{% endraw %}