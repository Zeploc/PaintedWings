// Fill out your copyright notice in the Description page of Project Settings.

#include "myCheckpointMechanics.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "playerCheckpointMechanics.h"
#include "Engine.h"

// Sets default values for this component's properties
AmyCheckpointMechanics::AmyCheckpointMechanics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	// ...

	checkpointTrigger = CreateDefaultSubobject<UBoxComponent>("Box");
	checkpointTrigger->OnComponentBeginOverlap.AddDynamic(this, &AmyCheckpointMechanics::OnOverlapBegin);
	checkpointTrigger->OnComponentEndOverlap.AddDynamic(this, &AmyCheckpointMechanics::OnOverlapEnd);

}


// Called when the game starts
void AmyCheckpointMechanics::BeginPlay()
{
	Super::BeginPlay();
	// ...

	//checkpointTrigger = GetOwner()->FindComponentByClass<UBoxComponent>();
	//checkpointTrigger->bGenerateOverlapEvents = true;
}


// Called every frame
void AmyCheckpointMechanics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (checkpointTrigger)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TriggerCollider;"));
	}
	// ...
}

void AmyCheckpointMechanics::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Occuring"));
		if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap Occuring"));
			OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>()->m_pCurrentCheckpoint = this;
		}
	}
}

void AmyCheckpointMechanics::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Occuring"));
		if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap Occuring"));
			OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>()->m_pCurrentCheckpoint = this;
		}
	}
}

