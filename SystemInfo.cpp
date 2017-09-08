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

	FPhysicalDrives = new TPhysicalDrives();
}

// ---------------------------------------------------------------------------
__fastcall TSystemInfo::~TSystemInfo() {
	FPhysicalDrives->Free();
	FLogicalDrives->Free();
	FIPAddressList->Free();
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

				int PhysicalDriveNum = GetPhysicalDriveNumByLetter(Letter);

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

	TPhysicalDrive *PhysicalDrive;

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

	GetPhysicalDrives(FPhysicalDrives);

	if (Registry) {
		delete Registry;
	}
}
