#pragma once
class Telegram;

namespace GameEngine
{
	class BaseEntity 
	{
		friend class EntityManager;
		
	public:
		
		int ID() const { return UniqueID; }
		
		BaseEntity(int id);

		virtual bool HandleMessage(const Telegram& msg) = 0;

		virtual ~BaseEntity() = default;

	private:
		int UniqueID;
		static int NextID;
		void SetID(int id);
	};
}
