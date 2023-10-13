#pragma once

class AHero;

class MovementState
{
public:
	MovementState();
	virtual  ~MovementState();

public:
	virtual void Enter();
	virtual void ChangeState(MovementState* nextState);
	virtual void Tick(const float DeltaTime);
	virtual void Exit();

	AActor* player;
};

