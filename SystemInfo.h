// ---------------------------------------------------------------------------

#ifndef SystemInfoH
#define SystemInfoH

#include <System.SysUtils.hpp>
#include <System.StrUtils.hpp>
#include <System.Classes.hpp>
#include <System.DateUtils.hpp>

#include <Vcl.Printers.hpp>

#include "Registry.hpp"

#pragma comment(lib,"iphlpapi.lib")
#include <iphlpapi.h>

namespace P3tr0viCh {

	class TSystemInfo;

	// ---------------------------------------------------------------------------
	class PACKAGE TWindowsVersion : public TObject {
		friend class TSystemInfo;

	private:
		String FProductName;
		String FReleaseId;
		String FCurrentVersion;
		String FCurrentBuild;
		String FUBR;
		String FCSDBuildNumber;
		String FCSDVersion;
		bool F64Bit;

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
	class PACKAGE TAdapterInfo : public TObject {
		friend class TSystemInfo;

	private:
		String FName;
		String FGUID;
		String FDescription;
		String FMACAddress;
		String FIPAddress;
		unsigned int FType;

	__published:
		__property String Name = {read = FName};
		__property String GUID = {read = FGUID};
		__property String Description = {read = FDescription};
		__property String MACAddress = {read = FMACAddress};
		__property String IPAddress = {read = FIPAddress};
		__property unsigned int Type = {read = FType};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TSystemBoard : public TObject {
		friend class TSystemInfo;

	private:
		String FBaseBoardManufacturer;
		String FBaseBoardProduct;
		String FSystemManufacturer;
		String FSystemProductName;

	__published:
		__property String BaseBoardManufacturer = {read = FBaseBoardManufacturer
		};
		__property String BaseBoardProduct = {read = FBaseBoardProduct};
		__property String SystemManufacturer = {read = FSystemManufacturer};
		__property String SystemProductName = {read = FSystemProductName};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TLogicalDrive : public TObject {
		friend class TSystemInfo;

	private:
		char FLetter;
		String FLabel;

		unsigned __int64 FAvailable;
		unsigned __int64 FTotal;
		unsigned __int64 FFree;

		int FPhysicalDriveNum;

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
		friend class TSystemInfo;

	private:
		String FPath;

		String FVendor;
		String FProduct;
		String FProductRevision;
		String FSerialNumber;

		float FSize;

	public:
		__fastcall TPhysicalDrive();

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
	class PACKAGE TAdapterInfoList : public TObjectList {
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TSystemInfo : public TObject {
	private:
		TRegistry * Registry;

		String FComputerName;

		TWindowsVersion * FWindowsVersion;

		unsigned __int64 FWindowsUptime;
		TDateTime FWindowsBootDateTime;

		TStringList * FIPAddressList;
		TAdapterInfoList * FAdapterInfoList;

		TSystemBoard * FSystemBoard;

		String FProcessorName;
		String FProcessorSocket;

		unsigned __int64 FPhysMemory;
		unsigned int FPhysMemoryType;
		int FPhysMemoryCount;

		String FPrinterName;

		TLogicalDrives * FLogicalDrives;

		TPhysicalDrives * FPhysicalDrives;

		TStringList * FMonitorList;

		void GetComputerName();

		void GetWindowsVersion();

		void GetWindowsUptime();

		void GetIPAddress();
		void GetAdapterInfoList();

		void GetSystemBoard();

		void GetProcessorName();

		void GetPhysMemory();

		void GetPrinterName();

		void GetLogicalDrives();

		void GetPhysicalDrives();

		void GetMonitors();

		void GetSMBIOS();

	public:
		__fastcall TSystemInfo();
		__fastcall ~TSystemInfo();

		void Update();

	__published:
		__property String ComputerName = {read = FComputerName};

		__property TWindowsVersion * WindowsVersion = {read = FWindowsVersion};

		__property unsigned __int64 WindowsUptime = {read = FWindowsUptime};
		__property TDateTime WindowsBootDateTime = {read = FWindowsBootDateTime
		};

		__property TStringList * IPAddressList = {read = FIPAddressList};
		__property TAdapterInfoList * AdapterInfoList = {read = FAdapterInfoList
		};

		__property TSystemBoard * SystemBoard = {read = FSystemBoard};

		__property String ProcessorName = {read = FProcessorName};
		__property String ProcessorSocket = {read = FProcessorSocket};

		__property unsigned __int64 PhysMemory = {read = FPhysMemory};
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
	String FormatAdapterType(unsigned int Type);
}

#endif
