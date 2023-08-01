#pragma once

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <fstream>
using namespace std;

#pragma warning (disable:4996)

#define PROC_CAT(A, B) A ## B

struct APIInfo {
    const char* api_name;
    int param_count;
};

extern map<unsigned long long, const char*> dll_names;
extern map<unsigned long long, map<unsigned long long, APIInfo>> apis;

typedef void(__stdcall* P0)(void);
typedef void(__stdcall* P1)(size_t);
typedef void(__stdcall* P2)(size_t, size_t);
typedef void(__stdcall* P3)(size_t, size_t, size_t);
typedef void(__stdcall* P4)(size_t, size_t, size_t, size_t);
typedef void(__stdcall* P5)(size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P6)(size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P7)(size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P8)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P9)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P10)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P11)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P12)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P13)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P14)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P15)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P16)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P17)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P18)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P19)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
typedef void(__stdcall* P20)(size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t);
