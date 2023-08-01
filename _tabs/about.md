---
# the default layout is 'page'
icon: fas fa-info-circle
order: 4
---

The DBI transparency benchmark, as proposed in the "Full transparency in DBI frameworks"[^paper] paper.

The test suite consists of 57 applications. A part of them was collected from other research projects, while the rest were either inspired from the informal description found in the literature, or derived from our experience. As a consequence, we expanded the initial set of tests. Therefore, our test suite has been designed and developed based on four groups of public available proof of concepts, while the last two groups extend some uncovered scenarios:

- Publicly available tests developed for Windows OS:
	+ 12 test created by AVAST in 2014 for [^safemachine];
	+ 1 test for a timing attack described in [^plumerault];
	+ 4 tests described in [^sok].
- 12 publicly available applications developed for other OS (e.g.: Linux), but adapted by us to execute on Windows described in [^pwin], the remaining one (12 out of 13) is Linux-specific and could not be ported to Windows OS because of the differences in the OS kernels.
- 14 tests based on informal descriptions presented in [^dbitaxonomy] and partially on source-code fragments described in [^li2014defeating],[^bypassing],[^coresecurity],[^trumanshow].
- 14 tests designed by us, implementing transparency attacks as an extension of the previous group of tests.



## References

[^paper]: Crăciun, V., Mogage, A., Lucanu, D.: Full transparency in DBI frameworks (2023), https://arxiv.org/abs/2306.13529
[^safemachine]: Martin Hron and Jakub Jermář. SafeMachine: malware needs love, too. Virus Bulletin, 2014
[^plumerault]: Francois Plumerault and Baptiste David. DBI, debuggers, VM: gotta catch them all. Journal of Computer Virology and Hacking Techniques, 17(2):105–117, 2021
[^sok]: SoK: Using dynamic binary instrumentation for security. https://github.com/season-lab/sok-dbi-security.
[^pwin]: Z Zhechev. Security evaluation of dynamic binary instrumentation engines, 2018. Master's thesis, Department of Informatics Technical University of Munich
[^dbitaxonomy]: Ailton Santos Filho, Ricardo J. Rodrıguez, and Eduardo L. Feitosa. Evasion and countermeasures techniques to detect dynamic binary instrumentation frameworks. Digital Threats, 3(2), feb 2022
[^li2014defeating]: Xiaoning Li and Kang Li. Defeating the transparency features of dynamic binary instrumentation. BlackHat USA, 2014
[^bypassing]: Young Bi Lee, Jae Hyuk Suk, and Dong Hoon Lee. Bypassing anti-analysis of commercial protector methods using dbi tools. IEEE Access, 9:7655–7673, 2021
[^trumanshow]: Sun Ke, Li Xiaoning, and Ou Ya. Active detection and escape of dynamic binary instrumentation. Technical report, BlackHat, 2016
[^coresecurity]: Falcón Francisco and Riva Nahuel. Dynamic binary instrumentation frameworks: I know you’re there spying on me. Technical report, Recon, 2012