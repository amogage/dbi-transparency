---
title: Code Cache Artifact Detection
author: redacted
categories: [Transparency, Direct Evasion Techniques, Code Cache Artifact Detection]
tags: [transparency]
pin: true
mermaid: true
---


    

## Code Cache Fingerprints

<span style="color:orange">escape_dbi</span>.

ARCH: Windows x86
This example search for references to a constant string, leading to DBI-cache
If DBI-cache refs are found, code is shifted, such that triggering additional executions of the modified code will eventually execute a carefully crafted payload to exit instrumentation
Strings "This code is not instrumented" should not be displayed, if everything works ok

```c++
{% include escape_dbi/Source.cpp %}
```

<span style="color:orange">find-constant</span>.

ARCH: Windows x86/x64
This example attempts to search for duplicates of a constant in memory regions with EXECUTE access rights.
The target memory where duplicates are found are most probable associated with DBI-cache
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include find-constant/Source.cpp %}
```

<span style="color:orange">sok-readcc</span>.

This is a set of 4 examples implemented by SoK:
 - access code cache

```c++
{% include sok-readcc/Source.cpp %}
```



## Instruction Pointer in Unexpected Memory Regions

<span style="color:orange">eipsiginfo</span>.

ARCH: Windows x86/x64
This example use a SEH exception filter to leak te DBI-cache EIP comparing the exception context EIP with FPU-context EIP 
While te EIP is fixed upon handler execution, DBI doesn't seem to properly virtualize it inside the FPU context
On a native execution, they should match
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include eipsiginfo/Source.cpp %}
```

<span style="color:orange">fxsave</span>.

ARCH: Windows x86/x64
This example uses fxsave FPU instruction to save the FPU context and leak the EIP
DBIs most of the time do not correctly virtualize the FPU context
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include fxsave/Source.cpp %}
```

<span style="color:orange">sok-fpu</span>.

This is a set of 4 examples implemented by SoK:
 - leak EIP from FPU context

```c++
{% include sok-fpu/Source.cpp %}
```

<span style="color:orange">sok-readcc</span>.

This is a set of 4 examples implemented by SoK:
 - access code cache

```c++
{% include sok-readcc/Source.cpp %}
```

<span style="color:orange">VB2014_FpuContext1</span>.

ARCH: Windows x86
This example uses fnstenv to leak the DBI-cache EIP, of the last FPU instruction
The EIP in the FPU context should match the buffer where the instruction was executed in the first place
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_FpuContext1/Source.cpp %}
```

<span style="color:orange">VB2014_FpuContext2</span>.

ARCH: Windows x86
This example uses the 16 bit version of fnstenv to leak the DBI-cache low-WORD EIP, of the last FPU instruction
The low-word EIP found in the FPU context should match the low-word of the actual instruction EIP
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_FpuContext2/Source.cpp %}
```



## Incorrect Handling of Self-Modifying Code

<span style="color:orange">SMC</span>.

ARCH: Windows x86/x64
This example tests the possibility of the DBI to instrment self-modifying code
If code is not correct instrumented, the test fails
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include SMC/Source.cpp %}
```

<span style="color:orange">VB2014_ServiceException1</span>.

ARCH: Windows x86
This example fills 512 bytes above current stack pointer (ESP-512) with a specific pattern and then triggers a self modifying instruction
The DBI handling of self modifying instruction may force a backup of the DBI internal state in this stack region, considered to be FREE (return from potential functions using it)
If any byte in this 512 byte memory range gets changed, the test FAILS
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ServiceException1/Source.cpp %}
```



## Unexpected Context

<span style="color:orange">AdditionalThreads</span>.

ARCH: Windows x86
This example counts the number of threads inside the process.
We try to reveal that a DBI or an analysis system use additional threads for various reasons

```c++
{% include AdditionalThreads/Source.cpp %}
```

<span style="color:orange">HeapStack</span>.

ARCH: Windows x86/x64
This example flips the stack temporary to a dynamic allocated buffer, using heap portion as stack
If test fails, execution should crash

```c++
{% include HeapStack/Source.cpp %}
```

<span style="color:orange">ThreadContext</span>.

ARCH: Windows x86
This example cheks if the DBI use multiple address spaces for the same thread.
If thread context is not properly virtualized, at some point we may find the DBI-cache instead of the actual thread address space
Test fails if more than 1 address space relates to the same thread, on multiple thread suspend/resume operations

```c++
{% include ThreadContext/Source.cpp %}
```



## Memory Region Permission Mismatches

<span style="color:orange">nx</span>.

ARCH: Windows x86
Thsi example attempts to execute a piece of code with no EXECUTION rights, triggering an exception
When DBIs do not take kare on access rights, it is easy to execute this code and not trigger the exception, failing the test
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include nx/Source.cpp %}
```

<span style="color:orange">sok-pageguard</span>.

This is a set of 4 examples implemented by SoK:
 - usage of pageguard

```c++
{% include sok-pageguard/Source.cpp %}
```

<span style="color:orange">sok-pageguard_accross</span>.

This is a set of 4 examples implemented by SoK:
 - usage of pageguard accross 2 memory regions

```c++
{% include sok-pageguard_accross/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteData1</span>.

ARCH: Windows x86
This example attempts to execute a RW ret opcode, and it should trigger the exception to pass
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteData1/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteData2</span>.

ARCH: Windows x86
This example allocates a RW payload and adds EXECUTE rights so no exception should trigger, then switches to ReadOnly and exception should trigger
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteData2/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteUnmap1</span>.

ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE and no exception should trigger
On top of that, the memory region gets changed to NOACCESS and exception should be triggered this time
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteUnmap1/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteUnmap2</span>.

ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE and no exception should trigger
On top of that, the memory region gets additionaly PAGE_GUARD access and exception should be triggered this time
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteUnmap2/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteUnmap3</span>.

ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE and PAGE_GUARD and exception should be triggered
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteUnmap3/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteUnmap4</span>.

ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE, RW and PAGE_GUARD and exception should be triggered
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteUnmap4/Source.cpp %}
```

<span style="color:orange">VB2014_ExecuteUnmap5</span>.

ARCH: Windows x86
This example copies a piece of code to a memory region with intial RWE acces and executing the memory region should not trigger any exception
Then it denies access through PAGE_NOACCES and exception should be triggered
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_ExecuteUnmap5/Source.cpp %}
```

<span style="color:orange">VB2014_TransientException1</span>.

ARCH: Windows x86
This example allocates 2 pages with EXECUTE_READ_WRITE access and moves a few instruction at the intersection of these pages
The code is actually sligthly crossing the boundaries of the first page
The second page gets additionally PAGE_GUARD access, such that when code gets executed, the first instruction in the second page should trigger an EXCEPTION_GUARD_PAGE exception
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_TransientException1/Source.cpp %}
```

<span style="color:orange">VB2014_TransientException2</span>.

ARCH: Windows x86
This example allocates an executable memory rgion and copies a few instructions to execute there
Before executing the code, the memory region gets additionally PAGE_GUARD access and it gets executed, triggering an exception
Tis exeception automatically removes the PAGE_GUARD and a second execution should work just fine
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf

```c++
{% include VB2014_TransientException2/Source.cpp %}
```

