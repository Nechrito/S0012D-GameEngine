#include "stdneb.h"
#include "JsonParser.h"
#include "io/uri.h"
#include "io/ioserver.h"
#include <string>

namespace GameEngine
{
	__ImplementClass(JsonParser, 'GEJP', Core::RefCounted)
	__ImplementSingleton(JsonParser)
	
	JsonParser::JsonParser() : IoInstance(nullptr), uri("root:")
	{
		__ConstructSingleton
	}

	JsonParser::~JsonParser()
	{
		__DestructSingleton
	}

	void JsonParser::Write(Util::Array<BaseEntity*> entities)
	{
		writer = IO::JsonWriter::Create();
		
		for (auto& entity : entities)
		{
			Write(entity);
		}
	}

	void JsonParser::Write(BaseEntity* entity)
	{
		if (!entity)
			return;
		
		if (!IoInstance)
			IoInstance = IO::IoServer::Instance();


		Util::String fileName(uri);
		fileName.Append(entity->Name.AsString());
		fileName.AppendInt(entity->UniqueID);

		if (IoInstance->FileExists(fileName))
			IoInstance->DeleteFile(fileName);

		if (!writer)
			writer = IO::JsonWriter::Create();

		if (!fileStream)
			fileStream = IO::FileStream::Create();

		fileStream->SetURI(fileName);

		writer->SetStream(fileStream);
		writer->Open();

		// todo....
	}

	void JsonParser::Read(const Util::StringAtom& str)
	{
		
		if (!IoInstance)
			IoInstance = IO::IoServer::Instance();


	}
}

