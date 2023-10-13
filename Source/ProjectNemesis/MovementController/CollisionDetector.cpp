// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionDetector.h"

#include "PlayerMovement.h"
#include "Components/CapsuleComponent.h"
#include "ProjectNemesis/Player/Hero.h"
#include "ProjectNemesis/Utils/DebugMessage.h"


// Sets default values for this component's properties
UCollisionDetector::UCollisionDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCollisionDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PlayerMovement = Cast<UPlayerMovement>(GetOwner()->GetComponentByClass(UPlayerMovement::StaticClass()));
	Player = Cast<AHero>(GetOwner());
	QueryParams.AddIgnoredActor(Player);
	CalculateRaySpacing();
	
}


// Called every frame
void UCollisionDetector::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCollisionDetector::UpdateRaycastOrigins()
{
	FBoxSphereBounds bounds = Player->CapsuleComponent->Bounds;
	FBoxSphereBounds newBox = bounds.ExpandBy(SkinWidth * -2.0);


	RaycastOrigins.bottomLeft = FVector(newBox.Origin.X,newBox.Origin.Y - newBox.BoxExtent.Y, newBox.Origin.Z - newBox.BoxExtent.Z);
	RaycastOrigins.bottomRight = FVector(newBox.Origin.X,newBox.Origin.Y + newBox.BoxExtent.Y, newBox.Origin.Z - newBox.BoxExtent.Z);
	RaycastOrigins.topLeft = FVector(newBox.Origin.X, newBox.Origin.Y - newBox.BoxExtent.Y, newBox.Origin.Z + newBox.BoxExtent.Z);
	RaycastOrigins.topRight = FVector(newBox.Origin.X, newBox.Origin.Y + newBox.BoxExtent.Y, newBox.Origin.Z + newBox.BoxExtent.Z);
}

void UCollisionDetector::CalculateRaySpacing()
{
	FBoxSphereBounds bounds = Player->CapsuleComponent->Bounds;
	FBoxSphereBounds newBox = bounds.ExpandBy(SkinWidth * -2.0);

	float boundWidth = newBox.BoxExtent.Y * 2;
	float boundHeight = newBox.BoxExtent.Z * 2;
	
	HorizontalRayCount = FMath::RoundToInt(boundHeight / DSTBetweenRays);
	VerticalRayCount = FMath::RoundToInt(boundWidth / DSTBetweenRays);
	
	HorizontalRaySpacing = boundHeight / (HorizontalRayCount - 1);
	VerticalRaySpacing = boundWidth / (VerticalRayCount - 1);
	
}

void UCollisionDetector::ResetCollisionInfo()
{

	CollisionInfo.Above = false;
	CollisionInfo.Below = false;
	CollisionInfo.Left = false;
	CollisionInfo.Right = false;
	
}

void UCollisionDetector::VerticalCollision(FVector& Movement)
{
	if(Movement.Z == 0) return;
	
	FHitResult Hit;
	float directionZ = FMath::Sign(Movement.Z);
	float rayLenght = FMath::Abs(Movement.Z) + SkinWidth;
	
	for (int i = 0; i < VerticalRayCount; ++i)
	{

		FVector rayOrigin = (directionZ == -1)? RaycastOrigins.bottomLeft : RaycastOrigins.topLeft;
		rayOrigin += FVector::RightVector * (VerticalRaySpacing * i + Movement.X);
		FVector TraceEnd = (rayOrigin + FVector::UpVector * directionZ * rayLenght);
		DrawDebugLine(GetWorld(), rayOrigin, TraceEnd, FColor::Red, false, .2, 0, 1);
		GetWorld()->LineTraceSingleByChannel(Hit, rayOrigin, TraceEnd, TraceChannelProperty, QueryParams);

		if(Hit.bBlockingHit && IsValid(Hit.GetActor()))
		{
			Movement.Z = (Hit.Distance - SkinWidth) * directionZ;
			rayLenght = Hit.Distance;
	
			CollisionInfo.Below = directionZ == -1;
			CollisionInfo.Above = directionZ == 1;

			//printf("Hit");
		}
	}
	
	
}

void UCollisionDetector::HorizontalCollision(FVector& Movement)
{
	
	float directionY = PlayerMovement->FaceDirection;
	float rayLenght = FMath::Abs(Movement.Y) + SkinWidth;
	if(FMath::Abs(Movement.Y) < SkinWidth)
	{
		rayLenght = 2 * SkinWidth;
	}

	FHitResult Hit;
	for (int i = 0; i < HorizontalRayCount; ++i)
	{
		FVector rayOrigin = (directionY == -1)? RaycastOrigins.bottomLeft : RaycastOrigins.bottomRight;
		rayOrigin += FVector::UpVector * (HorizontalRaySpacing * i);
		FVector TraceEnd = (rayOrigin + FVector::RightVector * directionY * rayLenght);
		DrawDebugLine(GetWorld(), rayOrigin, TraceEnd, FColor::Red, false, .2, 0, 1);
		GetWorld()->LineTraceSingleByChannel(Hit, rayOrigin, TraceEnd, TraceChannelProperty, QueryParams);

		if(Hit.bBlockingHit && IsValid(Hit.GetActor()))
		{
			if(Hit.Distance == 0)
				continue;

			Movement.Y = (Hit.Distance - SkinWidth) * directionY;
			rayLenght = Hit.Distance;

			CollisionInfo.Left = directionY == -1;
			CollisionInfo.Right = directionY == 1;
			//printf("Facing dir %d", FaceDirection);
		}
	}
}



