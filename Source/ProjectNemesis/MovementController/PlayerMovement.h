// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovement.generated.h"


class UCollisionDetector;
class MovementState;
class AHero;
struct FInputActionValue;

USTRUCT()
struct FJumpData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxJumpHeight = 120;
	UPROPERTY(EditAnywhere)
	float MinJumpHeight = 10;
	UPROPERTY(EditAnywhere)
	float TimeToJumpApex = 2;
	UPROPERTY(EditAnywhere)
	float JumpInputWindow = .15f;
	UPROPERTY(EditAnywhere)
	int MaxJumpCount = 1;
};

USTRUCT()
struct FMovementData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Speed = 10;
	UPROPERTY(EditAnywhere)
		float Acceleration = 1;
	UPROPERTY(EditAnywhere)
		float Deceleration = 1;
};

USTRUCT()
struct FWallJumpData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FVector2D WallJumpClimb = FVector2d(12, 20);
	UPROPERTY(EditAnywhere)
		FVector2D WallJumpOff = FVector2d(9, 7);
	UPROPERTY(EditAnywhere)
		FVector2D WallLeep = FVector2d(17, 15);
	UPROPERTY(EditAnywhere)
		float MaxWallSlideSpeed = 2;
	UPROPERTY(EditAnywhere)
		float WallStickTime = 0.25;
};

USTRUCT()
struct FDashData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		float DashDistance = 10;
	UPROPERTY(EditAnywhere)
		float DashTimeToApex = 2;
	UPROPERTY(EditAnywhere)
		float DashCoolDownTime = 2;
	UPROPERTY(EditAnywhere)
		float DashEndAirGraceTime = 0.5;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNEMESIS_API UPlayerMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerMovement();
	
	UPROPERTY(EditAnywhere)
		float Gravity = -49;
	
	UPROPERTY(EditAnywhere)
		FMovementData GroundMovementData;
	UPROPERTY(EditAnywhere)
		FMovementData AirMovementData;
	UPROPERTY(EditAnywhere)
		FJumpData JumpData;
	UPROPERTY(EditAnywhere)
		FWallJumpData WallJumpData;	
	UPROPERTY(EditAnywhere)
		FDashData DashData;


	UPROPERTY(BlueprintReadOnly)
		float CurrentSpeed;
	UPROPERTY(BlueprintReadOnly)
		FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
		bool bIsDashing = false;
	UPROPERTY(BlueprintReadOnly)
		bool bWallSliding;
	UPROPERTY(BlueprintReadOnly)
		int WallDirY;
	UPROPERTY(BlueprintReadOnly)
		bool bIsGrounded = true;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	int FaceDirection = 1;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveInput(const FInputActionValue& Value);
	FORCEINLINE void JumpInput()
	{
		bJumpInputDown = true;
		JumpFrameWindow = JumpData.JumpInputWindow;
	}
	FORCEINLINE void StopJumpingInput() {bJumpInputUp = true;}
	void Dash();

private:
	AHero* Player;
	MovementState* CurrentMovementState;
	UCollisionDetector* CollisionDetector;

	
	float Acceleration;
	float MaxJumpVelocity;
	float MinJumpVelocity;
	FVector DirectionalInput;

	float VelocityYSmoothing;
	float TimeToWallUnstick;
	
	
	bool bJumpInputDown = false;
	bool bJumpInputUp = false;
	float JumpFrameWindow = 0;
	float MaxTimeBtwHumps = 0;
	int CurrentJumpCount = 0;
	float MovementInput;

	bool CanDash = true;
	
	float DashCooldownTime;
	float DashTime;
	float DashVelocity;
	
	
	void Movement();
	void CalculateVelocity();
	void UpdateMovementData();
	void CalculateJump();
	void HandleWallSliding();
	void CheckDashCapability();
};
