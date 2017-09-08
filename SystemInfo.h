// ---------------------------------------------------------------------------

#ifndef SystemInfoH
#define SystemInfoH

#include <System.SysUtils.hpp>
#include <System.Classes.hpp>

#include <Vcl.Printers.hpp>

#include "Registry.hpp"

namespace P3tr0viCh {

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
		TRegistry *Registry;

		String FComputerName;
		TStringList* FIPAddressList;

		String FWindowsProductName;
		String FWindowsCSDVersion;
		bool FWindows64Bit;

		String FBaseBoardManufacturer;
		String FBaseBoardProduct;
		String FSystemManufacturer;
		String FSystemProductName;

		String FProcessorName;

		DWORDLONG FPhysMemory;

		String FPrinterName;

		TLogicalDrives* FLogicalDrives;

		TPhysicalDrives* FPhysicalDrives;

		String GetComputerName();

		void GetIPAddress(TStringList *IPAddressList);

		bool GetWindows64Bit();
		void GetWindowsVersion(String &ProductName, String &CSDVersion);

		void GetSystemBoard(String &SystemManufacturer,
			String &SystemProductName, String &BaseBoardManufacturer,
			String &BaseBoardProduct);

		String GetProcessorName();

		DWORDLONG GetPhysMemory();

		String GetPrinterName();

		void GetLogicalDrives(TLogicalDrives *LogicalDrives);

		void GetPhysicalDrives(TPhysicalDrives *PhysicalDrives);

	public:
		__fastcall TSystemInfo();
		__fastcall ~TSystemInfo();

		void Update();

	__published:
		__property String ComputerName = {read = FComputerName};
		__property TStringList* IPAddressList = {read = FIPAddressList};

		__property String WindowsProductName = {read = FWindowsProductName};
		__property String WindowsCSDVersion = {read = FWindowsCSDVersion};
		__property bool IsWindows64Bit = {read = FWindows64Bit};

		__property String BaseBoardManufacturer = {read = FBaseBoardManufacturer
		};
		__property String BaseBoardProduct = {read = FBaseBoardProduct};
		__property String SystemManufacturer = {read = FSystemManufacturer};
		__property String SystemProductName = {read = FSystemProductName};

		__property String ProcessorName = {read = FProcessorName};

		__property DWORDLONG PhysMemory = {read = FPhysMemory};

		__property String PrinterName = {read = FPrinterName};

		__property TLogicalDrives* LogicalDrives = {read = FLogicalDrives};

		__property TPhysicalDrives* PhysicalDrives = {read = FPhysicalDrives};
	};
}
#endif
