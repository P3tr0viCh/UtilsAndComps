// ---------------------------------------------------------------------------

#pragma hdrstop

#include "SystemInfo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace P3tr0viCh;

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::TSystemInfo() {
	FComputerName = "";

	FWindowsVersion = new TWindowsVersion();

	FIPAddressList = new TStringList();
	FAdapterInfoList = new TAdapterInfoList();

	FSystemBoard = new TSystemBoard();

	FProcessorName = "";
	FProcessorSocket = "";

	FPhysMemory = 0;
	FPhysMemoryType = 0;
	FPhysMemoryCount = 0;

	FLogicalDrives = new TLogicalDrives();

	FPhysicalDrives = new TPhysicalDrives();

	FMonitorList = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::~TSystemInfo() {
	FMonitorList->Free();
	FPhysicalDrives->Free();
	FLogicalDrives->Free();
	FSystemBoard->Free();
	FAdapterInfoList->Free();
	FIPAddressList->Free();
	FWindowsVersion->Free();
}

// ------------------------------------------------------------------------
void TSystemInfo::GetComputerName() {
	DWORD Size = 0;

	wchar_t *CComputerName;

	GetComputerNameEx(ComputerNameDnsFullyQualified, NULL, &Size);

	if (GetLastError() != ERROR_MORE_DATA) {
		return;
	}

	CComputerName = new wchar_t[Size];

	if (!GetComputerNameEx(ComputerNameDnsFullyQualified, CComputerName, &Size))
	{
		return;
	}

	FComputerName = String(CComputerName);
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetWindowsVersion() {
	FWindowsVersion->FProductName = "";
	FWindowsVersion->FReleaseId = "";
	FWindowsVersion->FCurrentVersion = "";
	FWindowsVersion->FCurrentBuild = "";
	FWindowsVersion->FUBR = "";
	FWindowsVersion->FCSDBuildNumber = "";
	FWindowsVersion->FCSDVersion = "";
	FWindowsVersion->F64Bit = false;

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKeyReadOnly
			("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")) {
			FWindowsVersion->FProductName = Registry->ReadString("ProductName");
			FWindowsVersion->FReleaseId = Registry->ReadString("ReleaseId");
			FWindowsVersion->FCurrentVersion =
				Registry->ReadString("CurrentVersion");
			FWindowsVersion->FCurrentBuild =
				Registry->ReadString("CurrentBuild");

			try {
				FWindowsVersion->FUBR = IntToStr(Registry->ReadInteger("UBR"));
			}
			catch (...) {
			}

			FWindowsVersion->FCSDBuildNumber =
				Registry->ReadString("CSDBuildNumber");
			FWindowsVersion->FCSDVersion = Registry->ReadString("CSDVersion");

			Registry->CloseKey();
		}
	}

	DWORD Size = MAX_PATH;
	wchar_t Directory[MAX_PATH];

	FWindowsVersion->F64Bit = GetSystemWow64Directory(Directory, Size) > 0;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetWindowsUptime() {
	FWindowsUptime = GetTickCount64();
	FWindowsBootDateTime = IncMilliSecond(Now(), -FWindowsUptime);
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetIPAddress() {
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
void TSystemInfo::GetAdapterInfoList() {
	AdapterInfoList->Clear();

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	try {
		pAdapterInfo = (IP_ADAPTER_INFO*) malloc(sizeof(IP_ADAPTER_INFO));
		if (pAdapterInfo == NULL) {
			return;
		}

		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)
			== ERROR_BUFFER_OVERFLOW) {
			free(pAdapterInfo);

			pAdapterInfo = (IP_ADAPTER_INFO*) malloc(ulOutBufLen);

			if (pAdapterInfo == NULL) {
				return;
			}
		}

		if ((GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			pAdapter = pAdapterInfo;

			TAdapterInfo * AdapterInfo;

			while (pAdapter) {
				if (AnsiContainsText(pAdapter->Description,
					"Wi-Fi Direct Virtual Adapter")) {
					pAdapter = pAdapter->Next;
					continue;
				}

				AdapterInfo = new TAdapterInfo();

				AdapterInfo->FGUID = pAdapter->AdapterName;
				AdapterInfo->FName = pAdapter->AdapterName;
				AdapterInfo->FDescription = pAdapter->Description;

				String Address;
				for (UINT i = 0; i < pAdapter->AddressLength - 1; i++) {
					Address =
						Address + Format("%0.2X",
						ARRAYOFCONST((pAdapter->Address[i]))) + "-";
				}
				Address = Address + Format("%0.2X",
					ARRAYOFCONST(
					(pAdapter->Address[pAdapter->AddressLength - 1])));

				AdapterInfo->FMACAddress = Address;

				AdapterInfo->FType = pAdapter->Type;

				AdapterInfo->FIPAddress =
					pAdapter->IpAddressList.IpAddress.String;

				AdapterInfoList->Add(AdapterInfo);

				pAdapter = pAdapter->Next;
			}
		}
	}
	__finally {
		if (pAdapterInfo) {
			free(pAdapterInfo);
		}
	}

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;

		String Name;
		String S = "SYSTEM\\CurrentControlSet\\Control\\Network";

		if (Registry->OpenKeyReadOnly(S)) {
			TStringList * Keys = new TStringList();

			Registry->GetKeyNames(Keys);

			Registry->CloseKey();

			for (int i = 0; i < AdapterInfoList->Count; i++) {
				for (int k = 0; k < Keys->Count; k++) {
					if (Registry->OpenKeyReadOnly(S + "\\" + Keys->Strings[k] +
						"\\" + ((TAdapterInfo*) AdapterInfoList->Items[i])
						->FGUID + "\\Connection")) {
						Name = Registry->ReadString("Name");

						if (Name != "") {
							((TAdapterInfo*) AdapterInfoList->Items[i])
								->FName = Name;
						}

						Registry->CloseKey();
					}
				}
			}

			Keys->Free();
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetSystemBoard() {
	FSystemBoard->FSystemManufacturer = "";
	FSystemBoard->FSystemProductName = "";
	FSystemBoard->FBaseBoardManufacturer = "";
	FSystemBoard->FBaseBoardProduct = "";

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKeyReadOnly("HARDWARE\\DESCRIPTION\\System\\BIOS")) {
			FSystemBoard->FSystemManufacturer =
				Registry->ReadString("SystemManufacturer");
			FSystemBoard->FSystemProductName =
				Registry->ReadString("SystemProductName");
			FSystemBoard->FBaseBoardManufacturer =
				Registry->ReadString("BaseBoardManufacturer");
			FSystemBoard->FBaseBoardProduct =
				Registry->ReadString("BaseBoardProduct");

			Registry->CloseKey();
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetProcessorName() {
	String S = "";

	if (Registry) {
		Registry->RootKey = HKEY_LOCAL_MACHINE;

		if (Registry->OpenKeyReadOnly
			("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")) {
			S = Registry->ReadString("ProcessorNameString");

			Registry->CloseKey();
		}
	}

	FProcessorName = S;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetPhysMemory() {
	TMemoryStatusEx MS;

	MS.dwLength = sizeof(TMemoryStatusEx);
	GlobalMemoryStatusEx(&MS);

	FPhysMemory = MS.ullTotalPhys;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetPrinterName() {
	FPrinterName = NULL;

	wchar_t Device[100], Driver[MAX_PATH], Port[MAX_PATH];

	unsigned int DeviceMode;

	if (Printer()->Printers->Count < 1) {
		return;
	}

	try {
		Printer()->GetPrinter(Device, Driver, Port, DeviceMode);
	}
	catch (...) {
		return;
	}

	FPrinterName = Device;
}

// ---------------------------------------------------------------------------
__fastcall TPhysicalDrive::TPhysicalDrive() {
	FPath = NULL;
	FVendor = NULL;
	FProduct = NULL;
	FProductRevision = NULL;
	FSerialNumber = NULL;

	FSize = -1;
}

// ---------------------------------------------------------------------------
bool IsDriveReady(String Path) {
	WORD OldErrorMode;

	bool Result;

	OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	try {
		Result = DirectoryExists(Path);
	}
	__finally {
		SetErrorMode(OldErrorMode);
	}

	return Result;
}

// ---------------------------------------------------------------------------
String GetVolumeLabel(String Path) {
	wchar_t Label[30];

	if (GetVolumeInformation(Path.w_str(), Label, sizeof(Label), NULL, NULL,
		NULL, NULL, NULL)) {
		return Label;
	}

	return "";
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
int GetPhysicalDriveNumByLetter(char Letter) {
	String DrivePath = Format("\\\\.\\%s:", ARRAYOFCONST((Letter)));

	HANDLE hDevice = CreateFile(DrivePath.c_str(), 0,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDevice == NULL || hDevice == INVALID_HANDLE_VALUE) {
		return -1;
	}

	STORAGE_DEVICE_NUMBER storageDeviceNumber = {0};
	DWORD dwBytesReturned = 0;

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0,
		&storageDeviceNumber, sizeof(storageDeviceNumber), &dwBytesReturned,
		NULL)) {
		CloseHandle(hDevice);

		return -1;
	}

	CloseHandle(hDevice);

	return storageDeviceNumber.DeviceNumber;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetLogicalDrives() {
	LogicalDrives->Clear();

	String Path;

	UINT DriveType;
	int PhysicalDriveNum;

	DWORD Drives = ::GetLogicalDrives();

	for (int i = 2; i < 26; i++) {
		if ((Drives >> i) & 1) {
			TLogicalDrive * LogicalDrive = new TLogicalDrive();

			LogicalDrive->FLetter = AnsiString(char(65 + i))[1];

			Path = String(LogicalDrive->FLetter) + ":\\";

			DriveType = GetDriveType(Path.w_str());

			if (DriveType == DRIVE_FIXED || DriveType == DRIVE_REMOVABLE) {
				LogicalDrive->FLabel = GetVolumeLabel(Path);

				if (IsDriveReady(Path)) {
					GetDiskSpace(Path, LogicalDrive->FAvailable,
						LogicalDrive->FTotal, LogicalDrive->FFree);
				}

				LogicalDrive->FPhysicalDriveNum =
					GetPhysicalDriveNumByLetter(LogicalDrive->FLetter);

				LogicalDrives->Add(LogicalDrive);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetPhysicalDrives() {
	PhysicalDrives->Clear();

	int DriveNum = 0;

	do {
		String DrivePath = Format("\\\\.\\PhysicalDrive%d",
			ARRAYOFCONST((DriveNum)));

		HANDLE hDevice = CreateFile(DrivePath.c_str(), 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (hDevice == NULL || hDevice == INVALID_HANDLE_VALUE) {
			break;
		}

		STORAGE_PROPERTY_QUERY storagePropertyQuery;
		ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));

		storagePropertyQuery.PropertyId = StorageDeviceProperty;
		storagePropertyQuery.QueryType = PropertyStandardQuery;

		STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = {0};
		DWORD dwBytesReturned = 0;

		if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
			&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
			&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
			&dwBytesReturned, NULL)) {
			CloseHandle(hDevice);

			continue;
		}

		const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
		BYTE * pOutBuffer = new BYTE[dwOutBufferSize];
		ZeroMemory(pOutBuffer, dwOutBufferSize);

		if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
			&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), pOutBuffer,
			dwOutBufferSize, &dwBytesReturned, NULL)) {
			delete[]pOutBuffer;

			CloseHandle(hDevice);

			continue;
		}

		DISK_GEOMETRY geometry;

		if (!DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0,
			&geometry, sizeof(geometry), &dwBytesReturned, NULL)) {
			delete[]pOutBuffer;

			CloseHandle(hDevice);

			continue;
		}

		delete[]pOutBuffer;

		CloseHandle(hDevice);

		STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor =
			(STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;

		TPhysicalDrive * PhysicalDrive = new TPhysicalDrive();

		PhysicalDrive->FPath = DrivePath;

		PhysicalDrive->FVendor = (pDeviceDescriptor->VendorIdOffset != 0 ?
			String((char*)((char*)pDeviceDescriptor +
			pDeviceDescriptor->VendorIdOffset)) : String(""));
		PhysicalDrive->FProduct = pDeviceDescriptor->ProductIdOffset != 0 ?
			String((char*)((char*)pDeviceDescriptor +
			pDeviceDescriptor->ProductIdOffset)) : String("");
		PhysicalDrive->FProductRevision =
			pDeviceDescriptor->ProductRevisionOffset != 0 ?
			String((char*)((char*)pDeviceDescriptor +
			pDeviceDescriptor->ProductRevisionOffset)) : String("");
		PhysicalDrive->FSerialNumber =
			pDeviceDescriptor->SerialNumberOffset != 0 ?
			Trim(String((char*)((char*)pDeviceDescriptor +
			pDeviceDescriptor->SerialNumberOffset))) : String("");

		PhysicalDrive->FSize =
			(double)geometry.BytesPerSector * geometry.Cylinders.QuadPart *
			geometry.SectorsPerTrack * geometry.TracksPerCylinder;

		PhysicalDrives->Add(PhysicalDrive);
	}
	while (++DriveNum) /* exit by break */ ;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetMonitors() {
	MonitorList->Clear();

	DISPLAY_DEVICE DD;
	DD.cb = sizeof(DD);

	int DeviceIndex = 0;
	int MonitorIndex;

	std::wstring DeviceName;

	while (EnumDisplayDevices(0, DeviceIndex, &DD, 0)) {
		DeviceName = DD.DeviceName;

		MonitorIndex = 0;

		while (EnumDisplayDevices(DeviceName.c_str(), MonitorIndex, &DD, 0)) {
			MonitorList->Add(DD.DeviceString);

			MonitorIndex++;
		}

		DeviceIndex++;
	}

}

// ---------------------------------------------------------------------------
typedef struct _RawSMBIOSData {
	BYTE Used20CallingMethod;
	BYTE SMBIOSMajorVersion;
	BYTE SMBIOSMinorVersion;
	BYTE DmiRevision;
	DWORD Length;
	PBYTE SMBIOSTableData;
} RawSMBIOSData, *PRawSMBIOSData;

// ---------------------------------------------------------------------------
typedef struct _SMBIOSHEADER_ {
	BYTE Type;
	BYTE Length;
	WORD Handle;
} SMBIOSHEADER, *PSMBIOSHEADER;

// ---------------------------------------------------------------------------
typedef struct _TYPE_4_ {
	SMBIOSHEADER Header;

	BYTE SocketDesignation;
	BYTE ProcessorType;
	BYTE ProcessorFamily;
	BYTE ProcessorManufacturer;
	DWORD64 ProcessorID;
	BYTE ProcessorVersion;
	BYTE Voltage;
	WORD ExternalClock;
	WORD MaxSpeed;
	WORD CurrentSpeed;
	BYTE Status;
	BYTE ProcessorUpgrade;
	BYTE SerialNumber;
	BYTE AssetTag;
	BYTE PartNumber;
} ProcessorInfo, *PProcessorInfo;

// ---------------------------------------------------------------------------
typedef struct _TYPE_17_ {
	SMBIOSHEADER Header;

	WORD PhysicalArrayHandle;
	WORD ErrorInformationHandle;
	WORD TotalWidth;
	WORD DataWidth;
	WORD Size;
	BYTE FormFactor;
	BYTE DeviceSet;
	BYTE DeviceLocator;
	BYTE BankLocator;
	BYTE MemoryType;
	WORD TypeDetail;
	WORD Speed;
	BYTE Manufacturer;
	BYTE SerialNumber;
	BYTE AssetTag;
	BYTE PartNumber;
} MemoryDevice, *PMemoryDevice;

// ---------------------------------------------------------------------------
#ifdef UNICODE
#define LocateString	LocateStringW
#else
#define LocateString	LocateStringA
#endif

// ---------------------------------------------------------------------------
const char* LocateStringA(const char* str, UINT i) {
	static const char strNull[] = "";

	if (0 == i || 0 == *str)
		return strNull;

	while (--i) { // i-- ошибка
		str += strlen((char*)str) + 1;
	}

	return str;
}

// ---------------------------------------------------------------------------
const wchar_t* LocateStringW(const char* str, UINT i) {
	static wchar_t buff[2048];
	const char *pStr = LocateStringA(str, i);

	SecureZeroMemory(buff, sizeof(buff));
	MultiByteToWideChar(CP_OEMCP, 0, pStr, (int) strlen(pStr), buff,
		sizeof(buff));

	return buff;
}

// ---------------------------------------------------------------------------
const char* toPointString(void* p) {
	return (char*)p + ((PSMBIOSHEADER)p)->Length;
}

// ---------------------------------------------------------------------------
String ProcProcessorInfo(void* p) {
	PProcessorInfo pProcessor = (PProcessorInfo)p;
	const char *str = toPointString(p);

	return LocateString(str, pProcessor->SocketDesignation);
}

// ---------------------------------------------------------------------------
unsigned int ProcMemoryDevice(void* p) {
	PMemoryDevice pMD = (PMemoryDevice)p;

	return pMD->Size != 0 ? pMD->MemoryType : 0;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetSMBIOS() {
	FProcessorSocket = "";
	FPhysMemoryType = 0;
	FPhysMemoryCount = 0;

	if (Win32MajorVersion < 6) {
		return;
	}

	const BYTE byteSignature[] = {'B', 'M', 'S', 'R'};
	const DWORD Signature = *((DWORD*)byteSignature);

	DWORD needBufferSize = GetSystemFirmwareTable(Signature, 0, NULL, 0);

	LPBYTE pBuff = (LPBYTE) malloc(needBufferSize);

	if (pBuff) {
		try {
			GetSystemFirmwareTable(Signature, 0, pBuff, needBufferSize);

			const PRawSMBIOSData pDMIData = (PRawSMBIOSData)pBuff;

			LPBYTE p = (LPBYTE)(&(pDMIData->SMBIOSTableData));
			const LPBYTE lastAddress = p + pDMIData->Length;
			PSMBIOSHEADER pHeader;

			unsigned int MemoryType;

			for (; ;) {
				pHeader = (PSMBIOSHEADER)p;

				switch (pHeader->Type) {
				case 4:
					FProcessorSocket = ProcProcessorInfo((void*)pHeader);
					break;
				case 17:
					MemoryType = ProcMemoryDevice((void*)pHeader);
					if (MemoryType > 0) {
						FPhysMemoryType = MemoryType;
						FPhysMemoryCount++;
					}

					break;
				}

				if ((pHeader->Type == 127) && (pHeader->Length == 4))
					break;

				LPBYTE nt = p + pHeader->Length;

				while (0 != (*nt | *(nt + 1)))
					nt++;

				nt += 2;

				if (nt >= lastAddress)
					break;

				p = nt;
			}
		}
		__finally {
			free(pBuff);
		}
	}
}

// ---------------------------------------------------------------------------
void TSystemInfo::Update() {
	Registry = new TRegistry(KEY_READ | KEY_WOW64_64KEY);

	try {
		try {
			GetComputerName();

			GetWindowsVersion();

			GetWindowsUptime();

			GetIPAddress();
			GetAdapterInfoList();

			GetSystemBoard();

			GetProcessorName();

			GetPhysMemory();

			GetSMBIOS();

			GetPrinterName();

			GetLogicalDrives();

			GetPhysicalDrives();

			GetMonitors();
		}
		catch (Exception * E) {
			FComputerName = "Unknown Error: " + E->Message;
		}
	}
	__finally {
		delete Registry;
	}
}

// ---------------------------------------------------------------------------
String P3tr0viCh::FormatProcessorSocket(String Socket) {
	if (SameText(Socket, "CPU 1") || SameText(Socket, "SOCKET 0")) {
		return "";
	}

	return Trim(ReplaceText(ReplaceText(Socket, "LGA", ""), "Socket", ""));
}

// ---------------------------------------------------------------------------
String P3tr0viCh::FormatMemoryType(unsigned int Type) {
	switch (Type) {
	case 0x01:
		return "Other";
	case 0x02:
		return "Unknown";
	case 0x03:
		return "DRAM";
	case 0x04:
		return "EDRAM";
	case 0x05:
		return "VRAM";
	case 0x06:
		return "SRAM";
	case 0x07:
		return "RAM";
	case 0x08:
		return "ROM";
	case 0x09:
		return "FLASH";
	case 0x0A:
		return "EEPROM";
	case 0x0B:
		return "FEPROM";
	case 0x0C:
		return "EPROM";
	case 0x0D:
		return "CDRAM";
	case 0x0E:
		return "3DRAM";
	case 0x0F:
		return "SDRAM (DDR|DDR2)";
	case 0x10:
		return "SGRAM";
	case 0x11:
		return "RDRAM";
	case 0x12:
		return "DDR";
	case 0x13:
		return "DDR2";
	case 0x14:
		return "DDR2 FB-DIMM";
	case 0x15:
	case 0x16:
	case 0x17:
		return "Reserved";
	case 0x18:
		return "DDR3";
	case 0x19:
		return "FBD2";
	case 0x1A:
		return "DDR4";
	case 0x1B:
		return "LPDDR";
	case 0x1C:
		return "LPDDR2";
	case 0x1D:
		return "LPDDR3";
	case 0x1E:
		return "LPDDR4";
	default:
		return Type;
	}
}

// ---------------------------------------------------------------------------
String P3tr0viCh::FormatAdapterType(unsigned int Type) {
	switch (Type) {
	case MIB_IF_TYPE_ETHERNET:
		return "Ethernet";
	case IF_TYPE_ISO88025_TOKENRING:
		return "MIB_IF_TYPE_TOKENRING";
	case MIB_IF_TYPE_PPP:
		return "PPP network";
	case MIB_IF_TYPE_LOOPBACK:
		return "A software loopback network interface";
	case MIB_IF_TYPE_SLIP:
		return "An ATM network interface";
	case IF_TYPE_IEEE80211:
		return "WiFi";
	default:
		return "Type " + IntToStr((int) Type);
	}
}
