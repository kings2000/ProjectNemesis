// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectNemesis/Game/CPP_PlayerController.h"
#include "ProjectNemesis/MovementController/CollisionDetector.h"
#include "ProjectNemesis/MovementController/PlayerMovement.h"


// Sets default values
AHero::AHero()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Root");
	RootComponent = CapsuleComponent;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ForwardArrow");
	ArrowComponent->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	//PlayerMovementComponent = CreateDefaultSubobject<UPlayerMovement>("PlayerMovement");
	
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	PlayerMovementComponent = Cast<UPlayerMovement>(GetComponentByClass(UPlayerMovement::StaticClass()));
	CollisionDetector = Cast<UCollisionDetector>(GetComponentByClass(UCollisionDetector::StaticClass()));
	
	ACPP_PlayerController* PlayerController = Cast<ACPP_PlayerController>(Controller);
	PlayerController->SetCurrentControlledActor(this);
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerMovementComponent = Cast<UPlayerMovement>(GetComponentByClass(UPlayerMovement::StaticClass()));
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		//// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerMovementComponent, &UPlayerMovement::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerMovementComponent, &UPlayerMovement::StopJumpingInput);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerMovementComponent, &UPlayerMovement::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, PlayerMovementComponent, &UPlayerMovement::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, PlayerMovementComponent, &UPlayerMovement::MoveInput);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, PlayerMovementComponent, &UPlayerMovement::Dash);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

