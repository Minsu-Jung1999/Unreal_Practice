// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Controller.generated.h"




UCLASS()
class GAMEPRACTICING_API APlayer_Controller : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Controller();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* FollowCam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

#pragma region Attack
	void LMB_Down();	

	// 한 줄 밖에 없어서 컴파일러 단에서 실행 하다고록 함
	FORCEINLINE void LMB_Up() { bLMB_down = false; }
	
	void Attack();		

	UFUNCTION(BlueprintCallable)
	void AttackEnd();	

	/** 콤보 공격을 실행 하였을 때 -> 공격 모션 중에 마우스를 눌렀을 때 */
	UFUNCTION(BlueprintCallable)
	void ComboAttackCheck();	
	
	// 콤보 공격 개수 증가.			max value = = 콤보 공격 개수
	int Combocount;

	// 공격 중인 가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anims")
	bool bAttacking;
	
	// 공격 버튼 눌렀는 가
	bool bLMB_down;

	// 콤보 공격을 해야 하는 가
	bool bComboAttack;
#pragma endregion
};
