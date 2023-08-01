---
title: Resource Limitation
author: redacted
categories: [Transparency, Indirect Evasion Techniques, Resource Limitation]
tags: [transparency]
pin: true
mermaid: true
---


    

## Stalling Code

<span style="color:orange">StallingCode</span>.

ARCH: Windows x86
This example sequence a few loops summing up to 2.1 billion iterations, adding a huge overhead to instrumentation
API instrumentation for DBIs diving into OS libraries, increase this overhead exponentially, while a native execution should be "gone in 60s", instrumentation goes at least 10x higher:

```c++
{% include StallingCode/Source.cpp %}
```



## Memory Exhaustion

<span style="color:orange">APICalls_IncreaseDBI_cache</span>.

ARCH: Windows x86
This example calls a lot of unique APIs (about 4200), to increase the size of the DBI cache to about 1GB in PIN
It also allocates 1GB to hold an already increased memory size before starting to execute APIs
Because it is not possible to physically reduce this increased size by optimisations, one solution would be to allow DBI insturment only the app address-space and skip OS libraries
If the instrumentation does not succeeds, the application will generate an execution-crash

```c++
{% include APICalls_IncreaseDBI_cache/Source.cpp %}
```

<span style="color:orange">MemExhaus</span>.

ARCH: Windows x86
This example allocates a lot of memory and snapshots the largest available memory taken from a native execution and compares it with those oobtained when instrmented
Usually when DBI is used, these limits are easily exceeded

```c++
{% include MemExhaus/Source.cpp %}
```

<span style="color:orange">Thread-Increase-DBICache</span>.

This example creates 30k threads, by relocating a piece of code to dynamically allocated payloads
DBIs either try to sync the executable payloads with the DBI-cache, or to continously allocate different cache portions, which exhausts the DBI-cache limits
A native execution usually don't reach over 200MB memory space, while PIN and DynamoRIO both crash with much higher memory usage

```c++
{% include Thread-Increase-DBICache/Source.cpp %}
```

<span style="color:orange">ThreadCreation</span>.

ARCH: Windows x86
Thia example adds a significant overhead, by creating about 1000 Threads and computing some values the same time.
The overhead is pretty large for most DBIs and negative side effects might take place, like app crash, OS hanging, etc.

```c++
{% include ThreadCreation/Source.cpp %}
```

<span style="color:orange">vmleave</span>.

ARCH: Windows x86/x64
This example scans the entire process memory, searching for DBI vm-leave instruction sequences
Each DBI has its own DBI piece of code, and sometimes this gets updated in incremental versions of a DBI, making new versions to pass, where the previous ones failed
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo

```c++
{% include vmleave/Source.cpp %}
```

