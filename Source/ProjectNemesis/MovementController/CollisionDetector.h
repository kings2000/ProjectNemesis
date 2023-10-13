// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionDetector.generated.h"

class AHero;
class UPlayerMovement;

USTRUCT()
struct FCollisionInfo
{
	GENERATED_BODY()

public:

	bool Above, Below;
	bool Left, Right;
};

USTRUCT()
struct FRayCastOrigins
{
	GENERATED_BODY()

	FVector topLeft, topRight;
	FVector bottomLeft, bottomRight;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNEMESIS_API UCollisionDetector : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCollisionDetector();
	
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FCollisionInfo CollisionInfo;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void UpdateRaycastOrigins();
	void VerticalCollision(FVector& Movement);
	void HorizontalCollision(FVector& Movement);
	void CalculateRaySpacing();
	void ResetCollisionInfo();
	
private:
	int HorizontalRayCount;
	int VerticalRayCount;
	float HorizontalRaySpacing;
	float VerticalRaySpacing;
	
		
	const double SkinWidth = 1.5;
	const double DSTBetweenRays = 25;

	AHero* Player;
	UPlayerMovement* PlayerMovement;
	
	FRayCastOrigins RaycastOrigins;
	FCollisionQueryParams QueryParams;
};
