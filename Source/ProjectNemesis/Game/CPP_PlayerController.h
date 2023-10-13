// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class PROJECTNEMESIS_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	UFUNCTION(BlueprintCallable)
		void SetCurrentControlledActor(APawn* actor);
	
protected:
	virtual void BeginPlay() override;

private:
	APawn* player;
};
