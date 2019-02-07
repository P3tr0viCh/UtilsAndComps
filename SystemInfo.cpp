// ---------------------------------------------------------------------------

#pragma hdrstop

#include "SystemInfo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace P3tr0viCh;

// ---------------------------------------------------------------------------
void TWindowsVersion::Update() {
	FProductName = "";
	FReleaseId = "";
	FCurrentVersion = "";
	FCurrentBuild = "";
	FUBR = "";
	FCSDBuildNumber = "";
	FCSDVersion = "";
	F64Bit = false;

	TRegistry * Registry = new TRegistry(KEY_READ | KEY_WOW64_64KEY);

	try {
		Registry->RootKey = HKEY_LOCAL_MACHINE;
		if (Registry->OpenKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			false)) {
			FProductName = Registry->ReadString("ProductName");
			FReleaseId = Registry->ReadString("ReleaseId");
			FCurrentVersion = Registry->ReadString("CurrentVersion");
			FCurrentBuild = Registry->ReadString("CurrentBuild");

			try {
				FUBR = IntToStr(Registry->ReadInteger("UBR"));
			}
			catch (...) {
			}

			FCSDBuildNumber = Registry->ReadString("CSDBuildNumber");
			FCSDVersion = Registry->ReadString("CSDVersion");

			Registry->CloseKey();
		}
	}
	__finally {
		delete Registry;
	}

	DWORD Size = MAX_PATH;
	wchar_t Directory[MAX_PATH];

	F64Bit = GetSystemWow64Directory(Directory, Size) > 0;
}

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::TSystemInfo() {
	FComputerName = "";

	FWindowsVersion = new TWindowsVersion();

	FIPAddressList = new TStringList();

	FSystemManufacturer = "";
	FSystemProductName = "";
	FBaseBoardManufacturer = "";
	FBaseBoardProduct = "";

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
	FIPAddressList->Free();
	FWindowsVersion->Free();
}

// ------------------------------------------------------------------------
String TSystemInfo::GetComputerName() {
	DWORD Size = 0;

	wchar_t *CComputerName;

	GetComputerNameEx(ComputerNameDnsFullyQualified, NULL, &Size);

	if (GetLastError() != ERROR_MORE_DATA) {
		return NULL;
	}

	CComputerName = new wchar_t[Size];

	if (!GetComputerNameEx(ComputerNameDnsFullyQualified, CComputerName, &Size))
	{
		return NULL;
	}

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

	if (Printer()->Printers->Count < 1) {
		return NULL;
	}

	try {
		Printer()->GetPrinter(Device, Driver, Port, DeviceMode);
	}
	catch (...) {
		return NULL;
	}

	return Device;
}

// ---------------------------------------------------------------------------
__fastcall TLogicalDrive::TLogicalDrive(char ALetter, String ALabel,
	unsigned __int64 AAvailable, unsigned __int64 ATotal,
	unsigned __int64 AFree, int APhysicalDriveNum) {
	FLetter = ALetter;
	FLabel = ALabel;

	FAvailable = AAvailable;
	FTotal = ATotal;
	FFree = AFree;

	FPhysicalDriveNum = APhysicalDriveNum;
}

// ---------------------------------------------------------------------------
__fastcall TPhysicalDrive::TPhysicalDrive(String APath, String AVendor,
	String AProduct, String AProductRevision, String ASerialNumber, float ASize)
{
	FPath = APath;
	FVendor = AVendor;
	FProduct = AProduct;
	FProductRevision = AProductRevision;
	FSerialNumber = ASerialNumber;

	FSize = ASize;
}

// ---------------------------------------------------------------------------
__fastcall TPhysicalDrive::TPhysicalDrive(String APath, String AVendor) {
	::TPhysicalDrive(APath, AVendor, NULL, NULL, NULL, -1);
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
void TSystemInfo::GetLogicalDrives(TLogicalDrives *LogicalDrives) {
	LogicalDrives->Clear();

	char Letter;
	String Path;
	String Label;

	UINT DriveType;
	int PhysicalDriveNum;

	DWORD Drives = ::GetLogicalDrives();

	unsigned __int64 Available;
	unsigned __int64 Total;
	unsigned __int64 Free;

	for (int i = 2; i < 26; i++) {
		if ((Drives >> i) & 1) {
			Letter = AnsiString(char(65 + i))[1];

			Path = String(Letter) + ":\\";

			DriveType = GetDriveType(Path.w_str());

			if (DriveType == DRIVE_FIXED || DriveType == DRIVE_REMOVABLE) {
				Label = GetVolumeLabel(Path);

				if (IsDriveReady(Path)) {
					GetDiskSpace(Path, Available, Total, Free);
				}
				else {
					Available = 0;
					Total = 0;
					Free = 0;
				}

				PhysicalDriveNum = GetPhysicalDriveNumByLetter(Letter);

				LogicalDrives->Add(new TLogicalDrive(Letter, Label, Available,
					Total, Free, PhysicalDriveNum));
			}
		}
	}
}

// ---------------------------------------------------------------------------
TPhysicalDrive* GetPhysicalDriveInfo(int DriveNum) {
	String DrivePath = Format("\\\\.\\PhysicalDrive%d",
		ARRAYOFCONST((DriveNum)));

	HANDLE hDevice = CreateFile(DrivePath.c_str(), 0,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDevice == NULL || hDevice == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));

	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	TPhysicalDrive *PhysicalDrive;

	DWORD dwRet;
	STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = {0};
	DWORD dwBytesReturned = 0;

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
		&dwBytesReturned, NULL)) {
		dwRet = GetLastError();

		CloseHandle(hDevice);

		return new TPhysicalDrive(DrivePath,
			"Exception: " + IntToStr((int)dwRet), NULL, NULL, NULL, 0);
	}

	const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
	BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
	ZeroMemory(pOutBuffer, dwOutBufferSize);

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), pOutBuffer,
		dwOutBufferSize, &dwBytesReturned, NULL)) {
		dwRet = GetLastError();

		delete[]pOutBuffer;

		CloseHandle(hDevice);

		return new TPhysicalDrive(DrivePath,
			"Exception: " + IntToStr((int)dwRet));
	}

	DISK_GEOMETRY geometry;

	if (!DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0,
		&geometry, sizeof(geometry), &dwBytesReturned, NULL)) {
		dwRet = GetLastError();

		CloseHandle(hDevice);

		return new TPhysicalDrive(DrivePath,
			"Exception: " + IntToStr((int)dwRet));
	}

	CloseHandle(hDevice);

	STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor =
		(STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;

	String Vendor = (pDeviceDescriptor->VendorIdOffset != 0 ?
		String((char*)((char*)pDeviceDescriptor +
		pDeviceDescriptor->VendorIdOffset)) : String(""));
	String Product = pDeviceDescriptor->ProductIdOffset != 0 ?
		String((char*)((char*)pDeviceDescriptor +
		pDeviceDescriptor->ProductIdOffset)) : String("");
	String ProductRevision = pDeviceDescriptor->ProductRevisionOffset != 0 ?
		String((char*)((char*)pDeviceDescriptor +
		pDeviceDescriptor->ProductRevisionOffset)) : String("");
	String SerialNumber = pDeviceDescriptor->SerialNumberOffset != 0 ?
		Trim(String((char*)((char*)pDeviceDescriptor +
		pDeviceDescriptor->SerialNumberOffset))) : String("");

	delete[]pOutBuffer;

	double Size =
		(double)geometry.BytesPerSector * geometry.Cylinders.QuadPart *
		geometry.SectorsPerTrack * geometry.TracksPerCylinder;

	PhysicalDrive = new TPhysicalDrive(DrivePath, Vendor, Product,
		ProductRevision, SerialNumber, Size);

	return PhysicalDrive;
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetPhysicalDrives(TPhysicalDrives * PhysicalDrives) {
	PhysicalDrives->Clear();

	int DriveNum = 0;

	TPhysicalDrive * PhysicalDrive;

	do {
		PhysicalDrive = GetPhysicalDriveInfo(DriveNum);

		if (PhysicalDrive != NULL) {
			PhysicalDrives->Add(PhysicalDrive);
		}

		DriveNum++;
	}
	while (PhysicalDrive != NULL);
}

// ---------------------------------------------------------------------------
void TSystemInfo::GetMonitors(TStringList * MonitorList) {
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

	while (--i) {  // i-- ошибка
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
void TSystemInfo::GetSMBIOS(String &ProcessorSocket,
	unsigned int &PhysMemoryType, int &PhysMemoryCount) {

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
					ProcessorSocket = ProcProcessorInfo((void*)pHeader);
					break;
				case 17:
					MemoryType = ProcMemoryDevice((void*)pHeader);
					if (MemoryType > 0) {
						PhysMemoryType = MemoryType;
						PhysMemoryCount++;
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
	Registry = new TRegistry();

	try {
		try {
			FComputerName = GetComputerName();

			GetIPAddress(FIPAddressList);

			FWindowsVersion->Update();

			GetSystemBoard(FSystemManufacturer, FSystemProductName,
				FBaseBoardManufacturer, FBaseBoardProduct);

			FProcessorName = GetProcessorName();

			FPhysMemory = GetPhysMemory();

			GetSMBIOS(FProcessorSocket, FPhysMemoryType, FPhysMemoryCount);

			FPrinterName = GetPrinterName();

			GetLogicalDrives(FLogicalDrives);

			GetPhysicalDrives(FPhysicalDrives);

			GetMonitors(FMonitorList);
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
