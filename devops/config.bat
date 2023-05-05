@echo off

rem Engine params pc-100
rem set EnginePath_4.27=d:\UE_4.27
rem set EnginePath_5.0=d:\UE_5.0
rem set EnginePath_5.0_InstalledBuild=d:\Epic Games\Builds\UE_5.0_InstalledBuild

rem Engine params abyss
set EnginePath_4.27=c:\UE_4.27
set EnginePath_5.0=c:\UE_5.0
set EnginePath_5.0_InstalledBuild=f:\Builds\UE_5.0_InstalledBuild

set UBTReletivePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTReletivePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=c:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Packaging
set RunUATPath=%EnginePath_5.0_InstalledBuild%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=d:\Projects\Unreal\UnrealTPSGame
set ProjectPureName=TPS
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set DirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set FilesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// Copyright Epic Games, Inc. All Rights Reserved.
set EXTRA_MODULE_NAMES="%ProjectPureName%"
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%\Build\Windows\%ProjectPureName%.exe