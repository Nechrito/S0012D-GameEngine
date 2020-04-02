#include "stdneb.h"
#include "JsonParser.h"
#include "io/uri.h"
#include "io/ioserver.h"
#include <string>


#include "GraphicsComponent.h"
#include "TransformComponent.h"

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
		
		//if (!IoInstance)
		IoInstance = IO::IoServer::Instance();
		const IO::URI directory = "root:cache";
		if (!IoInstance->DirectoryExists(directory))
			IoInstance->CreateDirectoryA(directory);

		Util::String fileName(directory.AsString() + "/");
		fileName.Append(entity->Name.AsString());
		fileName.AppendInt(entity->UniqueID);
		fileName.Append(".json");

		if (IoInstance->FileExists(fileName))
			IoInstance->DeleteFile(fileName);

		writer = IO::JsonWriter::Create();

		fileStream = IO::FileStream::Create();

		fileStream->SetURI(fileName);
		writer->SetStream(fileStream);
		writer->Open();

		// todo: write "Entity" as root with "components" as parent whereas TransformComponent and Graphicscomponent are the children including their properties
		writer->BeginObject("Entity");

		TransformComponent* transform = dynamic_cast<TransformComponent*>(entity->GetComponent("Transform"));
		
		writer->Add(transform->Transform, "Transform");
		writer->Add(transform->Velocity, "Velocity");


		GraphicsComponent* graphics = dynamic_cast<GraphicsComponent*>(entity->GetComponent("Graphics"));
		writer->Add(graphics->Resource.AsString(), "Resource");
		writer->Add(graphics->ID.id, "Graphics ID");

		writer->End();
		writer->Close();
	}

	void JsonParser::Read(const Util::StringAtom& str)
	{
		IoInstance = IO::IoServer::Instance();


	}
}

