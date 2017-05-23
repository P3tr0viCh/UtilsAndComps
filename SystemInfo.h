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

	public:
		__fastcall TLogicalDrive(char ALetter, String ALabel,
			unsigned __int64 FAvailable, unsigned __int64 FTotal,
			unsigned __int64 FFree);

	__published:
		__property char Letter = {read = FLetter};
		__property String Label = {read = FLabel};

		__property unsigned __int64 Available = {read = FAvailable};
		__property unsigned __int64 Total = {read = FTotal};
		__property unsigned __int64 Free = {read = FFree};
	};

	// ---------------------------------------------------------------------------
	class PACKAGE TLogicalDrives : public TObjectList {
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
	};
}
#endif
