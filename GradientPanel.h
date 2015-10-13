// ---------------------------------------------------------------------------

#ifndef GradientPanelH
#define GradientPanelH
// ---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------

enum DECLSPEC_DENUM TFillDirection : unsigned char {
	fdTopToBottom, fdBottomToTop, fdLeftToRight, fdRightToLeft
};

class PACKAGE TGradientPanel : public TCustomPanel {
private:
	bool FNotPaint;
	TFillDirection FFillDirection;
	TColor FColorStart, FColorEnd;

	void __fastcall SetColorEnd(TColor Color);
	void __fastcall SetFillDirection(TFillDirection FillDirection);
	void __fastcall SetColorStart(TColor Color);

protected:
	void __fastcall Paint(void);

public:
	__fastcall TGradientPanel(TComponent* Owner);
	void __fastcall StartUpdate();
	void __fastcall EndUpdate();

	__property DockManager;

__published:
	__property TColor ColorEnd   = {read = FColorEnd, write = SetColorEnd};
	__property TColor ColorStart = {read = FColorStart, write = SetColorStart};
	__property TFillDirection FillDirection = {
		read = FFillDirection, write = SetFillDirection};

	__property Align      = { default = 0};
	__property Alignment  = { default = 2};
	__property Anchors    = { default = 3};
	__property AutoSize   = { default = 0};
	__property BevelEdges = { default = 15};
	__property BevelInner = { default = 0};
	__property BevelKind  = { default = 0};
	__property BevelOuter = { default = 2};
	__property BevelWidth = { default = 1};

	__property BiDiMode;

	__property BorderWidth = { default = 0};
	__property BorderStyle = { default = 0};
	__property Caption     = { default = 0};
	__property Color       = { default = -16777201};

	__property Constraints;
	__property Ctl3D;

	__property UseDockManager = { default = 1};
	__property DockSite       = { default = 0};

	__property DoubleBuffered;

	__property DragCursor  = { default = -12};
	__property DragKind    = { default = 0};
	__property DragMode    = { default = 0};
	__property Enabled     = { default = 1};
	__property FullRepaint = { default = 1};

	__property Font;

	__property Locked = { default = 0};

	__property Padding;

	__property ParentBiDiMode       = { default = 1};
	__property ParentBackground     = { default = 1};
	__property ParentColor          = { default = 0};
	__property ParentCtl3D          = { default = 1};
	__property ParentDoubleBuffered = { default = 1};
	__property ParentFont           = { default = 1};
	__property ParentShowHint       = { default = 1};

	__property PopupMenu;

	__property ShowCaption = { default = 1};

	__property ShowHint;

	__property TabOrder = { default = -1};
	__property TabStop  = { default = 0};

	__property Touch;

	__property VerticalAlignment = { default = 2};
	__property Visible           = { default = 1};
	__property StyleElements     = { default = 7};

	__property OnAlignInsertBefore;
	__property OnAlignPosition;
	__property OnCanResize;
	__property OnClick;
	__property OnConstrainedResize;
	__property OnContextPopup;
	__property OnDockDrop;
	__property OnDockOver;
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnEndDock;
	__property OnEndDrag;
	__property OnEnter;
	__property OnExit;
	__property OnGesture;
	__property OnGetSiteInfo;
	__property OnMouseActivate;
	__property OnMouseDown;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnResize;
	__property OnStartDock;
	__property OnStartDrag;
	__property OnUnDock;
};
// ---------------------------------------------------------------------------
#endif
