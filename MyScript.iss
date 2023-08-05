#define MyAppName "{code:GetAppName}"
#define MyAppVersion "{code:GetAppVersion}"
#define MyAppPublisher "Stéphane Bressani"
#define MyAppURL "https://www.bressani.dev/"
#define MyAppExeName "OctaSplit.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{72E96521-BD8C-4E3A-9954-64B8C1F282A3}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
; AppVerName combines AppName and AppVersion
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=.\
OutputBaseFilename={#MyAppName} Setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Code]
var
  AppVersion: String;
  AppName: String;

procedure InitializeWizard();
var
  EnvFile: AnsiString;
  EnvLines: TArrayOfString;
  i: Integer;
begin
  LoadStringFromFile(ExpandConstant('{src}\.env'), EnvFile);
  StringChange(EnvFile, #13#10, '\n');
  StringChange(EnvFile, #13, '\n');
  StringChange(EnvFile, #10, '\n');
  EnvLines := StringSplit(EnvFile, '\n');
  for i := 0 to GetArrayLength(EnvLines)-1 do
  begin
    if Copy(EnvLines[i], 1, 12) = 'APP_VERSION=' then
    begin
      AppVersion := Copy(EnvLines[i], 13, MaxInt);
    end
    else if Copy(EnvLines[i], 1, 9) = 'APP_NAME=' then
    begin
      AppName := Copy(EnvLines[i], 10, MaxInt);
    end;
  end;
  WizardForm.Caption := WizardForm.Caption + ' ' + AppVersion;
  { setup section should be updated too }
  WizardForm.SetupCaption := WizardForm.SetupCaption + ' ' + AppName;
end;

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Steee\CLionProjects\2023\OctaSplit\build\OctaSplit_artefacts\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent