// Fill out your copyright notice in the Description page of Project Settings.

#include "WindStream.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "playerCheckpointMechanics.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AWindStream::AWindStream()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent->bEditableWhenInherited = true;
	RootComponent->RelativeLocation = { 0.0f,0.0f,0.0f };
	windTrigger = CreateDefaultSubobject<UBoxComponent>("Box");
	windTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWindStream::OnOverlapBegin);
	windTrigger->OnComponentEndOverlap.AddDynamic(this, &AWindStream::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AWindStream::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWindStream::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWindStream::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{
		Rotation = GetOwner()->GetActorRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		ACharacter* player = (ACharacter*)OtherActor;
		player->AddMovementInput(Direction, 50.0f);
	}
}

void AWindStream::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

