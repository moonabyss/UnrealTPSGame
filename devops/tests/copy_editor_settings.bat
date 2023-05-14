@echo on

call "%~dp0\..\config.bat"

rem copy minimal editor settings
mkdir "%~dp0Saved\Config\WindowsEditor"
copy /Y "devops\tests\templates\EditorPerProjectUserSettings.ini" "Saved\Config\WindowsEditor\EditorPerProjectUserSettings.ini"
