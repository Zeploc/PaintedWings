// Fill out your copyright notice in the Description page of Project Settings.

#include "playerCheckpointMechanics.h"
#include "UObject/ConstructorHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UplayerCheckpointMechanics::UplayerCheckpointMechanics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UplayerCheckpointMechanics::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UplayerCheckpointMechanics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UplayerCheckpointMechanics::MoveToCurrentCheckpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Activating"));
	if (m_pCurrentCheckpoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint Activated"));
		GetOwner()->SetActorLocation(m_pCurrentCheckpoint->GetActorLocation());
	}
}