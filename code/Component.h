#pragma once
#include "util/stringatom.h"


// Forward declaration avoids the parsing the compiler would have to do during an #include "..."
// A better optimization would ofc be pre-compiled headers
struct Telegram;

namespace GameEngine
{
	class BaseEntity;

	class Component
	{
	public:
		
		Component() : Owner(nullptr), Initialized(false) { }

		virtual void SetOwner(BaseEntity* entity);
		virtual void SetIdentifier(const Util::StringAtom& value);
		virtual Util::StringAtom GetIdentifier();

		virtual void Init();
		virtual void Update();
		virtual void Shutdown();
		virtual bool HandleMessage(const Telegram& msg) = 0;
		
		virtual ~Component();

	protected:
		BaseEntity* Owner;
		Util::StringAtom Identifier;
		bool Initialized;
	};

	inline void Component::SetOwner(BaseEntity* entity)
	{
		this->Owner = entity;
	}

	inline void Component::SetIdentifier(const Util::StringAtom& value)
	{
		this->Identifier = value;
	}

	inline Util::StringAtom Component::GetIdentifier()
	{
		return Identifier;
	}

	inline void Component::Init()
	{
		this->Initialized = true;
	}

	inline void Component::Update()
	{
	}

	inline void Component::Shutdown()
	{
		n_assert(Initialized);
		this->Initialized = false;
	}

	inline Component::~Component()
	{
		n_assert(!Initialized);
	}
}