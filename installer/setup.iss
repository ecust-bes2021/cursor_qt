; ============================================
; Inno Setup 安装脚本 - qt_tool_template
; ============================================
; [AUTO-GENERATED] 此文件由 CMake 从 setup.iss.in 自动生成
; 请勿直接编辑此文件！修改请编辑 CMakeLists.txt 或 installer/setup.iss.in
;
; 使用方法：
; 1. 先编译 Release 版本：cmake --build build --config Release
; 2. 构建安装程序：cmake --build build --target installer
; 3. 输出：installer/output/qt_tool_template-0.0.0-Setup.exe

; ========================================
; 应用信息配置（从 CMakeLists.txt 读取）
; ========================================
#define MyAppName "qt_tool_template"
#define MyAppVersion "0.0.0"
#define MyAppPublisher "BES"
#define MyAppURL "https://github.com/yourusername/yourproject"
#define MyAppExeName "qt_tool_template.exe"
#define MyAppDescription "Qt Tool Template Application"

; 源文件目录（Release 构建输出）
#define SourceDir "..\bin\Release"

[Setup]
; 唯一标识符
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppComments={#MyAppDescription}

; 安装目录
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}

; 输出设置
OutputDir=output
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-Setup
; 如果有图标，取消下面的注释
; SetupIconFile={#SourceDir}\resources\icons\app.ico
; UninstallDisplayIcon={app}\{#MyAppExeName}

; 压缩设置（最大压缩）
Compression=lzma2/ultra64
SolidCompression=yes
LZMAUseSeparateProcess=yes

; 界面设置
WizardStyle=modern
WizardSizePercent=110

; 权限设置
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=dialog

; 其他
DisableProgramGroupPage=yes
DisableWelcomePage=no
ShowLanguageDialog=auto

; 版本信息（显示在文件属性中）
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppDescription}
VersionInfoProductName={#MyAppName}

[Languages]
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "创建桌面快捷方式(&D)"; GroupDescription: "附加图标:"; Flags: checkedonce
Name: "quicklaunchicon"; Description: "创建快速启动栏图标(&Q)"; GroupDescription: "附加图标:"; Flags: unchecked

[Files]
; 主程序和所有依赖（递归复制整个 Release 目录）
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Dirs]
; 创建应用数据目录（带写权限）
Name: "{app}\config"; Permissions: users-modify
Name: "{app}\data"; Permissions: users-modify
Name: "{app}\logs"; Permissions: users-modify

[Icons]
; 开始菜单快捷方式
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\卸载 {#MyAppName}"; Filename: "{uninstallexe}"
; 桌面快捷方式（根据用户选择）
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
; 安装完成后可选运行程序
Filename: "{app}\{#MyAppExeName}"; Description: "运行 {#MyAppName}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; 卸载时删除程序生成的文件和目录
Type: filesandordirs; Name: "{app}\logs"
Type: filesandordirs; Name: "{app}\data"
; 注意：config 目录保留，以便用户重新安装时保留配置

[Code]
// ========================================
// Pascal 脚本 - 自定义安装逻辑
// ========================================

// 检查是否已安装旧版本
function InitializeSetup(): Boolean;
var
  OldVersion: String;
  UninstallKey: String;
begin
  Result := True;
  UninstallKey := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\' + ExpandConstant('{#SetupSetting("AppId")}') + '_is1';
  
  if RegQueryStringValue(HKLM, UninstallKey, 'DisplayVersion', OldVersion) or
     RegQueryStringValue(HKCU, UninstallKey, 'DisplayVersion', OldVersion) then
  begin
    if MsgBox('检测到已安装的版本: ' + OldVersion + #13#10#13#10 +
              '是否要继续安装新版本？' + #13#10 +
              '（旧版本将被覆盖）', mbConfirmation, MB_YESNO) = IDNO then
    begin
      Result := False;
    end;
  end;
end;

// 安装完成后的操作
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    // 这里可以添加安装后的自定义操作
    // 例如：写入配置文件、注册服务等
  end;
end;

// 卸载前确认
function InitializeUninstall(): Boolean;
begin
  Result := MsgBox('确定要卸载 {#MyAppName} 吗？', mbConfirmation, MB_YESNO) = IDYES;
end;

