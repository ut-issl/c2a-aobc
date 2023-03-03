rem @ECHO OFF       // CIから見やすいように一旦コメントアウト

rem 文字コードをutf-8に
chcp 65001

rem カレントディレクトリをバッチの場所にする
cd /d %~dp0

rem c2a root dirの１つ上へ
cd ..\..\..\..\..\
echo ### Copy C2A to temp dir
if exist ".\c2a_aobc_temp" (
	rmdir /s /q ".\c2a_aobc_temp"
)
xcopy /i /e /q ".\c2a_issl6u_aobc" ".\c2a_aobc_temp"
echo ### Copy S2E build env
xcopy /i /e /q "C:\GitLab-Runner\template\S2E" ".\c2a_issl6u_aobc\S2E\"
echo ### Move C2A to S2E env
move ".\c2a_aobc_temp" ".\c2a_issl6u_aobc\S2E\FlightSW"
cd ".\c2a_issl6u_aobc\S2E\FlightSW\"
rename "c2a_aobc_temp" "c2a_issl6u_aobc"
cd ..\..\..\

echo ### Git clone S2E
cd ".\c2a_issl6u_aobc\S2E\"
git clone --recursive https://gitlab.com/ut_issl/s2e/s2e_6u_aocs.git

echo ### Move ExtLibraries to S2E env
move ".\ExtLibraries" ".\s2e_6u_aocs\ExtLibraries"

echo ### Build with VS2019 using S2E_6U_AOCS
cd ".\s2e_6u_aocs\s2e_6u_aocs"
perl -pi -e "s/^set\(USE_C2A OFF\)/set\(USE_C2A ON\)/g" CMakeLists.txt
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" > nul 2>&1
cmake -G "Visual Studio 16 2019" -A Win32  -DCMAKE_CONFIGURATION_TYPES:STRING="Debug" > nul 2>&1
cmake --build . > nul 2>&1
