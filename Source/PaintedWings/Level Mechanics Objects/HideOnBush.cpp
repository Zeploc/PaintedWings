// Fill out your copyright notice in the Description page of Project Settings.

#include "HideOnBush.h"
#include "Components/BoxComponent.h"
#include "PaintedWings/Player/BirdPlayer.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AHideOnBush::AHideOnBush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Box");
}

// Called when the game starts or when spawned
void AHideOnBush::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHideOnBush::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AHideOnBush::OnOverlapEnd);
}

// Called every frame
void AHideOnBush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHideOnBush::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef && !BirdRef->bRespawning)
	{

		BirdRef->bHiding = true;
	}
}

void AHideOnBush::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef && !BirdRef->bRespawning)
	{
		BirdRef->bHiding = false;

	}
}

