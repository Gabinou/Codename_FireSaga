; Makes a Windows installer
;   -> iscc winstaller.iss

[Setup]
AppName=Firesaga
AppVersion=0.1
WizardStyle=modern
DefaultDirName={autopf}\Firesaga
DefaultGroupName=Firesaga
UninstallDisplayIcon={app}\main.exe
Compression=lzma2
SolidCompression=yes
OutputBaseFilename=Installer_Firesaga
OutputDir=.

[Files]
Source: "main.exe"; DestDir: "{app}"
Source: "Threat.contrast-black.ico"; DestDir: "{app}"
Source: "readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{commondesktop}\Firesaga"; Filename: "{app}\main.exe";  IconFilename: "{app}\Threat.contrast-black.ico";


