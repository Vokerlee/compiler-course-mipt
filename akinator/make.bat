if not exist .\src_bak do (
   mkdir .\src_bak
   copy .\src .\src_bak

   for /R "./src" %%f in (*.cpp *h) do (
   .\tools\iconv.exe -f UTF-8 -t CP1251 < "%%f" > "%%f.1251"
   del "%%f"
   copy "%%f.1251" "%%f"
   del "%%f.1251"
)

if not exist .\build do (
   mkdir build
   mkdir build\tools
)

copy .\tools .\build\tools
copy .\akinator_base.txt .\build\akinator_base.txt 

cls
echo "All sources have been converted into CP1251. Please, don't launch this script again."

pause

