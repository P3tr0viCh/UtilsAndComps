// ---------------------------------------------------------------------------

#ifndef SystemInfoH
#define SystemInfoH

#include <System.SysUtils.hpp>
#include <System.StrUtils.hpp>
#include <System.Classes.hpp>

#include <Vcl.Printers.hpp>

#include "Registry.hpp"

namespace P3tr0viCh {

	// ---------------------------------------------------------------------------
	class PACKAGE TWindowsVersion : public TObject {
	private:
		String FProductName;
		String FReleaseId;
		String FCurrentVersion;
		String FCurrentBuild;
		String FUBR;
		String FCSDBuildNumber;
		String FCSDVersion;
		bool F64Bit;

	public:
		void Update();

	__published:
		__property String ProductName = {read = FProductName};
		__property String ReleaseId = {read = FReleaseId};
		__property String CurrentVersion = {read = FCurrentVersion};
		__property String CurrentBuild = {read = FCurrentBuild};
		__property String UBR = {read = FUBR};
		__property String CSDBuildNumber = {read = FCSDBuildNumber};
		__property String CSDVersion = {read = FCSDVersion};
		__property bool Is64Bit = {read = F64Bit};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TLogicalDrive : public TObject {
	private:
		char FLetter;
		String FLabel;

		unsigned __int64 FAvailable;
		unsigned __int64 FTotal;
		unsigned __int64 FFree;

		int FPhysicalDriveNum;

	public:
		__fastcall TLogicalDrive(char ALetter, String ALabel,
			unsigned __int64 AAvailable, unsigned __int64 ATotal,
			unsigned __int64 AFree, int APhysicalDriveNum);

	__published:
		__property char Letter = {read = FLetter};
		__property String Label = {read = FLabel};

		__property unsigned __int64 Available = {read = FAvailable};
		__property unsigned __int64 Total = {read = FTotal};
		__property unsigned __int64 Free = {read = FFree};

		__property int PhysicalDriveNum = {read = FPhysicalDriveNum};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TPhysicalDrive : public TObject {
	private:
		String FPath;

		String FVendor;
		String FProduct;
		String FProductRevision;
		String FSerialNumber;

		float FSize;

	public:
		__fastcall TPhysicalDrive(String APath, String AVendor, String AProduct,
			String AProductRevision, String ASerialNumber, float ASize);
		__fastcall TPhysicalDrive(String APath, String AVendor);

	__published:
		__property String Path = {read = FPath};

		__property String Vendor = {read = FVendor};
		__property String Product = {read = FProduct};
		__property String ProductRevision = {read = FProductRevision};
		__property String SerialNumber = {read = FSerialNumber};

		__property float Size = {read = FSize};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TLogicalDrives : public TObjectList {
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TPhysicalDrives : public TObjectList {
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TSystemInfo : public TObject {
	private:
		TRegistry * Registry;

		String FComputerName;

		TWindowsVersion * FWindowsVersion;

		TStringList * FIPAddressList;

		String FBaseBoardManufacturer;
		String FBaseBoardProduct;
		String FSystemManufacturer;
		String FSystemProductName;

		String FProcessorName;
		String FProcessorSocket;

		DWORDLONG FPhysMemory;
		unsigned int FPhysMemoryType;
		int FPhysMemoryCount;

		String FPrinterName;

		TLogicalDrives * FLogicalDrives;

		TPhysicalDrives * FPhysicalDrives;

		TStringList * FMonitorList;

		String GetComputerName();

		void GetIPAddress(TStringList * IPAddressList);

		void GetSystemBoard(String &SystemManufacturer,
			String &SystemProductName, String &BaseBoardManufacturer,
			String &BaseBoardProduct);

		String GetProcessorName();

		DWORDLONG GetPhysMemory();

		String GetPrinterName();

		void GetLogicalDrives(TLogicalDrives * LogicalDrives);

		void GetPhysicalDrives(TPhysicalDrives * PhysicalDrives);

		void GetMonitors(TStringList * MonitorList);

		void GetSMBIOS(String &ProcessorSocket, unsigned int &PhysMemoryType,
			int &PhysMemoryCount);

	public:
		__fastcall TSystemInfo();
		__fastcall ~TSystemInfo();

		void Update();

	__published:
		__property String ComputerName = {read = FComputerName};

		__property TWindowsVersion * WindowsVersion = {read = FWindowsVersion};

		__property TStringList * IPAddressList = {read = FIPAddressList};

		__property String BaseBoardManufacturer = {read = FBaseBoardManufacturer
		};
		__property String BaseBoardProduct = {read = FBaseBoardProduct};
		__property String SystemManufacturer = {read = FSystemManufacturer};
		__property String SystemProductName = {read = FSystemProductName};

		__property String ProcessorName = {read = FProcessorName};
		__property String ProcessorSocket = {read = FProcessorSocket};

		__property DWORDLONG PhysMemory = {read = FPhysMemory};
		__property unsigned int PhysMemoryType = {read = FPhysMemoryType};
		__property int PhysMemoryCount = {read = FPhysMemoryCount};

		__property String PrinterName = {read = FPrinterName};

		__property TLogicalDrives * LogicalDrives = {read = FLogicalDrives};

		__property TPhysicalDrives * PhysicalDrives = {read = FPhysicalDrives};

		__property TStringList * MonitorList = {read = FMonitorList};
	};

	// ---------------------------------------------------------------------------
	String FormatProcessorSocket(String Socket);
	String FormatMemoryType(unsigned int Type);
}

#endif
