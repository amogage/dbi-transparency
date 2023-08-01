{% raw %}
/*
ARCH: Windows x86
Thia example adds a significant overhead, by creating about 1000 Threads and computing some values the same time.
The overhead is pretty large for most DBIs and negative side effects might take place, like app crash, OS hanging, etc.
*/

#include <Windows.h>
#include <stdio.h>
#include <intrin.h>

DWORD WINAPI t001(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t002(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t003(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t004(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t005(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t006(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t007(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t008(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t009(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t010(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t011(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t012(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t013(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t014(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t015(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t016(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t017(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t018(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t019(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t020(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t021(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t022(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t023(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t024(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t025(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t026(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t027(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t028(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t029(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t030(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t031(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t032(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t033(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t034(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t035(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t036(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t037(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t038(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t039(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t040(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t041(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t042(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t043(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t044(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t045(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t046(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t047(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t048(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t049(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t050(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t051(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t052(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t053(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t054(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t055(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t056(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t057(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t058(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t059(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t060(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t061(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t062(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t063(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t064(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t065(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t066(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t067(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t068(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t069(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t070(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t071(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t072(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t073(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t074(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t075(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t076(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t077(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t078(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t079(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t080(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t081(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t082(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t083(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t084(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t085(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t086(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t087(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t088(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t089(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t090(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t091(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t092(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t093(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t094(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t095(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t096(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t097(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t098(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t099(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t100(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t101(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t102(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t103(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t104(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t105(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t106(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t107(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t108(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t109(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t110(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t111(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t112(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t113(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t114(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t115(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t116(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t117(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t118(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t119(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t120(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t121(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t122(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t123(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t124(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t125(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t126(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t127(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t128(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t129(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t130(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t131(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t132(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t133(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t134(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t135(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t136(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t137(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t138(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t139(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t140(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t141(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t142(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t143(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t144(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t145(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t146(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t147(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t148(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t149(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t150(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t151(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t152(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t153(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t154(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t155(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t156(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t157(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t158(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t159(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t160(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t161(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t162(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t163(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t164(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t165(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t166(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t167(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t168(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t169(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t170(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t171(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t172(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t173(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t174(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t175(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t176(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t177(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t178(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t179(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t180(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t181(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t182(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t183(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t184(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t185(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t186(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t187(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t188(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t189(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t190(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t191(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t192(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t193(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t194(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t195(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t196(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t197(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t198(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t199(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t200(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t201(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t202(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t203(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t204(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t205(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t206(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t207(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t208(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t209(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t210(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t211(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t212(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t213(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t214(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t215(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t216(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t217(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t218(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t219(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t220(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t221(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t222(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t223(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t224(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t225(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t226(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t227(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t228(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t229(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t230(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t231(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t232(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t233(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t234(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t235(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t236(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t237(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t238(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t239(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t240(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t241(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t242(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t243(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t244(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t245(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t246(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t247(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t248(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t249(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t250(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t251(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t252(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t253(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t254(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t255(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t256(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t257(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t258(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t259(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t260(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t261(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t262(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t263(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t264(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t265(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t266(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t267(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t268(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t269(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t270(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t271(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t272(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t273(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t274(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t275(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t276(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t277(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t278(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t279(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t280(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t281(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t282(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t283(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t284(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t285(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t286(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t287(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t288(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t289(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t290(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t291(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t292(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t293(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t294(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t295(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t296(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t297(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t298(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t299(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t300(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t301(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t302(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t303(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t304(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t305(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t306(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t307(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t308(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t309(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t310(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t311(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t312(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t313(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t314(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t315(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t316(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t317(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t318(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t319(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t320(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t321(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t322(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t323(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t324(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t325(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t326(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t327(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t328(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t329(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t330(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t331(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t332(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t333(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t334(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t335(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t336(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t337(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t338(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t339(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t340(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t341(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t342(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t343(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t344(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t345(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t346(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t347(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t348(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t349(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t350(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t351(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t352(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t353(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t354(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t355(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t356(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t357(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t358(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t359(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t360(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t361(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t362(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t363(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t364(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t365(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t366(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t367(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t368(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t369(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t370(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t371(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t372(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t373(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t374(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t375(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t376(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t377(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t378(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t379(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t380(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t381(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t382(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t383(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t384(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t385(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t386(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t387(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t388(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t389(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t390(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t391(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t392(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t393(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t394(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t395(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t396(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t397(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t398(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t399(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t400(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t401(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t402(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t403(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t404(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t405(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t406(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t407(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t408(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t409(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t410(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t411(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t412(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t413(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t414(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t415(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t416(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t417(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t418(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t419(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t420(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t421(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t422(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t423(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t424(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t425(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t426(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t427(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t428(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t429(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t430(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t431(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t432(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t433(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t434(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t435(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t436(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t437(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t438(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t439(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t440(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t441(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t442(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t443(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t444(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t445(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t446(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t447(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t448(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t449(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t450(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t451(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t452(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t453(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t454(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t455(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t456(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t457(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t458(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t459(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t460(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t461(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t462(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t463(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t464(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t465(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t466(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t467(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t468(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t469(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t470(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t471(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t472(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t473(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t474(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t475(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t476(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t477(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t478(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t479(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t480(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t481(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t482(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t483(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t484(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t485(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t486(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t487(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t488(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t489(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t490(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t491(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t492(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t493(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t494(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t495(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t496(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t497(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t498(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t499(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t500(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t501(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t502(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t503(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t504(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t505(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t506(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t507(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t508(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t509(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t510(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t511(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t512(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t513(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t514(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t515(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t516(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t517(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t518(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t519(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t520(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t521(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t522(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t523(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t524(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t525(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t526(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t527(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t528(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t529(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t530(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t531(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t532(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t533(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t534(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t535(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t536(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t537(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t538(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t539(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t540(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t541(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t542(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t543(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t544(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t545(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t546(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t547(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t548(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t549(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t550(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t551(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t552(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t553(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t554(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t555(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t556(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t557(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t558(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t559(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t560(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t561(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t562(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t563(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t564(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t565(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t566(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t567(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t568(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t569(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t570(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t571(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t572(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t573(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t574(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t575(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t576(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t577(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t578(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t579(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t580(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t581(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t582(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t583(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t584(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t585(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t586(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t587(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t588(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t589(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t590(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t591(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t592(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t593(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t594(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t595(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t596(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t597(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t598(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t599(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t600(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t601(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t602(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t603(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t604(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t605(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t606(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t607(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t608(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t609(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t610(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t611(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t612(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t613(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t614(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t615(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t616(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t617(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t618(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t619(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t620(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t621(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t622(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t623(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t624(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t625(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t626(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t627(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t628(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t629(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t630(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t631(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t632(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t633(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t634(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t635(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t636(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t637(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t638(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t639(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t640(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t641(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t642(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t643(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t644(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t645(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t646(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t647(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t648(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t649(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t650(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t651(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t652(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t653(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t654(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t655(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t656(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t657(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t658(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t659(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t660(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t661(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t662(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t663(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t664(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t665(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t666(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t667(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t668(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t669(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t670(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t671(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t672(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t673(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t674(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t675(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t676(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t677(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t678(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t679(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t680(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t681(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t682(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t683(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t684(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t685(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t686(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t687(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t688(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t689(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t690(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t691(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t692(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t693(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t694(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t695(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t696(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t697(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t698(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t699(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t700(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t701(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t702(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t703(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t704(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t705(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t706(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t707(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t708(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t709(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t710(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t711(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t712(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t713(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t714(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t715(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t716(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t717(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t718(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t719(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t720(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t721(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t722(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t723(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t724(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t725(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t726(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t727(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t728(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t729(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t730(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t731(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t732(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t733(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t734(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t735(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t736(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t737(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t738(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t739(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t740(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t741(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t742(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t743(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t744(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t745(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t746(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t747(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t748(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t749(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t750(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t751(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t752(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t753(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t754(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t755(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t756(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t757(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t758(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t759(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t760(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t761(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t762(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t763(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t764(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t765(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t766(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t767(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t768(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t769(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t770(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t771(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t772(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t773(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t774(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t775(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t776(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t777(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t778(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t779(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t780(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t781(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t782(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t783(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t784(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t785(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t786(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t787(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t788(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t789(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t790(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t791(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t792(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t793(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t794(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t795(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t796(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t797(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t798(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t799(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t800(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t801(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t802(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t803(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t804(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t805(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t806(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t807(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t808(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t809(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t810(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t811(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t812(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t813(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t814(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t815(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t816(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t817(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t818(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t819(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t820(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t821(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t822(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t823(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t824(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t825(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t826(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t827(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t828(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t829(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t830(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t831(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t832(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t833(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t834(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t835(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t836(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t837(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t838(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t839(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t840(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t841(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t842(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t843(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t844(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t845(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t846(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t847(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t848(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t849(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t850(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t851(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t852(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t853(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t854(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t855(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t856(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t857(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t858(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t859(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t860(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t861(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t862(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t863(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t864(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t865(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t866(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t867(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t868(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t869(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t870(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t871(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t872(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t873(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t874(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t875(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t876(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t877(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t878(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t879(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t880(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t881(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t882(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t883(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t884(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t885(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t886(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t887(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t888(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t889(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t890(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t891(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t892(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t893(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t894(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t895(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t896(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t897(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t898(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t899(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t900(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t901(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t902(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t903(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t904(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t905(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t906(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t907(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t908(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t909(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t910(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t911(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t912(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t913(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t914(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t915(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t916(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t917(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t918(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t919(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t920(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t921(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t922(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t923(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t924(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t925(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t926(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t927(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t928(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t929(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t930(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t931(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t932(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t933(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t934(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t935(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t936(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t937(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t938(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t939(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t940(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t941(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t942(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t943(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t944(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t945(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t946(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t947(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t948(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t949(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t950(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t951(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t952(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t953(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t954(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t955(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t956(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t957(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t958(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t959(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t960(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t961(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t962(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t963(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t964(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t965(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t966(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t967(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t968(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t969(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t970(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t971(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t972(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t973(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t974(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t975(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t976(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t977(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t978(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t979(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t980(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t981(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t982(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t983(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t984(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t985(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t986(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t987(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t988(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t989(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t990(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t991(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t992(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t993(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t994(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t995(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t996(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t997(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t998(void* param) { ((DWORD*)param)[0]++; return 0; }
DWORD WINAPI t999(void* param) { ((DWORD*)param)[0]++; return 0; }

bool flag = false;

DWORD WINAPI th1(void* param) 
{ 
    HANDLE f = *(HANDLE*)param;
    char word[21] = { 0 };
    DWORD write = 0;
    for (int i = 0; i < 1000; i++)
    {
        memset(word, 0, 21);
        word[0] = '1';
        word[1] = '.';
        word[2] = ' ';
        for (int j = 3; j < 16; j++)
            word[j] = rand() % 26 + 'A';
        word[strlen(word)] = '\n';
        if (!flag)
        {
            flag = true;
            WriteFile(f, word, strlen(word), &write, 0);
        }
        flag = false;
    }
    return 0; 
}

DWORD WINAPI th2(void* param)
{
    HANDLE f = *(HANDLE*)param;
    char word[21] = { 0 };
    DWORD write = 0;
    for (int i = 0; i < 1000; i++)
    {
        memset(word, 0, 21);
        word[0] = '2';
        word[1] = '.';
        word[2] = ' ';
        for (int j = 3; j < 16; j++)
            word[j] = rand() % 10 + '0';
        word[strlen(word)] = '\n';
        if (!flag)
        {
            flag = true;
            WriteFile(f, word, strlen(word), &write, 0);
        }
        flag = false;
    }
    return 0;
}

int main()
{
    PTHREAD_START_ROUTINE threads[999] = {
        t001,t002,t003,t004,t005,t006,t007,t008,t009,t010,t011,t012,t013,t014,t015,t016,t017,t018,t019,t020,t021,t022,t023,t024,t025,t026,t027,t028,t029,t030,
        t031,t032,t033,t034,t035,t036,t037,t038,t039,t040,t041,t042,t043,t044,t045,t046,t047,t048,t049,t050,t051,t052,t053,t054,t055,t056,t057,t058,t059,t060,
        t061,t062,t063,t064,t065,t066,t067,t068,t069,t070,t071,t072,t073,t074,t075,t076,t077,t078,t079,t080,t081,t082,t083,t084,t085,t086,t087,t088,t089,t090,
        t091,t092,t093,t094,t095,t096,t097,t098,t099,t100,t101,t102,t103,t104,t105,t106,t107,t108,t109,t110,t111,t112,t113,t114,t115,t116,t117,t118,t119,t120,
        t121,t122,t123,t124,t125,t126,t127,t128,t129,t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t140,t141,t142,t143,t144,t145,t146,t147,t148,t149,t150,
        t151,t152,t153,t154,t155,t156,t157,t158,t159,t160,t161,t162,t163,t164,t165,t166,t167,t168,t169,t170,t171,t172,t173,t174,t175,t176,t177,t178,t179,t180,
        t181,t182,t183,t184,t185,t186,t187,t188,t189,t190,t191,t192,t193,t194,t195,t196,t197,t198,t199,t200,t201,t202,t203,t204,t205,t206,t207,t208,t209,t210,
        t211,t212,t213,t214,t215,t216,t217,t218,t219,t220,t221,t222,t223,t224,t225,t226,t227,t228,t229,t230,t231,t232,t233,t234,t235,t236,t237,t238,t239,t240,
        t241,t242,t243,t244,t245,t246,t247,t248,t249,t250,t251,t252,t253,t254,t255,t256,t257,t258,t259,t260,t261,t262,t263,t264,t265,t266,t267,t268,t269,t270,
        t271,t272,t273,t274,t275,t276,t277,t278,t279,t280,t281,t282,t283,t284,t285,t286,t287,t288,t289,t290,t291,t292,t293,t294,t295,t296,t297,t298,t299,t300,
        t301,t302,t303,t304,t305,t306,t307,t308,t309,t310,t311,t312,t313,t314,t315,t316,t317,t318,t319,t320,t321,t322,t323,t324,t325,t326,t327,t328,t329,t330,
        t331,t332,t333,t334,t335,t336,t337,t338,t339,t340,t341,t342,t343,t344,t345,t346,t347,t348,t349,t350,t351,t352,t353,t354,t355,t356,t357,t358,t359,t360,
        t361,t362,t363,t364,t365,t366,t367,t368,t369,t370,t371,t372,t373,t374,t375,t376,t377,t378,t379,t380,t381,t382,t383,t384,t385,t386,t387,t388,t389,t390,
        t391,t392,t393,t394,t395,t396,t397,t398,t399,t400,t401,t402,t403,t404,t405,t406,t407,t408,t409,t410,t411,t412,t413,t414,t415,t416,t417,t418,t419,t420,
        t421,t422,t423,t424,t425,t426,t427,t428,t429,t430,t431,t432,t433,t434,t435,t436,t437,t438,t439,t440,t441,t442,t443,t444,t445,t446,t447,t448,t449,t450,
        t451,t452,t453,t454,t455,t456,t457,t458,t459,t460,t461,t462,t463,t464,t465,t466,t467,t468,t469,t470,t471,t472,t473,t474,t475,t476,t477,t478,t479,t480,
        t481,t482,t483,t484,t485,t486,t487,t488,t489,t490,t491,t492,t493,t494,t495,t496,t497,t498,t499,t500,t501,t502,t503,t504,t505,t506,t507,t508,t509,t510,
        t511,t512,t513,t514,t515,t516,t517,t518,t519,t520,t521,t522,t523,t524,t525,t526,t527,t528,t529,t530,t531,t532,t533,t534,t535,t536,t537,t538,t539,t540,
        t541,t542,t543,t544,t545,t546,t547,t548,t549,t550,t551,t552,t553,t554,t555,t556,t557,t558,t559,t560,t561,t562,t563,t564,t565,t566,t567,t568,t569,t570,
        t571,t572,t573,t574,t575,t576,t577,t578,t579,t580,t581,t582,t583,t584,t585,t586,t587,t588,t589,t590,t591,t592,t593,t594,t595,t596,t597,t598,t599,t600,
        t601,t602,t603,t604,t605,t606,t607,t608,t609,t610,t611,t612,t613,t614,t615,t616,t617,t618,t619,t620,t621,t622,t623,t624,t625,t626,t627,t628,t629,t630,
        t631,t632,t633,t634,t635,t636,t637,t638,t639,t640,t641,t642,t643,t644,t645,t646,t647,t648,t649,t650,t651,t652,t653,t654,t655,t656,t657,t658,t659,t660,
        t661,t662,t663,t664,t665,t666,t667,t668,t669,t670,t671,t672,t673,t674,t675,t676,t677,t678,t679,t680,t681,t682,t683,t684,t685,t686,t687,t688,t689,t690,
        t691,t692,t693,t694,t695,t696,t697,t698,t699,t700,t701,t702,t703,t704,t705,t706,t707,t708,t709,t710,t711,t712,t713,t714,t715,t716,t717,t718,t719,t720,
        t721,t722,t723,t724,t725,t726,t727,t728,t729,t730,t731,t732,t733,t734,t735,t736,t737,t738,t739,t740,t741,t742,t743,t744,t745,t746,t747,t748,t749,t750,
        t751,t752,t753,t754,t755,t756,t757,t758,t759,t760,t761,t762,t763,t764,t765,t766,t767,t768,t769,t770,t771,t772,t773,t774,t775,t776,t777,t778,t779,t780,
        t781,t782,t783,t784,t785,t786,t787,t788,t789,t790,t791,t792,t793,t794,t795,t796,t797,t798,t799,t800,t801,t802,t803,t804,t805,t806,t807,t808,t809,t810,
        t811,t812,t813,t814,t815,t816,t817,t818,t819,t820,t821,t822,t823,t824,t825,t826,t827,t828,t829,t830,t831,t832,t833,t834,t835,t836,t837,t838,t839,t840,
        t841,t842,t843,t844,t845,t846,t847,t848,t849,t850,t851,t852,t853,t854,t855,t856,t857,t858,t859,t860,t861,t862,t863,t864,t865,t866,t867,t868,t869,t870,
        t871,t872,t873,t874,t875,t876,t877,t878,t879,t880,t881,t882,t883,t884,t885,t886,t887,t888,t889,t890,t891,t892,t893,t894,t895,t896,t897,t898,t899,t900,
        t901,t902,t903,t904,t905,t906,t907,t908,t909,t910,t911,t912,t913,t914,t915,t916,t917,t918,t919,t920,t921,t922,t923,t924,t925,t926,t927,t928,t929,t930,
        t931,t932,t933,t934,t935,t936,t937,t938,t939,t940,t941,t942,t943,t944,t945,t946,t947,t948,t949,t950,t951,t952,t953,t954,t955,t956,t957,t958,t959,t960,
        t961,t962,t963,t964,t965,t966,t967,t968,t969,t970,t971,t972,t973,t974,t975,t976,t977,t978,t979,t980,t981,t982,t983,t984,t985,t986,t987,t988,t989,t990,
        t991,t992,t993,t994,t995,t996,t997,t998,t999
    };
    DWORD feedback[999] = { 0 };
    DWORD total = 0;
    LARGE_INTEGER start, stop, freq;
    HANDLE thrds[999] = { 0 };
    double timp;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    for (int i = 0; i < 999; i++)
        thrds[i] = CreateThread(0, 0, threads[i], &feedback[i], 0, 0);
    WaitForMultipleObjects(999, thrds, TRUE, INFINITE);
    for (int i = 0; i < 999; i++)
        total += feedback[i];
    printf("counter: %d\n", total);

    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("timp = %.3lf\n", timp);

    srand(GetTickCount());
    HANDLE f = CreateFile("test.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL ,0);
    thrds[0] = CreateThread(0, 0, th1, &f, 0, 0);
    thrds[1] = CreateThread(0, 0, th2, &f, 0, 0);
    WaitForMultipleObjects(2, thrds, TRUE, INFINITE);
    CloseHandle(f);

    f = CreateFile("test.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD size = GetFileSize(f, 0);
    DWORD read = 0;
    char* buffer = (char*)malloc(size + 1);
    memset(buffer, 0, size + 1);
    ReadFile(f, buffer, size, &read, 0);
    CloseHandle(f);
    int counter = 0;
    char* pos = 0;
    char* search = buffer;
    do {
        pos = strstr(search, "\n");
        search = pos + 1;
        if (pos)
            counter++;
    } while (pos);
    printf("total: %d\n", counter);    

    if (timp < 0.2 && counter > 900 )
        printf("PASSED");
    else
        if (timp > 1.2 || counter < 900)
            printf("FAILED");

    //free(buffer);

	return 0;
}
{% endraw %}