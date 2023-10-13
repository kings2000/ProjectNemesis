#pragma once
#include "MovementState.h"

class GroundState : public MovementState
{
public:

	virtual void Enter() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void Exit() override;
};
