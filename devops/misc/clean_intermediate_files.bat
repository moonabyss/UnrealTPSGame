@echo off

call "%~dp0\..\config.bat"

pushd %ProjectRoot%
FOR %%f IN (%DirsToRemove%) DO (
    rmdir /s /q %%f
)

FOR %%f IN (%FilesToRemove%) DO (
    del /q %%f
)
popd