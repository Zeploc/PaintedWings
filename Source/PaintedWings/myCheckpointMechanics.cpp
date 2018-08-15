// Fill out your copyright notice in the Description page of Project Settings.

#include "myCheckpointMechanics.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "myPlayerCharacter.h"
#include "playerCheckpointMechanics.h"

// Sets default values for this component's properties
UmyCheckpointMechanics::UmyCheckpointMechanics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UmyCheckpointMechanics::BeginPlay()
{
	Super::BeginPlay();
	checkpointTrigger = GetOwner()->FindComponentByClass<UBoxComponent>();;
	checkpointTrigger->OnComponentBeginOverlap.AddDynamic(this, &UmyCheckpointMechanics::OnOverlapBegin);
	checkpointTrigger->OnComponentEndOverlap.AddDynamic(this, &UmyCheckpointMechanics::OnOverlapEnd);
	// ...
	
}


// Called every frame
void UmyCheckpointMechanics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UmyCheckpointMechanics::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->FindComponentByClass<UplayerCheckpointMechanics>())
		{
			OtherActor->FindComponentByClass<UplayerCheckpointMechanics>()->m_pCurrentCheckpoint = this->GetOwner();
		}
	}
}

void UmyCheckpointMechanics::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

