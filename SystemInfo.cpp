// ---------------------------------------------------------------------------

#pragma hdrstop

#include "SystemInfo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace P3tr0viCh;

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::TSystemInfo() {
	FComputerName = "";
	FIPAddressList = new TStringList();

	FWindows64Bit = false;
	FWindowsProductName = "";
	FWindowsCSDVersion = "";

	FSystemManufacturer = "";
	FSystemProductName = "";
	FBaseBoardManufacturer = "";
	FBaseBoardProduct = "";

	FProcessorName = "";

	FLogicalDrives = new TLogicalDrives();
}

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::~TSystemInfo() {
	FIPAddressList->Free();
	FLogicalDrives->Free();
}

// ------------------------------------------------------------------------
String TSystemInfo::GetComputerName() {
	DWORD Size = MAX_COMPUTERNAME_LENGTH;
	wchar_t CComputerName[MAX_COMPUTERNAME_LENGTH];

	::GetComputerName(CComputerName, &Size);

	return String(CComputerName);
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetIPAddress(TStringList *IPAddressList) {
	IPAddressList->Clear();

	WSADATA wsaData;

	if (!WSAStartup(WINSOCK_VERSION, &wsaData)) {
		char chInfo[64];

		if (!gethostname(chInfo, sizeof(chInfo))) {
			hostent *sh;
			sh = gethostbyname((char*)&chInfo);

			if (sh != NULL) {
				int nAdapter = 0;

				while (sh->h_addr_list[nAdapter]) {
					struct sockaddr_in adr;

					memcpy(&adr.sin_addr, sh->h_addr_list[nAdapter],
						sh->h_length);

					IPAddressList->Add(inet_ntoa(adr.sin_addr));

					nAdapter++;
				}
			}
		}
	}

	WSACleanup();
}

// ---------------------------------------------------------------------------
bool TSystemInfo::GetWindows64Bit() {
	DWORD Size = MAX_PATH;
	wchar_t Directory[MAX_PATH];

	return GetSystemWow64Directory(Directory, Size) > 0;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetWindowsVersion(String &ProductName, String &CSDVersion) {
	ProductName = "";
	CSDVersion = "";

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKeyReadOnly
			("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")) {
			ProductName = Registry->ReadString("ProductName");

			CSDVersion = Registry->ReadString("CSDVersion");

			Registry->CloseKey();
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetSystemBoard(String &SystemManufacturer,
	String &SystemProductName, String &BaseBoardManufacturer,
	String &BaseBoardProduct) {

	SystemManufacturer = "";
	SystemProductName = "";
	BaseBoardManufacturer = "";
	BaseBoardProduct = "";

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKeyReadOnly("HARDWARE\\DESCRIPTION\\System\\BIOS")) {
			SystemManufacturer = Registry->ReadString("SystemManufacturer");
			SystemProductName = Registry->ReadString("SystemProductName");
			BaseBoardManufacturer =
				Registry->ReadString("BaseBoardManufacturer");
			BaseBoardProduct = Registry->ReadString("BaseBoardProduct");

			Registry->CloseKey();
		}
	}
}

// ---------------------------------------------------------------------------
String TSystemInfo::GetProcessorName() {
	String S = "";

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;

		if (Registry->OpenKeyReadOnly
			("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")) {
			S = Registry->ReadString("ProcessorNameString");

			Registry->CloseKey();
		}
	}

	return S;
}

// ---------------------------------------------------------------------------
DWORDLONG TSystemInfo::GetPhysMemory() {
	TMemoryStatusEx MS;

	MS.dwLength = sizeof(TMemoryStatusEx);
	GlobalMemoryStatusEx(&MS);

	return MS.ullTotalPhys;
}

// ---------------------------------------------------------------------------
String TSystemInfo::GetPrinterName() {
	wchar_t Device[100], Driver[MAX_PATH], Port[MAX_PATH];

	unsigned int DeviceMode;

	if (Printer()->Printers->Count == 0) {
		return "";
	}
	else {
		Printer()->GetPrinter(Device, Driver, Port, DeviceMode);

		return Device;
	}
}

// ---------------------------------------------------------------------------
__fastcall TLogicalDrive::TLogicalDrive(char ALetter, String ALabel,
	unsigned __int64 AAvailable, unsigned __int64 ATotal,
	unsigned __int64 AFree) {
	FLetter = ALetter;
	FLabel = ALabel;

	FAvailable = AAvailable;
	FTotal = ATotal;
	FFree = AFree;
}

// ---------------------------------------------------------------------------
String GetVolumeLabel(String Path) {
	wchar_t Label[30];

	if (GetVolumeInformation(Path.w_str(), Label, sizeof(Label), NULL, NULL,
		NULL, NULL, NULL)) {
		return Label;
	}

	return "error";
}

// ---------------------------------------------------------------------------
void GetDiskSpace(String Path, unsigned __int64 &Available,
	unsigned __int64 &Total, unsigned __int64 &Free) {
	if (!GetDiskFreeSpaceEx(Path.w_str(), (PULARGE_INTEGER) & Available,
		(PULARGE_INTEGER) & Total, (PULARGE_INTEGER) & Free)) {
		Available = 0;
		Total = 0;
		Free = 0;
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetLogicalDrives(TLogicalDrives *LogicalDrives) {
	LogicalDrives->Clear();

	char Letter;
	String Path;
	String Label;

	DWORD Drives = ::GetLogicalDrives();

	for (int i = 2; i < 26; i++) {
		if ((Drives >> i) & 1) {
			Letter = AnsiString(char(65 + i))[1];

			Path = String(Letter) + ":\\";

			if (GetDriveType(Path.w_str()) == DRIVE_FIXED) {
				Label = GetVolumeLabel(Path);

				unsigned __int64 Available;
				unsigned __int64 Total;
				unsigned __int64 Free;

				GetDiskSpace(Path, Available, Total, Free);

				LogicalDrives->Add(new TLogicalDrive(Letter, Label, Available,
					Total, Free));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::Update() {
	Registry = new TRegistry();

	FComputerName = GetComputerName();

	GetIPAddress(FIPAddressList);

	FWindows64Bit = GetWindows64Bit();
	GetWindowsVersion(FWindowsProductName, FWindowsCSDVersion);

	GetSystemBoard(FSystemManufacturer, FSystemProductName,
		FBaseBoardManufacturer, FBaseBoardProduct);

	FProcessorName = GetProcessorName();

	FPhysMemory = GetPhysMemory();

	FPrinterName = GetPrinterName();

	GetLogicalDrives(FLogicalDrives);

	if (Registry) {
		delete Registry;
	}
}
