@echo off

call "%~dp0\..\config.bat"

rem copy minimal editor settings
mkdir "Saved\Config\WindowsEditor"
copy /Y "devops\tests\templates\EditorPerProjectUserSettings.ini" "Saved\Config\WindowsEditor\EditorPerProjectUserSettings.ini"
