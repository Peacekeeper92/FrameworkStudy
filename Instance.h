#pragma once

using namespace std;

class Instance
{
private:
	string* Name;
	size_t IID; // Generated by it's own pointer (hash)

	unordered_map<size_t, Component*> Components;

public:

	template<class _Ty>
	inline void AddComponent()
	{
		auto Memory = MemoryBank::GetInstance();
		if (!Memory->FindComponent(typeid(_Ty).hash_code()))
		{
			DebugLog(L_WARNING, "Cannot find component");
			return;
		}

		shared_ptr<_Ty> Comp = make_shared<_Ty>();

		Components.insert_or_assign(typeid(_Ty).hash_code(), (Component*)&*Comp);
		((Component*)&*Comp)->Init();
	}

	template<class _Ty>
	inline _Ty* const GetComponent()
	{
		auto CID = typeid(_Ty).hash_code();
		if (Components.find(CID) != Components.end())
			return (_Ty*)Components[CID];

		DebugLog(L_ERROR, typeid(_Ty).name() + string(" does not exist."));

		return nullptr;
	}

	template<class _Ty>
	inline void RemoveComponent()
	{
		if (!Components.find(typeid(_Ty).hash_code()))
		{
			DebugLog(L_WARNING, "This instance doesn't have this component.");
			return;
		}

		Components.erase(typeid(_Ty).hash_code());

		return;
	}

	Instance();
	~Instance();
};

