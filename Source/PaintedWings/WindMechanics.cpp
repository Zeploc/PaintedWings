// Fill out your copyright notice in the Description page of Project Settings.

#include "WindMechanics.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "playerCheckpointMechanics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UWindMechanics::UWindMechanics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...RootComponent = CreateDefaultSubobject<USceneComponent>("RootComp");

}


// Called when the game starts
void UWindMechanics::BeginPlay()
{
	Super::BeginPlay();

	windTrigger->OnComponentBeginOverlap.AddDynamic(this, &UWindMechanics::OnOverlapBegin);
	windTrigger->OnComponentEndOverlap.AddDynamic(this, &UWindMechanics::OnOverlapEnd);
	// ...
	
}


// Called every frame
void UWindMechanics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bInStream)
	{
		
	}
	// ...
}


void UWindMechanics::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyingForce"));
		bInStream = true;
		Rotation = GetOwner()->GetActorRotation();
		GetOwner()->GetActorForwardVector();
		FVector forward = FVector(0.0f, 1.0f, 0.0f);
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		Direction = GetOwner()->GetActorForwardVector();
		Direction.Normalize();
		Direction.X = Direction.X * LaunchSize;
		Direction.Y = Direction.Y * LaunchSize;
		Direction.Z = Direction.Z * LaunchSize;
		player = (ACharacter*)OtherActor;
		player->LaunchCharacter(Direction, false, true);
	}
}

void UWindMechanics::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{
		bInStream = false;
	}
}

