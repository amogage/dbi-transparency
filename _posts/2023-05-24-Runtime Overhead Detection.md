---
title: Runtime Overhead Detection
author: redacted
categories: [Transparency, Direct Evasion Techniques, Runtime Overhead Detection]
tags: [transparency]
pin: true
mermaid: true
---


    

## Peak Memory Usage

<span style="color:orange">peak_mem_usage</span>.

ARCH: Windows x86
This example measures the memory usage between two calls to symilar large functions, in 3 points
Native execution should have a small difference betwen first two and no difference between the last two, otherwise the execution fails

```c++
{% include peak_mem_usage/Source.cpp %}
```



## Performance Degradation

<span style="color:orange">Detector</span>.

This example exploits a CPU cache behavior commpared to DBIs, executing a behavior on physical CPU and a different one in DBIs and analysis environments

```c++
{% include Detector/Source.cpp %}
```

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

<span style="color:orange">jit-branch</span>.

ARCH: Windows x86
This example measures difference between consecutive branch instrumentation, where usually DBIs add a significant overhead
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include jit-branch/Source.cpp %}
```

<span style="color:orange">jit-lib</span>.

ARCH: Windows x86
This example measures difference between consecutive instrumentation of library load, where usually DBIs add a significant overhead
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18

```c++
{% include jit-lib/Source.cpp %}
```

