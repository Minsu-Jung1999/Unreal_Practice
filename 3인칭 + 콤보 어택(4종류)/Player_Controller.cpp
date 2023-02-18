// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
APlayer_Controller::APlayer_Controller()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Instantiating Class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	//Attach the Spring Arm to the Character's Skeletal Mesh Component
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 700.0f;


	//Attach the Camera to the SpringArmComponent
	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	FollowCam->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	FollowCam->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// �̵�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// RotationRate = ENGINE_API FFindFloorResult ����ü�� �ִ� FRotator ����
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);

	// ���� ��ư Ȯ�� �� ���� �ʱ�ȭ
	bLMB_down = false;
	bAttacking = false;

	bComboAttack = false;
	Combocount = 0;
}

// Called when the game starts or when spawned
void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Controller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �� �� �� �� �̵�
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayer_Controller::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayer_Controller::MoveRight);
	
	// ���콺 ��Ʈ�� ( �¿� ���� ī�޶� �̵�)
	PlayerInputComponent->BindAxis("LookAround", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	// ����
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// ���ò�
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayer_Controller::LMB_Down);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayer_Controller::LMB_Up);


}

void APlayer_Controller::MoveForward(float Axis)
{
	if (Controller != nullptr && Axis != 0.0f && (!bAttacking))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void APlayer_Controller::MoveRight(float Axis)
{
	if (Controller != nullptr && Axis != 0.0f && (!bAttacking))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void APlayer_Controller::LMB_Down()
{
	bLMB_down = true;
	UE_LOG(LogTemp, Warning, TEXT("LMB Down()"));

	// not atttacking
	if (!bAttacking)
	{
		Attack();
	}
	else if(bAttacking)
	{
		/** Notify�� ����Ͽ� bComboAttack�� �����Ѵ�. */
		bComboAttack = true;
	}


}

/** �������� �˰��� */
void APlayer_Controller::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack( )"));
	bLMB_down = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		bAttacking = true;	// bLMB_DOWN �� ������ �����ϰ� �ִϸ��̼ǰ� ��Ÿ�ְ� ������ �Ǿ� �ִٸ� (������ ���� �׻� �̷������� Ȯ�� �� ����)

		const char* comboList[] = { "Attack_A","Attack_B","Attack_C", "Attack_D" };		// ���ڿ� ������ �迭�� ��Ÿ�ֿ� �����ߴ� ���� ��� �̸��� ����

		/** �ִϸ��̼��� ���������� ���� ��*/
		if (!(AnimInstance->Montage_IsPlaying(CombatMontage)))
		{
			AnimInstance->Montage_Play(CombatMontage);	// �ִϸ��̼��� ���������� �ʴٸ� Montage_Play�� ����ؼ� ���� �տ� �� �� �����Ѵ�.
		}
		else
		{
			AnimInstance->Montage_Play(CombatMontage);	// 
			AnimInstance->Montage_JumpToSection(FName(comboList[Combocount]), CombatMontage); // ���� �迭�� �ִ� ����� �����Ų��.
		}
	}
}

void APlayer_Controller::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd( )"));		// ���߿� �� ���� �����Ƿ� FORCEINLINE�� ���� �ʾҴ�.
	bAttacking = false;
}

void APlayer_Controller::ComboAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("ComboaAttackCheck is activate"));

	if (Combocount >= 3)	// �޺� ���� �� ���� �ʵ��� 
		Combocount = 0;

	if (bComboAttack)		// �޺� ������ ������ �ȴٸ� -> MainAnim_BP�� Event_Graph ���� ������ ���� ����Ѵ�.
	{

		Combocount++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Attack is available Combocount ++"));
		bComboAttack = false;
		Attack(); // �޺� ���� ���� 
	}
}

