---
title: Functional Limitation
author: redacted
categories: [Transparency, Indirect Evasion Techniques, Functional Limitation]
tags: [transparency]
pin: true
mermaid: true
---


    

## Unsupported Assembly Instruction

<span style="color:orange">UnhandledInstrs</span>.

ARCH: Windows x86
This example use a mix of exceptions and possible new or CPU instructions from other architectures to see how DBI handles them
While DynamoRIO passes the test, PIN fails

```c++
{% include UnhandledInstrs/Source.cpp %}
```



## Unsupported Behaviors

<span style="color:orange">FreeUnknown</span>.

ARCH: Windows x86/x64
This example frees memory that is not known: it was not expected on native execution and maybe comes form injected code
Usually when instrumented, the execution crashes

```c++
{% include FreeUnknown/Source.cpp %}
```

