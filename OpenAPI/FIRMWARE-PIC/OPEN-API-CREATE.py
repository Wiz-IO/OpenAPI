import os
from os.path import join
import time

path = os.path.dirname(__file__)

a = open( join(path, "OPEN-API-SHARED.txt"), "r") # list functions
Lines = a.readlines()
a.close()

def do_mkdir(path, name):
    dir = join(path, name)
    if False == os.path.isdir( dir ):
        try:
            os.mkdir(dir)
        except OSError:
            print ("[ERROR] Creation of the directory %s failed" % dir)
            exit(1)
    return dir

do_mkdir( path, "openapi-lib")

# SOURCE FILE
do_mkdir( path, "openapi-lib")
FC = open( join(path, "openapi-lib", "OpenAPI-shared.c"), "w")
FC.write("""/*
    OpenAPI [ %s ] Georgi Angelov
    Position Independent Code - Shared Library
*/\n\n""" % time.strftime("%d.%m.%Y"))

# HEADER FILE
FH = open( join(path, "src", "openapi", "OpenAPI-shared.h"), "w")
FH.write("""/*
    OpenAPI [ %s ] Georgi Angelov
    Position Independent Code - Shared Library
*/\n\n""" % time.strftime("%d.%m.%Y"))
COUNTER = 0 
for L in Lines:
    L = L.replace("\r", "").replace("\n", "").strip()
    L = L.replace("\t", ",")
    if L == '' or L.startswith('/') or L.startswith(' ') or L.startswith("\n") or L.startswith("\r"): continue
    L = L.split(",")
    if "API-LIST-END" in L: 
        break;
    FC.write( 'void %s(void){}\n' % ( L[0] ) )
    FH.write( '    {"%s", %s},\n' % ( L[0], L[0] ) )
    COUNTER +=1
print("OpenAPI use %d functions" % COUNTER)
FC.write("\n\n/* Shared Functions : %d */\n" % COUNTER)

# MAKE FILE
FM = open( join(path, "openapi-lib", "makefile"), "w")
FM.write("""CC_OPTIONS=-mcpu=cortex-m4 -mthumb -msingle-pic-base -fPIC -Wall
GCC_PATH=C:/Users/1124/.platformio/packages/toolchain-gccarmnoneeabi/bin/
\nall:
\t@echo   * Creating OpenAPI PIC Library
\t$(GCC_PATH)arm-none-eabi-gcc $(CC_OPTIONS) -g -Os -c OpenAPI-shared.c -o OpenAPI-shared.o
\t$(GCC_PATH)arm-none-eabi-gcc -shared -Wl,-soname,libopenapi.a -nostdlib -o libopenapi.a OpenAPI-shared.o
""")
FM.close()
FC.close()
FH.close()

