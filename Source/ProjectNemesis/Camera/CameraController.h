// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraController.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTNEMESIS_API ACameraController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraVariable, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraVariable, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraVariable, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraVariable, meta = (AllowPrivateAccess = "true"))
		float followSpeed = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraVariable, meta = (AllowPrivateAccess = "true"))
		bool CanFollow;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* hero);

	class AActor* Player;
};
