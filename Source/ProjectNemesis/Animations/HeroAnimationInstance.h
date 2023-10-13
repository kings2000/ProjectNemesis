// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimationInstance.generated.h"

class UPlayerMovement;
class AHero;
/**
 * 
 */
UCLASS()
class PROJECTNEMESIS_API UHeroAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHeroAnimationInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation Property")
		AHero* Hero;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation Property")
		UPlayerMovement* PlayerMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation Property")
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation Property")
		bool IsFalling;
};
