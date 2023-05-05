@echo off
call "%~dp0\devops\config.bat"

devops\misc\generate_project_files.bat "%EnginePath_5.0_InstalledBuild%" "%VersionSelector%" "%ProjectName%"