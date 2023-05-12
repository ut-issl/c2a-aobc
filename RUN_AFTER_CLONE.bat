@echo off

echo RUN_AFTER_CLONE.bat
echo setting for local repository.
echo .
echo [SET USER HOOKS]
copy /Y .\.gitlab\my_hooks\pre-commit.pl .\.git\hooks\pre-commit
echo done.
echo.
echo [SET UPDATE-INDEX]
rem Do nothing for now.
echo done.
echo.
echo [SET UP DEVELOP ENVIRONMENT FOR SAMPLE AOBC USER]
call :make_link ".\Examples\src\src_common" ".\"
echo done.
echo.
echo The process has been completed.
echo Press any key to exit...
pause >nul
exit


:make_link
if exist %1 (
  rmdir /s /q %1
)
mklink /j /d %1 %2

exit /b
