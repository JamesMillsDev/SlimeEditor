#pragma once

namespace Catalyst::Actors
{
	class Actor;

	class ActorComponent
	{
		friend Actor;

	public:
		[[nodiscard]] Actor* Owner() const;

	public:
		ActorComponent(const ActorComponent&) = delete;
		ActorComponent(ActorComponent&&) = delete;

	public:
		ActorComponent& operator=(const ActorComponent&) = delete;
		ActorComponent& operator=(ActorComponent&&) = delete;

	protected:
		ActorComponent();
		virtual ~ActorComponent();

	protected:
		virtual void BeginPlay();

		virtual void Tick();
		virtual void Render();

		virtual void EndPlay();

	private:
		Actor* m_owner;

	};
}