// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Controller.generated.h"

UCLASS()
class RPGGAMEPRACTICE_API ACharacter_Controller : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Controller();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



#pragma region Camera
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* CameraComp;

#pragma endregion

#pragma region Movement
	void MoveForward(float Axis);
	void MoveRight(float Axis);


	bool bDead;
#pragma endregion




};
