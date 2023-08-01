{% raw %}
/*
ARCH: Windows x86
This example calls a lot of unique APIs (about 4200), to increase the size of the DBI cache to about 1GB in PIN
It also allocates 1GB to hold an already increased memory size before starting to execute APIs
Because it is not possible to physically reduce this increased size by optimisations, one solution would be to allow DBI insturment only the app address-space and skip OS libraries
If the instrumentation does not succeeds, the application will generate an execution-crash
*/

#include "apidefs.h"

//perform API calls depending on the actual number of params
bool LowCall(int index, size_t* params, void* proc)
{
    __try {
        switch (index)
        {
            case 0: ((PROC_CAT(P, 0))proc) (); break;
            case 1: ((PROC_CAT(P, 1))proc) (params[0]); break;
            case 2: ((PROC_CAT(P, 2))proc) (params[0], params[1]); break;
            case 3: ((PROC_CAT(P, 3))proc) (params[0], params[1], params[2]); break;
            case 4: ((PROC_CAT(P, 4))proc) (params[0], params[1], params[2], params[3]); break;
            case 5: ((PROC_CAT(P, 5))proc) (params[0], params[1], params[2], params[3], params[4]); break;
            case 6: ((PROC_CAT(P, 6))proc) (params[0], params[1], params[2], params[3], params[4], params[5]); break;
            case 7: ((PROC_CAT(P, 7))proc) (params[0], params[1], params[2], params[3], params[4], params[5], params[6]); break;
            case 8: ((PROC_CAT(P, 8))proc) (params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]); break;
            case 9: ((PROC_CAT(P, 9))proc) (params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8]); break;
            case 10: ((PROC_CAT(P, 10))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]); break;
            case 11: ((PROC_CAT(P, 11))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10]); break;
            case 12: ((PROC_CAT(P, 12))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11]); break;
            case 13: ((PROC_CAT(P, 13))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12]); break;
            case 14: ((PROC_CAT(P, 14))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13]); break;
            case 15: ((PROC_CAT(P, 15))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14]); break;
            case 16: ((PROC_CAT(P, 16))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14], params[15]); break;
            case 17: ((PROC_CAT(P, 17))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14], params[15], params[16]); break;
            case 18: ((PROC_CAT(P, 18))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14], params[15], params[16], params[17]); break;
            case 19: ((PROC_CAT(P, 19))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14], params[15], params[16], params[17], params[18]); break;
            case 20: ((PROC_CAT(P, 20))proc)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11], params[12], params[13], params[14], params[15], params[16], params[17], params[18], params[19]); break;
        }
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {        
        return false;
    }
}

//generate partitions for buffer positions in params, such that some calls may not crash
size_t** GenerateMixPointerPartitions(unsigned char* dummy, int params)
{
    size_t** buffers = (size_t**)malloc((1 << params) * sizeof(size_t*));
    for (int i = 0; i < (1 << params); i++)
    {
        buffers[i] = (size_t*)malloc(params * sizeof(size_t));
        memset(buffers[i], 0, params * sizeof(size_t));
        int bits = i;
        int k = 0;
        int offset = 0;
        while (bits)
        {
            if (bits & 1)
            {
                buffers[i][k] = (size_t)dummy + offset;
                offset += 50;
            }
            k++;
            bits >>= 1;
        }
    }
    return buffers;
}

//high-level execute API
bool ExecuteProc(int params, void* proc)
{
    bool result = false;
    static BYTE dummy_buffer[1024] = { 0 };
    memset(dummy_buffer, 'a', 1023);
    size_t** partitions = GenerateMixPointerPartitions(dummy_buffer, params);
    size_t total = 1 << params;
    for (size_t i = 0; i < total; i++)
    {        
        if (LowCall(params, partitions[i], proc))
        {
            result = true;
            break;
        }
    }    
    for (size_t i = 0; i < total; i++)
        free(partitions[i]);
    free(partitions);
    return result;
}

//execute 4200 APIs
void ExecuteAPIs()
{
    int total = 0;
    for (auto it : dll_names)
    {
        HMODULE hm = LoadLibrary(dll_names[it.first]);
        if (hm)
        {
            for (auto it2 : apis[it.first])
            {
                BYTE* proc = (BYTE*)GetProcAddress(hm, it2.second.api_name);
                if (proc)
                {
                    if (total == 4200)
                        return;
                    if (ExecuteProc(it2.second.param_count, proc))
                    {
                        printf("%d. %s : %s\n", total, it.second, it2.second.api_name);
                        total++;
                    }
                }
            }
        }
    }
}

//General exception filter to block the execution crash
LONG WINAPI TopFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    ExitProcess(0);
}

int main()
{
    //dummy allocation to increase the size of the memory
    BYTE* dummy = (BYTE*)malloc(1024 * 1024 * 1024);
    SetUnhandledExceptionFilter(TopFilter);
    //ExtractAPIs();
    ExecuteAPIs();
    free(dummy);
    printf("PASSED");

    return 0;
}
{% endraw %}