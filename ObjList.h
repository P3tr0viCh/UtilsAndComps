// ---------------------------------------------------------------------------

#ifndef ObjListH
#define ObjListH

#include <System.Contnrs.hpp>

// ---------------------------------------------------------------------------
template<typename T>
class TObjList : public TObjectList {
protected:
	T* __fastcall GetItem(int Index) {
		return (T*)TObjectList::GetItem(Index);
	}

	void __fastcall SetItem(int Index, T* AObject) {
		TObjectList::SetItem(Index, AObject);
	}

public:
	int __fastcall Add(T* AObject) {
		return TObjectList::Add(AObject);
	}

	int __fastcall Remove(T* AObject) {
		return TObjectList::Remove(AObject);
	}

	void __fastcall Assign(TObjList* ObjList) {
		Clear();

		T* AObject;

		for (int i = 0; i < ObjList->Count; i++) {
			AObject = new T();

			AObject->Assign(ObjList->Items[i]);

			Add(AObject);
		}
	}

	String __fastcall ToString() {
		String S;

		S = "TObjList";
		S += "{";

		for (int i = 0; i < Count; i++) {
			S += sLineBreak;
			S += "Item" + IntToStr(i) + "='" + Items[i]->ToString() + "'";
		}
		S += "}";

		return S;
	}

	T* operator[](int Index) {
		return Items[Index];
	}

	__property T* Items[int Index] = {read = GetItem, write = SetItem};
};

// ---------------------------------------------------------------------------
#endif
