@echo on

call "%~dp0\..\config.bat"

rem copy minimal editor settings
mkdir "%~dp0Saved\Config\WindowsEditor"
copy /Y "%~dp0devops\tests\templates\EditorPerProjectUserSettings.ini" "%~dp0Saved\Config\WindowsEditor\EditorPerProjectUserSettings.ini"
