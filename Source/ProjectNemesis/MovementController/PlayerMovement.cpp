// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovement.h"

#include "CollisionDetector.h"
#include "InputActionValue.h"
#include "GameFramework/PhysicsVolume.h"
#include "MovementState/GroundState.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "ProjectNemesis/Player/Hero.h"
#include "ProjectNemesis/Utils/DebugMessage.h"

FVector MoveTo(FVector& From, FVector& To, float& Time);
float MoveTo(const float& From, const float& To, const float& MaxDistanceDelta);

// Sets default values for this component's properties
UPlayerMovement::UPlayerMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMovement::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AHero>(GetOwner());

	CollisionDetector = Cast<UCollisionDetector>(Player->GetComponentByClass(UCollisionDetector::StaticClass()));

	const float jumpGravity = -(2 * JumpData.MaxJumpHeight) / FMath::Pow(JumpData.TimeToJumpApex, 2);
	MaxJumpVelocity = FMath::Abs(jumpGravity) * JumpData.TimeToJumpApex;
	MinJumpVelocity = FMath::Sqrt(2 * FMath::Abs(jumpGravity) * MinJumpVelocity);
	
	const float DashForce = (2 * DashData.DashDistance) / FMath::Pow(DashData.DashTimeToApex, 2);
	DashVelocity = FMath::Abs(DashForce) * DashData.DashTimeToApex;
}


// Called every frame
void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Movement();

	
}



void UPlayerMovement::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementInput = MovementVector.Y;
	
}

void UPlayerMovement::Dash()
{
	if(CanDash == false) return;
	if(DashCooldownTime > 0) return;

	CanDash = false;
	DashCooldownTime = DashData.DashCoolDownTime;
	
	if(CollisionDetector->CollisionInfo.Left == true || CollisionDetector->CollisionInfo.Right == true)
	{
		if(MovementInput > 0 && WallDirY < 0 || (MovementInput < 0 && WallDirY > 0))
		{
			TimeToWallUnstick = 0;
			Velocity.Y = -WallDirY * DashVelocity;
		}
		else
		{
			Velocity.Y = FaceDirection * DashVelocity;
		}
	}
	else
	{
		Velocity.Y = FaceDirection * DashVelocity;
	}
	
	
	DashTime = (DashData.DashTimeToApex) + DashData.DashEndAirGraceTime;
	printfk(1, "Dash Velocity: %f", DashVelocity);
	printfk(2, "Dash Time: %f", DashTime);
}

void UPlayerMovement::Movement()
{
	if(Player == nullptr || CollisionDetector == nullptr) return;

	bIsGrounded = CollisionDetector->CollisionInfo.Below;
	
	CheckDashCapability();
	CalculateJump();
	UpdateMovementData();
	CalculateVelocity();
	HandleWallSliding();
	
	CollisionDetector->UpdateRaycastOrigins();
	CollisionDetector->ResetCollisionInfo();

	if(Velocity.Y != 0)
	{
		FaceDirection = FMath::Sign(Velocity.Y);
	}
	
	CollisionDetector->HorizontalCollision(Velocity);
	CollisionDetector->VerticalCollision(Velocity);

	
	
	Player->AddActorWorldOffset(Velocity);

	if(CollisionDetector != nullptr)
	{
		if(CollisionDetector->CollisionInfo.Above || CollisionDetector->CollisionInfo.Below)
		{
			Velocity.Z = 0;
		}
	}
}

void UPlayerMovement::CalculateVelocity()
{
	const float TargetVelocity = MovementInput * CurrentSpeed;
	
	if(DashTime > 0 && CollisionDetector->CollisionInfo.Left == false && CollisionDetector->CollisionInfo.Right == false)
	{
		bIsDashing = true;
		DashTime -= GetWorld()->DeltaTimeSeconds;
		Velocity.Z = 0;
		
	}else
	{
		if(bIsDashing)
		{
			Velocity.Y = 0;
		}
		DashTime = 0;
		bIsDashing = false;
	}
	
	Velocity.Y = MoveTo(Velocity.Y, TargetVelocity, Acceleration);
	
	if(bIsDashing) return;
		Velocity.Z += Gravity * GetWorld()->DeltaTimeSeconds;
}

void UPlayerMovement::UpdateMovementData()
{

	if(bIsGrounded)
	{
		Acceleration = (MovementInput == 0) ? GroundMovementData.Deceleration : GroundMovementData.Acceleration;
		CurrentSpeed = GroundMovementData.Speed;
	}
	else
	{
		Acceleration = (MovementInput == 0) ? AirMovementData.Deceleration : AirMovementData.Acceleration;
		CurrentSpeed = AirMovementData.Speed;
	}
	
}

void UPlayerMovement::CalculateJump()
{
	JumpFrameWindow -= GetWorld()->DeltaTimeSeconds;
	MaxTimeBtwHumps -= GetWorld()->DeltaTimeSeconds;
	
	if(bJumpInputUp && Velocity.Z > MinJumpVelocity)
	{
		Velocity.Z = MinJumpVelocity;
		bJumpInputUp = false;
		return;
	}

	const bool canJump = (CurrentJumpCount < JumpData.MaxJumpCount) && MaxTimeBtwHumps <= 0;
	
	
	if((bWallSliding && canJump) &&  (bJumpInputDown || JumpFrameWindow > 0))
	{
		//print("Wall Jump");
		
		if(WallDirY == FMath::RoundToInt(MovementInput))
		{
			Velocity.Y = -WallDirY * WallJumpData.WallJumpClimb.X;
			Velocity.Z = WallJumpData.WallJumpClimb.Y;
		}else if(FMath::RoundToInt(MovementInput) == 0)
		{
			Velocity.Y = -WallDirY * WallJumpData.WallJumpOff.X;
			Velocity.Z = WallJumpData.WallJumpOff.Y;
		}
		else
		{
			Velocity.Y = -WallDirY * WallJumpData.WallLeep.X;
			Velocity.Z = WallJumpData.WallLeep.Y;
		}
		CurrentJumpCount++;
		JumpFrameWindow = 0;
		MaxTimeBtwHumps = 0.05;
		//printf("Frame Wall: %d",CurrentJumpCount );
		bJumpInputUp = false;
		bJumpInputDown = false;
		return;
	}
	
	if(((bJumpInputDown && CollisionDetector->CollisionInfo.Below ) ||
		(JumpFrameWindow > 0 && CollisionDetector->CollisionInfo.Below)) ||
		(bJumpInputDown && canJump))
	{
		Velocity.Z = MaxJumpVelocity / 5;
		JumpFrameWindow = 0;
		CurrentJumpCount++;
		MaxTimeBtwHumps = 0.05;
		//printf("Frame Ground: %d",CurrentJumpCount );
		
	}
	
	bJumpInputUp = false;
	bJumpInputDown = false;
	
	if((CollisionDetector->CollisionInfo.Below || bWallSliding) && Velocity.Z <= 0 && CurrentJumpCount > 0)
	{
		CurrentJumpCount = 0;
		//printf("Frame Reset: %d",CurrentJumpCount );
	}
	
	
}

void UPlayerMovement::HandleWallSliding()
{
	WallDirY = CollisionDetector->CollisionInfo.Left ? -1 : 1;
	bWallSliding = false;

	
	
	if((CollisionDetector->CollisionInfo.Left || CollisionDetector->CollisionInfo.Right) &&
		!CollisionDetector->CollisionInfo.Below && Velocity.Z < 0)
	{
		///printfk(1,"Sliding to ameraica %hs", (CollisionDetector->CollisionInfo.Left || CollisionDetector->CollisionInfo.Right) ? "True" : "False");
		
		bWallSliding = true;
		
		if(Velocity.Z < -WallJumpData.MaxWallSlideSpeed)
		{
			Velocity.Z = -WallJumpData.MaxWallSlideSpeed;
		}

		if(TimeToWallUnstick > 0 && bIsDashing == false)
		{
			VelocityYSmoothing = 0;
			Velocity.Y = 0;
		
			if(MovementInput != WallDirY && MovementInput != 0)
			{
				TimeToWallUnstick -= GetWorld()->DeltaTimeSeconds;
			}
			else
			{
				TimeToWallUnstick = WallJumpData.WallStickTime;
			}
		}
		else
		{
			TimeToWallUnstick = WallJumpData.WallStickTime;
		}
	}
}

void UPlayerMovement::CheckDashCapability()
{
	DashCooldownTime -= GetWorld()->DeltaTimeSeconds;
	//Dash Chack
	if(CanDash == false)
	{
		if(CollisionDetector->CollisionInfo.Below ||
			CollisionDetector->CollisionInfo.Left ||
			CollisionDetector->CollisionInfo.Right)
		{
			CanDash = true;
		}
	}
}


FVector MoveTo(const FVector& From, const FVector& To, const float MaxDistanceDelta)
{
	const FVector a = (To - From);
	const float Magnitude = a.Length();

	if(Magnitude <= MaxDistanceDelta || Magnitude == 0)
	{
		return To;
	}
	return From + a / Magnitude * MaxDistanceDelta;
}

float MoveTo(const float& From, const float& To, const float& MaxDistanceDelta)
{
	if(FMath::Abs(To - From) <= MaxDistanceDelta)
	{
		return To;
	}
	return From + FMath::Sign(To - From) * MaxDistanceDelta;
}