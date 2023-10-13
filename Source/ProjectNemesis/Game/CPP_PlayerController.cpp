// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectNemesis/Camera/CameraController.h"

void ACPP_PlayerController::SetCurrentControlledActor(APawn* actor)
{
	player = actor;

	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraController::StaticClass(), cameras);

	if(cameras.Num() > 0)
	{
		ACameraController* cam = Cast<ACameraController>(cameras[0]);
		if (cam == nullptr) { return; }
		cam->SetTarget(player);
		SetViewTarget(cam);
	}
}

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

