{% raw %}
/*
ARCH: Windows x86/x64
This example queries 2 env-vars related to PIN and DynamoRIO, on native execution they should not be present
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>

#pragma warning (disable:4996)

wchar_t detections[2][100] = {
    L"PIN_CRT_TZDATA",              //PIN specific env var
    L"DYNAMORIO_CONFIGDIR"          //DynamoRIO specific env var
};

int main()
{
    char data[1024];
    wchar_t* env = GetEnvironmentStringsW();
    wchar_t key[100] = { 0 };
    bool detect = false;

    for (const wchar_t* name = env; *name != L'\0'; )
    {
        const wchar_t* equal = wcschr(name, L'=');
        if (equal == name)
        {
            name += wcslen(name) + 1;
            continue;
        }

        memset(key, 0, sizeof(key));
        wcsncpy(key, name, equal - name);
        name += wcslen(name) + 1;

        for (int j = 0; j < 2; j++)
        {
            if (!wcscmp(key, detections[j]))
            {
                detect = true;
                break;
            }
        }
    }

    FreeEnvironmentStrings(env);

    if (detect)
        printf("FAILED");
    else
        printf("PASSED");

    return 0;
}
{% endraw %}