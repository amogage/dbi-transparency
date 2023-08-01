# Tested with [Python 3.7.0 (v3.7.0:1bf9cc5093, Jun 27 2018, 04:59:51) [MSC v.1914 64 bit (AMD64)] on win32]
# Compiler: Microsoft Visual Studio 2022 Community
import os, subprocess, sys, shutil, psutil, json

MS_BUILD = r'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe'             
CURR_DIR = os.getcwd()
PLATFORM = '/property:Platform=x86'
CONFIG = '/p:Configuration=Release'
OUTPUT = '/p:OutDir=..\\release'

if __name__ == '__main__':
    vcxproj_paths = []
    for root, dirs, files in os.walk('.'):
        for file in files:
            if file.endswith('.vcxproj'):
                current_path = os.path.join(root, file)
                vcxproj_paths += [current_path]

    index = 1    
    print('Building DBI Transparency tests ...')
    for project in vcxproj_paths:        
        subprocess.run([MS_BUILD, project, '/p:DebugSymbols=false', '/p:DebugType=None', '/p:CL_MPCount=16', CONFIG, PLATFORM, OUTPUT], creationflags=subprocess.CREATE_NO_WINDOW)            
        project_fname = project[project.rfind('\\')+1:-8]
        output = str(index) + '. ' + project_fname
        if os.path.exists('release\\' + project_fname + '.exe'):
            print(f"{output:<40}{'[OK]':>5}")
        else:
            print(f"{output:<40}{'[ER]':>5}")
        os.remove('release\\' + project_fname + '.pdb')
        index += 1

    print('Executing DBI Transparency tests with provided DBI frameworks / tools ...')
    test_results = {}
    with open('dbi_configs.json', 'r') as f:
        data = json.load(f)        
        os.chdir('release')
        for i, (cfg, dbi_path) in enumerate(data.items()):
            if cfg not in test_results:
                test_results[cfg] = {}
            print(f"Executing config: [{cfg}]")
            tests = [f for f in os.listdir('.') if f.endswith('.exe') and 'hello' not in f]
            for test in tests:
                p = subprocess.Popen([dbi_path, '--', test], stdout=subprocess.PIPE, creationflags=subprocess.CREATE_NO_WINDOW)
                output = str(p.communicate()[0])[-7:-1]
                if output == 'FAILED':               
                    print(f"{test[:-4]:<40}{'0':>5}")
                    test_results[cfg][test[:-4]] = 0
                elif output == 'PASSED':
                    print(f"{test[:-4]:<40}{'1':>5}")
                    test_results[cfg][test[:-4]] = 1
                else:
                    print(f"{test[:-4]:<40}{'-1':>5}")
                    test_results[cfg][test[:-4]] = -1
        os.chdir('..')

    