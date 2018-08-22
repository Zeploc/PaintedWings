// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpReset.h"

#include "Player/BirdPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
AJumpReset::AJumpReset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = TriggerSphere;
	DashResetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dash Reset Mesh"));
	DashResetMesh->SetCollisionProfileName("OverlapAll");

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AJumpReset::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AJumpReset::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJumpReset::Respawn()
{
	GetWorldTimerManager().ClearTimer(RespawnHandler);
	DashResetMesh->SetVisibility(true);
}

// Called every frame
void AJumpReset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpReset::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* bird = Cast<ABirdPlayer>(OtherActor);
	if (bird && DashResetMesh->IsVisible())
	{
		UE_LOG(LogTemp, Warning, TEXT("DashReset"));
		bird->SetDashAvaliability(true);
		DashResetMesh->SetVisibility(false);
		GetWorldTimerManager().SetTimer(RespawnHandler, this, &AJumpReset::Respawn, RespawnTime, false);
	}
}