// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpResetMechanicsComp.h"
#include "playerCheckpointMechanics.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UJumpResetMechanicsComp::UJumpResetMechanicsComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJumpResetMechanicsComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &UJumpResetMechanicsComp::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &UJumpResetMechanicsComp::OnOverlapEnd);
}


// Called every frame
void UJumpResetMechanicsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJumpResetMechanicsComp::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{

	}
}

void UJumpResetMechanicsComp::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{

	}
}

