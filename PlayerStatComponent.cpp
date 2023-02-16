// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "Net/UnrealNetWork.h"
#include "TimerManager.h"


// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	Hunger = 100.f;
	Thirst = 100.f;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);



}

void UPlayerStatComponent::HandleHungerAndThirst()
{

}

