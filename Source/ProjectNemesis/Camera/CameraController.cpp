// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraController.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACameraController::ACameraController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(SceneComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
}

// Called when the game starts or when spawned
void ACameraController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CanFollow == false) return;
	
	if (Player != nullptr)
	{
		FVector to = Player->GetActorLocation();
		FVector from = GetActorLocation();
		to.Z = from.Z;
		FVector newPos =  FMath::Lerp(from, to, DeltaTime * followSpeed);
		SetActorLocation(newPos);
		
	}
}

void ACameraController::SetTarget(AActor* hero)
{
	Player = hero;
	
}

