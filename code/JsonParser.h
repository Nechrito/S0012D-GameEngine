#pragma once
#include "io/ioserver.h"
#include "util/stringatom.h"
#include "io/jsonreader.h"
#include "io/jsonwriter.h"
#include "BaseEntity.h"
#include "io/filestream.h"

namespace GameEngine
{
	class JsonParser final : public Core::RefCounted
	{
		__DeclareClass(JsonParser)
		__DeclareSingleton(JsonParser)

	public:
		JsonParser();
		~JsonParser();

		void Write(Util::Array<BaseEntity*> entities);
		void Write(BaseEntity* entity);
		void Read(const Util::StringAtom& str);

	private:
		IO::IoServer* IoInstance;
		Util::String uri;

		IO::FileStream* fileStream;
		IO::JsonReader* reader;
		IO::JsonWriter* writer;
	};
}
