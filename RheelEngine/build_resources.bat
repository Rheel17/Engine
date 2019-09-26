REM /*
REM  * Copyright © 2019 Levi van Rheenen. All rights reserved.
REM  */
 
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
echo /* > resources.h
echo  * Copyright © 2019 Levi van Rheenen. All rights reserved. >> resources.h
echo  */ >> resources.h
echo:>> resources.h
echo // THIS IS A MACHINE GENERATED FILE. DO NOT EDIT^^! >> resources.h
echo #ifndef ___RE_RESOURCES_H >> resources.h
echo #define ___RE_RESOURCES_H >> resources.h
echo:>> resources.h
echo #include ^<unordered_map^> >> resources.h
echo:>> resources.h
echo extern "C" { >> resources.h
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
echo } >> resources.h
echo:>> resources.h
echo struct ___res___ { >> resources.h
echo 	std::unordered_map^<std::string, char *^> resources_pointers { >> resources.h

for /f "delims=" %%i in ('
	"objdump -t resources.o | egrep -oh _binary___.*_start"
') do ( 
	echo 		{ "__%%i", __%%i }, >> resources.h
)

for /f "delims=" %%i in ('
	"objdump -t resources.o | egrep -oh _binary___.*_end"
') do ( 
	echo 		{ "__%%i", __%%i }, >> resources.h
)

echo 	}; >> resources.h
echo }; >> resources.h
echo:>> resources.h
echo #endif >> resources.h
endlocal
