// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"
/**
 * 
 */
UCLASS()
class RPGGAMEPRACTICE_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

		AMyGameMode();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> PlayerRecharge;

#pragma region Spawn_Position
	float Spawn_Z = 500.0f;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Min;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Max;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Min;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Max;

#pragma endregion

#pragma region Spawn_Delay
	UPROPERTY(EditAnywhere)
		float Spawn_T_Min;
	UPROPERTY(EditAnywhere)
		float Spawn_T_Max;
#pragma endregion



	void SpawnPlayerRecharge();


};
