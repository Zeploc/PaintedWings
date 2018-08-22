// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpResetMechanicsComp.h"
#include "playerCheckpointMechanics.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Player/BirdPlayer.h"
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
	SpawnLoc = GetOwner()->GetActorLocation();
	TriggerSphere = GetOwner()->FindComponentByClass<USphereComponent>();
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &UJumpResetMechanicsComp::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &UJumpResetMechanicsComp::OnOverlapEnd);
	TimeUsed = 0.0f;
	CurrentTime = 0.0f;
}


// Called every frame
void UJumpResetMechanicsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentTime += DeltaTime;
	if (bGone)
	{
		if (CurrentTime - TimeUsed >= 3.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DashLocReset"));
			bGone = !bGone;
			GetOwner()->SetActorLocation(SpawnLoc);
		}
	}
	// ...
}

void UJumpResetMechanicsComp::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ABirdPlayer>(OtherActor)) //OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DashReset"));
		ABirdPlayer* bird = (ABirdPlayer*)OtherActor;
		bird->SetDashAvaliability(true);
		GetOwner()->SetActorLocation(FVector(1000,1000,1000));
		TimeUsed = CurrentTime;
		bGone = true;
	}
}

void UJumpResetMechanicsComp::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{

	}
}

