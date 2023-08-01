---
title: Just-in-Time Compiler Detection
author: redacted
categories: [Transparency, Direct Evasion Techniques, Just-in-Time Compiler Detection]
tags: [transparency]
pin: true
mermaid: true
---


    

## System Library Hooks

<span style="color:orange">LibraryHooks</span>.

ARCH: Windows x86/x64.  
This example iterates API detour hooks, there should be no hook at all in a native application.
If at least 1 hook is found, the test fails.

```c++
{% include LibraryHooks/Source.cpp %}
```



## Excessive Number of Full Access Memory Pages

<span style="color:orange">pageperm</span>.

ARCH: Windows x86
This example counts how many memory chunks exist with RWE poermission rights
There should be none to pass the test, otherwise these are some kind of paylaods, specific to DBIs but also to other kind of code injection
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include pageperm/Source.cpp %}
```



## Common API Calls

<span style="color:orange">APIMonitor</span>.

ARCH: Windows x86/x64 (x64 not fully working, some adjustments should pe performed on ReplicateFunction() )
This example attempts to hook ntdll:NtAllocateVirtualMemory API in order to count the total number of allocated memory regions
The allocations count is snapshoted at native execution and then compared to any other execution, the difference should not exceed a maximum allowed
Possible wayS to bypass this test would be: 
 - to know how much memory is required before the start of the instrumentation
 - decrease the granularity of incremental allocated chunks in DBI-cache
 - allocate memory using syscalls instead of APIs

```c++
{% include APIMonitor/Source.cpp %}
```

