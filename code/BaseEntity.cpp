#include "BaseEntity.h"
#include <cassert>

int GameEngine::BaseEntity::NextID = 0;

GameEngine::BaseEntity::BaseEntity(const int id)
{
	this->SetID(id);
}

void GameEngine::BaseEntity::SetID(const int id)
{
	assert(id >= NextID && "Invalid ID");
	UniqueID = id;
	NextID = UniqueID + 1;
}
