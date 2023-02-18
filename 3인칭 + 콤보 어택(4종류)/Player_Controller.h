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

	// �� �� �ۿ� ��� �����Ϸ� �ܿ��� ���� �ϴٰ�� ��
	FORCEINLINE void LMB_Up() { bLMB_down = false; }
	
	void Attack();		

	UFUNCTION(BlueprintCallable)
	void AttackEnd();	

	/** �޺� ������ ���� �Ͽ��� �� -> ���� ��� �߿� ���콺�� ������ �� */
	UFUNCTION(BlueprintCallable)
	void ComboAttackCheck();	
	
	// �޺� ���� ���� ����.			max value = = �޺� ���� ����
	int Combocount;

	// ���� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anims")
	bool bAttacking;
	
	// ���� ��ư ������ ��
	bool bLMB_down;

	// �޺� ������ �ؾ� �ϴ� ��
	bool bComboAttack;
#pragma endregion
};
