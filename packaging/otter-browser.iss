#define MyAppName "Otter Browser"
#define MyAppVersion "0.9.02-dev"
#define MyAppURL "http://otter-browser.org/"
#define MyAppExeName "otter-browser.exe"

[Setup]
AppId={{A0517512-5271-465D-AE59-D08F487B5CAF}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=Z:\otter-browser-inno\input\COPYING
OutputDir=Z:\otter-browser-inno\output\
OutputBaseFilename=otter-browser-{#MyAppVersion}-setup
Compression=lzma2/ultra64
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "nepali"; MessagesFile: "compiler:Languages\Nepali.islu"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "serbiancyrillic"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "serbianlatin"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "Z:\otter-browser-inno\input\otter-browser.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\icudt51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\icuin51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\icuuc51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5MultimediaWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5OpenGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Script.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Sensors.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5WebKit.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5WebKitWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5WinExtras.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Z:\otter-browser-inno\input\iconengines\*.*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs
Source: "Z:\otter-browser-inno\input\imageformats\*.*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs
Source: "Z:\otter-browser-inno\input\locale\*.*"; DestDir: "{app}\locale"; Flags: ignoreversion recursesubdirs
Source: "Z:\otter-browser-inno\input\platforms\*.*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs
Source: "Z:\otter-browser-inno\input\printsupport\*.*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs
Source: "Z:\otter-browser-inno\input\sqldrivers\*.*"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
