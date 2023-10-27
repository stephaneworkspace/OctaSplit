#define MyAppName "OctaSplit"
#define MyAppVersion "1.0.1"
#define MyAppPublisher "Stéphane Bressani"
#define MyAppURL "https://www.bressani.dev/"
#define MyAppExeName "OctaSplit.exe"

[Setup]
AppId={{72E96521-BD8C-4E3A-9954-64B8C1F282A3}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
; AppVerName combines AppName and AppVersion
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf32}\bressani.dev\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=.\
;OutputBaseFilename={#MyAppName} Setup
OutputBaseFilename=OctaSplit-1.0.1-Setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Code\OctaSplit\build\OctaSplit_artefacts\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Files]
Source: "C:\Code\OctaSplit\build\OctaSplit_artefacts\Release\.env"; DestDir: "{app}"; Flags: ignoreversion

[Files]
 Source: "C:\Code\OctaSplit\build\OctaSplit_artefacts\Release\Assets\*"; DestDir: "{app}\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent