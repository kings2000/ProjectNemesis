// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimationInstance.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectNemesis/MovementController/PlayerMovement.h"
#include "ProjectNemesis/Player/Hero.h"
#include "ProjectNemesis/Utils/DebugMessage.h"

UHeroAnimationInstance::UHeroAnimationInstance()
{
	Speed = 0;
}

void UHeroAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Hero = Cast<AHero>(TryGetPawnOwner());
	
}

void UHeroAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(PlayerMovement )
	{
		Speed = PlayerMovement->Velocity.Y;
		IsFalling = (PlayerMovement->bIsGrounded == false) && PlayerMovement->Velocity.Z < 0;
		//printfk(1, "Speed: %f", Speed);
	}
	else
	{
		//TArray<AActor*> OutActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHero::StaticClass(), OutActors);
		Hero = Cast<AHero>(TryGetPawnOwner());
		if(Hero)
		{
			PlayerMovement = Hero->GetPlayerMovment();
		}
	}

	
	
}
