@echo off
setlocal EnableDelayedExpansion

REM create the resources.o file

set files=

for /f %%i in ('
	forfiles /p Resources /s /c "cmd /c if @isdir==FALSE echo @relpath"
') do (
	set files=!files! %%i
)

cd Resources
ld -o ../../Build/Resources/resources.o -b binary %files%

REM create resources.h file

cd ..\..\Build\Resources
echo #ifndef ___RE_RESOURCES_H > resources.h
echo #define ___RE_RESOURCES_H >> resources.h
echo:>> resources.h
echo #define RESOURCE_START(resource) ___binary___ ## resource ## _start >> resources.h
echo #define RESOURCE_END(resource) ___binary___ ## resource ## _end >> resources.h
echo:>> resources.h
echo #ifdef __cplusplus >> resources.h
echo extern "C" { >> resources.h
echo #endif >> resources.h
echo:>> resources.h

for /f "delims=" %%i in ('
	"objdump -t resources.o | egrep -oh _binary___.*_start"
') do ( 
	echo extern char __%%i[] asm("%%i"^); >> resources.h
)

for /f "delims=" %%i in ('
	"objdump -t resources.o | egrep -oh _binary___.*_end"
') do ( 
	echo extern char __%%i[] asm("%%i"^); >> resources.h
)

echo:>> resources.h
echo #ifdef __cplusplus >> resources.h
echo } >> resources.h
echo #endif >> resources.h
echo:>> resources.h
echo #endif >> resources.h
endlocal
