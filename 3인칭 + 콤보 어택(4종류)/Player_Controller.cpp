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

	// 이동
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// RotationRate = ENGINE_API FFindFloorResult 구조체에 있는 FRotator 값임
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);

	// 공격 버튼 확인 불 변수 초기화
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

	// 앞 뒤 좌 우 이동
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayer_Controller::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayer_Controller::MoveRight);
	
	// 마우스 컨트롤 ( 좌우 상하 카메라 이동)
	PlayerInputComponent->BindAxis("LookAround", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	// 점프
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// 어택끄
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
		/** Notify를 사용하여 bComboAttack를 수정한다. */
		bComboAttack = true;
	}


}

/** 공격진행 알고리즘 */
void APlayer_Controller::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack( )"));
	bLMB_down = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		bAttacking = true;	// bLMB_DOWN 을 참으로 변경하고 애니메이션과 몽타주가 연결이 되어 있다면 (포인터 사용시 항상 이런식으로 확인 후 진행)

		const char* comboList[] = { "Attack_A","Attack_B","Attack_C", "Attack_D" };		// 문자열 포인터 배열로 몽타주에 저장했던 공격 모션 이름들 저장

		/** 애니메이션이 실행중이지 않을 때*/
		if (!(AnimInstance->Montage_IsPlaying(CombatMontage)))
		{
			AnimInstance->Montage_Play(CombatMontage);	// 애니메이션이 실행중이지 않다면 Montage_Play를 사용해서 가장 앞에 거 만 진행한다.
		}
		else
		{
			AnimInstance->Montage_Play(CombatMontage);	// 
			AnimInstance->Montage_JumpToSection(FName(comboList[Combocount]), CombatMontage); // 다음 배열에 있는 모션을 실행시킨다.
		}
	}
}

void APlayer_Controller::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd( )"));		// 나중에 쓸 일이 많으므로 FORCEINLINE에 넣지 않았다.
	bAttacking = false;
}

void APlayer_Controller::ComboAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("ComboaAttackCheck is activate"));

	if (Combocount >= 3)	// 콤보 공격 값 넘지 않도록 
		Combocount = 0;

	if (bComboAttack)		// 콤보 공격이 실행이 된다면 -> MainAnim_BP에 Event_Graph 에서 조정된 값을 사용한다.
	{

		Combocount++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Attack is available Combocount ++"));
		bComboAttack = false;
		Attack(); // 콤보 공격 실행 
	}
}

