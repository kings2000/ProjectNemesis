// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hero.generated.h"

class UArrowComponent;
class UCapsuleComponent;
class UPlayerMovement;
class UNiagaraComponent;
class UNiagaraSystem;
class UInputAction;
class USkeletalMeshComponent;

UCLASS()
class PROJECTNEMESIS_API AHero : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHero();

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComponent;

	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* DashAction;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	   FORCEINLINE	UPlayerMovement* GetPlayerMovment() {return  PlayerMovementComponent;}
	UFUNCTION(BlueprintCallable)
		FORCEINLINE UCapsuleComponent* GetCapsuleComponent() {return  CapsuleComponent;}

private:
	class UPlayerMovement* PlayerMovementComponent;
	class UCollisionDetector* CollisionDetector;

	
};
