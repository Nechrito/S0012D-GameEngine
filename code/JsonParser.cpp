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

	Util::String JsonParser::GetFileName(BaseEntity* entity)
	{
		IoInstance = IO::IoServer::Instance();
		
		const IO::URI directory = "root:cache";
		if (!IoInstance->DirectoryExists(directory))
			IoInstance->CreateDirectoryA(directory);


		Util::String fileName(directory.AsString() + "/");
		fileName.Append(entity->Name.AsString());
		fileName.AppendInt(entity->UniqueID);
		fileName.Append(".json");
		
		return fileName;
	}

	void JsonParser::Write(const Util::Array<BaseEntity*>& entities)
	{
		for (auto& entity : entities)
		{
			Write(entity);
		}
	}

	void JsonParser::Write(BaseEntity* entity)
	{
		if (!entity)
			return;

		const Util::String fileName = GetFileName(entity);

		if (IoInstance->FileExists(fileName))
			IoInstance->DeleteFile(fileName);

		writer = IO::JsonWriter::Create();

		fileStream = IO::FileStream::Create();
		fileStream->SetURI(fileName);
		fileStream->Open();

		writer->SetStream(fileStream);
		writer->Open();

		// todo: write "Entity" as root with "components" as parent whereas TransformComponent and Graphicscomponent are the children including their properties
		writer->BeginObject("Entity");

		TransformComponent* transform = dynamic_cast<TransformComponent*>(entity->GetComponent("Transform"));
		GraphicsComponent* graphics = dynamic_cast<GraphicsComponent*>(entity->GetComponent("Graphics"));

		// transform
		writer->Add(transform->Transform, "Transform");
		writer->Add(transform->Velocity, "Velocity");

		// graphics
		writer->Add(graphics->Resource.AsString(), "Resource");
		writer->Add(graphics->ID.id, "Graphics ID");

		writer->End();
		writer->Close();
	}

	void JsonParser::ReadAll(const Util::Array<BaseEntity*>& entities)
	{
		for (auto& entity : entities)
		{
			Read(entity);
		}
	}

	BaseEntity* JsonParser::Read(BaseEntity* entity)
	{
		const Util::String fileName = GetFileName(entity);
		if (!IoInstance->FileExists(fileName))
		{
			return entity;
		}

		reader = IO::JsonReader::Create();
		fileStream = IO::FileStream::Create();
		fileStream->SetURI(fileName);
		fileStream->Open();

		reader->SetStream(fileStream);
		reader->Open();
		reader->SetToNode("Entity");
		
		TransformComponent* transform = dynamic_cast<TransformComponent*>(entity->GetComponent("Transform"));
		GraphicsComponent* graphics = dynamic_cast<GraphicsComponent*>(entity->GetComponent("Graphics"));

		transform->Transform = reader->GetTransform44("Transform");
		transform->Velocity = reader->GetFloat4("Velocity");

		graphics->Resource = reader->GetString("Resource");
		graphics->ID = reader->GetInt("Graphics ID");

		reader->Close();
		fileStream->Close();
		
		return entity;
	}
}