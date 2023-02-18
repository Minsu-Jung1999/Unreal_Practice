// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Controller.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACharacter_Controller::ACharacter_Controller()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->JumpZVelocity = 650.f; // 점프 높이 조절
	GetCharacterMovement()->AirControl = 0.2f; // 공중에서 이동량 조절



	// 이동
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// RotationRate = ENGINE_API FFindFloorResult 구조체에 있는 FRotator 값임
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);


	// 카메라 거치대 초기화 하기
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->bUsePawnControlRotation = true;
	// 거치대 길이
	SpringArmComp->TargetArmLength = 600.f;	

	// 카메라 초기화하기
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	bDead = false;
	Health = 100.f;

}

// Called when the game starts or when spawned
void ACharacter_Controller::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Controller::OnBeginOverlap);
	
	if (Player_Power_Widget_Class != nullptr)
	{
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
}

// Called every frame
void ACharacter_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Health -= DeltaTime * Health_Treshold;

	if (Health <= 0)
	{
		if (!bDead)
		{
			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &ACharacter_Controller::RestartGame, 3.0f, false);
		}
	}

}

// Called to bind functionality to input
void ACharacter_Controller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Player Input Key Binding */
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacter_Controller::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacter_Controller::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}



void ACharacter_Controller::MoveForward(float Axis)
{
	if (!bDead)
	{
		if (Controller != nullptr && Axis != 0.0f)
		{
			// Find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// Get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Axis);
		}

	}
	
}

void ACharacter_Controller::MoveRight(float Axis)
{
	if (!bDead)
	{
		if (Controller != nullptr && Axis != 0.0f)
		{
			// Find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// Get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Axis);
		}

	}
}
void ACharacter_Controller::OnBeginOverlap(UPrimitiveComponent* HitComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		Health += Healt_Adding;

		if (Health > 100.0f)
			Health = 100.0f;

		OtherActor->Destroy();
	}
}
void ACharacter_Controller::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
