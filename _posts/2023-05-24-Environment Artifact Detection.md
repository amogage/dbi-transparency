---
title: Environment Artifact Detection
author: redacted
categories: [Transparency, Direct Evasion Techniques, Environment Artifact Detection]
tags: [transparency]
pin: true
mermaid: true
---


    

## Process Hierarchy

<span style="color:orange">ProcessHirarchy</span>.

ARCH: Windows x86
This example attempts to query the process hirearchy from which the existing process is part of
If parent doesn't match any inside a whitelist, the test fails
The test may be improved watching for child processes that were not created by the existing code

```c++
{% include ProcessHirarchy/Source.cpp %}
```



## Xmode Code

<span style="color:orange">Transit_x64</span>.

ARCH: Windows x86
This example transit from 32 bit to 64 bit to clear the process cmdline buffer in PEB
None of the existing DBIs are able to perform the instrumentation of this CPU transition, as they normally require both 32 and 64 bit dependencies to be loaded.
And there are other negative side-effects.
Not passing this test will result in execution crash.
Credits to "Joseph Sible-Reinstate Monica" : https://stackoverflow.com/questions/72674069/how-can-a-wow64-program-overwrite-its-command-line-arguments-as-seen-by-wmi

```c++
{% include Transit_x64/Source.cpp %}
```

<span style="color:orange">Xmode</span>.

ARCH: Windows x86
This example was build based on wowGrail and exploits the WOW GATEWAY mechanism to execute x64 bit APIs inside an x86 process
No DBI is capable to pass this test as it would mean to use both 32 and 64 bit DBI libraries
Credits to : 
https://github.com/aaaddress1/wowGrail/blob/master/wowGrail/wowGrail.cpp
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo

```c++
{% include Xmode/Source.cpp %}
```



## Incorrect Emulation of Supported Assembly Instructions

<span style="color:orange">enter</span>.

ARCH: Windows x86/x64
Tis example uses enter instruction.
From documentation, tis instruction makes trubles to Valgrind
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include enter/Source.cpp %}
```



## Command-Line Arguments

<span style="color:orange">cmdlnargs</span>.

ARCH: Windows x86/x64
Tis example compares te format of cmdline between native execution and DBI instrmentation
Usually, thare are some changes depending on how DBIs pass the cmdline to the child processes

```c++
{% include cmdlnargs/Source.cpp %}
```



## Process Handles

<span style="color:orange">ProcessHandles</span>.

ARCH: Windows x86
This example query object handles of type HANDLE_PROCESS systemwide and attempts to identify common DBI names in possible parent processes
This could have also been computed only iterating processes, however a parent process may be replaced in PEB to reference a different process, 
   but a handle to a child process inside the parent cannot be easily changed

```c++
{% include ProcessHandles/Source.cpp %}
```



## File Handles

<span style="color:orange">FileHandles</span>.

ARCH: Windows x86
This example query object handles of type HANDLE_FILE systemwide and attempts to identify HANDLES for common DBI names and current working directory in non-standard process names
Usually these handles are present either inside the current process, or inside the parent process
To make the scenario possible, we also iterate process handles

```c++
{% include FileHandles/Source.cpp %}
```



## Event Handles



## Shared Section Handles

<span style="color:orange">SectionHandles</span>.

ARCH: Windows x86
This example query object handles of type HANDLE_SECTION systemwide and attempts to identify common DBI resources inside the shared memory region defined by the section
Both PIN and DynamoRIO use a shared SECTION

```c++
{% include SectionHandles/Source.cpp %}
```



## Signal Masks

<span style="color:orange">Evasion</span>.

ARCH: Windows x86/x64
This example charts te process address space to {start_addr -> {size, access}} using only exceptions, no API at all.
The excessive usage of exceptions has 2 negative side-effects on DBIs
1. a signifficant increase of execution overhead
2. usage of nested exceptions has a high cance of generating execution crashes
Te test is considered PASSED if application does not crash during instrumentation

```c++
{% include Evasion/Source.cpp %}
```

<span style="color:orange">exception_monitor</span>.

ARCH: Windows x86/x64
This example includes a mix of all possible low-level exceptions to test the DBI abbility to deal wit such scenarios
- SEH
- VEH
- Unhandled

```c++
{% include exception_monitor/Source.cpp %}
```



## Fingerprints of DBI-related Binary Programs

<span style="color:orange">AdditionalStacks</span>.

ARCH: Windows x86
This example attempts to search for memory regions having STACK properties and:
    - it passes when all possible stack pointers reference OS specific modules;
    - it fails when some of the stack pointers reference modules or payloads, that are not part of the OS modules

Stack properties:
    - memory does not start with bytes other than 0
    - memory has RW access rights
    - memory has at least 1 string ASCII/UTF16 of size>=10
    - memory is larger than 4KB in size

Whitelist modules:
    "main-module.exe",
    "msvcp...dll",
    "vcruntime...dll",
    "kernel32.dll",
    "kernelbase.dll",
    "ucrtbase.dll",
    "wow64cpu.dll",
    "ntdll.dll"

Each stack found, is traversed and all possible pointers are mapped to their corresponding source memory.
Any other module or payload is actually a DBI injected dll or a DBI code-cache

```c++
{% raw_include AdditionalStacks/Source.cpp %}
```

<span style="color:orange">AdditionalThreads</span>.

ARCH: Windows x86
This example counts the number of threads inside the process.
We try to reveal that a DBI or an analysis system use additional threads for various reasons

```c++
{% include AdditionalThreads/Source.cpp %}
```

<span style="color:orange">Feedback-based</span>.

ARCH: Windows x86
This example charts executable payloads and possible DLLs outside OS and compares the snapshot taken inside the instrumented process and outside of it through a child process
The test pass when there are no payloads and no dfference between inside and outside snapshots and fails otherwise

```c++
{% include Feedback-based/Source.cpp %}
```

<span style="color:orange">LibraryHooks</span>.

ARCH: Windows x86/x64
This example iterates API detour hooks, there should be no hook at all in a native application
If at least 1 hook is found, the test fails

```c++
{% include LibraryHooks/Source.cpp %}
```

<span style="color:orange">mapname</span>.

ARCH: Windows x86
This examnple maps potential known DLLs as part of different DBIs
If any of them is found, the test fails
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include mapname/Source.cpp %}
```

<span style="color:orange">StackTrace</span>.

ARCH: Windows x86
This example creates a string of consecutive return-to OS libraries, based on a stack-trace from stack_top back to stack_bottom and then hashes this string using CRC32
The hash value taken from a native execution is compared to instrumented generated hashes to see if the DBI-cache stack is using the main-thread stack, or an additional stack created by the DBI

```c++
{% include StackTrace/Source.cpp %}
```

<span style="color:orange">unhook</span>.

ARCH: Windows x86
This example unhooks any API hook found in OS libraries and then triggers and exception, forcing the execution of kiUserExceptionDispatcher
While native execution passes the test because no hook exists, any DBI hooking at least 1 API has a high chance to crash
Test should be executed in an OS environment where no EDR/AV sollution exist to inject other supervising modules system-wide

```c++
{% include unhook/Source.cpp %}
```



## Thread Local Storage Presence

<span style="color:orange">TLSPresence</span>.

ARCH: Windows x86
This example fingerprints TLS memory regions specific to PIN and DynamoRIO, based on constant strings
Both PIN and DynamoRIO fails
Credits to:
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo

```c++
{% include TLSPresence/Source.cpp %}
```



## Environment Variables

<span style="color:orange">env_var</span>.

ARCH: Windows x86/x64
This example queries 2 env-vars related to PIN and DynamoRIO, on native execution they should not be present
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include env_var/Source.cpp %}
```

