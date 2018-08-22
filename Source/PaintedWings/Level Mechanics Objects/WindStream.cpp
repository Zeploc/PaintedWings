// Fill out your copyright notice in the Description page of Project Settings.

#include "WindStream.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "playerCheckpointMechanics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Player/BirdPlayer.h"

// Sets default values
AWindStream::AWindStream()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	RootComponent->bEditableWhenInherited = true;
	RootComponent->RelativeLocation = { 0.0f,0.0f,0.0f };*/
	//windTrigger = CreateDefaultSubobject<UBoxComponent>("Box");

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComp");

	windTrigger = CreateDefaultSubobject<UBoxComponent>("Interact Box");
	windTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWindStream::OnOverlapBegin);
	windTrigger->OnComponentEndOverlap.AddDynamic(this, &AWindStream::OnOverlapEnd);
	windTrigger->SetupAttachment(RootComponent);

	WindDirection = CreateDefaultSubobject<UArrowComponent>("Wind Direction Arrow");
	WindDirection->SetupAttachment(RootComponent);
	WindDirection->RelativeRotation.Pitch = 90.0f;
}

// Called when the game starts or when spawned
void AWindStream::BeginPlay()
{
	Super::BeginPlay();
	Direction = WindDirection->GetForwardVector();
}

// Called every frame
void AWindStream::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInside)
	{
		UE_LOG(LogTemp, Warning, TEXT("WindStream"));
		if (bStream) { LaunchSize = StreamSize; };
		player->LaunchCharacter(Direction * LaunchSize, false, false);
	}

}

void AWindStream::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ABirdPlayer>(OtherActor))// OtherComp->GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
	{
		/*Rotation = GetOwner()->GetActorRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);*/
		ACharacter* player = (ACharacter*)OtherActor;
		player->LaunchCharacter(Direction * LaunchSize, true, true);

		/*player = (ACharacter*)OtherActor;
		if (bStream)
		{
			bInside = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WINDBOOST"));
			player->LaunchCharacter(Direction * LaunchSize, true, true);
		}*/
	}
}

void AWindStream::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bInside = false;
}

