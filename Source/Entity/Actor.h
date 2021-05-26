#pragma once

#include <glm/glm.hpp>

#include <utility>
#include <memory>

class SpriteRenderer;
class Sprite;
class PhysicActor;
enum class ActorState;

class Actor: public std::enable_shared_from_this<Actor>
{
protected:
	
	Actor(glm::vec2 pos, glm::vec3 size, std::unique_ptr<Sprite> sprite, glm::vec3 color = glm::vec3(1.0f));

public:
	
	virtual ~Actor() = 0;

	virtual void BeginPlay();
	virtual void Update(float deltaTime, glm::vec4 playerAttackHitbox);
	virtual void Draw(SpriteRenderer &renderer, double deltatime);
	virtual void TakeDamage();
	virtual void Move(float deltaTime, glm::vec2 direction);
	virtual void Destroy();

	virtual bool IsPlayer();

	virtual void SetState(ActorState state);
	void SetActive(bool newActive);
	void SetDelete(bool newDelete);
	void SetColor(glm::vec3 color);

	inline bool IsActive() { return m_active; }
	inline bool IsDelete() { return m_delete; }
	inline std::shared_ptr<PhysicActor> GetActorCollider() { return ActorCollider; }

	inline glm::vec2 GetPosition() { return Position; }
	void SetPosition(glm::vec2 pos);
	bool IsAttacked(glm::vec4 attackHitbox);

	virtual void OnBeginOverlapFunction(std::shared_ptr<PhysicActor> other);

	virtual void OnEndOverlapFunction(std::shared_ptr<PhysicActor> other);

	float ZIndex;
	bool IsDestroyable;
	bool IsDestroyed;

private:

	Actor();

protected:
	// actor state
	glm::vec2 Position;
	glm::vec3 m_scale;
	float m_rotAngle;
	glm::vec3 m_rotAxis;
	glm::vec3 m_color;

	bool m_active;

	// Actor should be deleted on restart
	bool m_delete;

	// physics pointer
	std::shared_ptr<PhysicActor> ActorCollider = nullptr;

	std::unique_ptr<Sprite> ActorSprite;

	ActorState State;
	ActorState LastState;
	float AnimationProgress;

	float Speed = 200;
};
