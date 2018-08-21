// Fill out your copyright notice in the Description page of Project Settings.

#include "BirdController.h"

#include "BirdPlayer.h"
#include "Level Mechanics Objects/Collectable.h"


// Sets default values
ABirdController::ABirdController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollectableCount = 0;
}

// Called when the game starts or when spawned
void ABirdController::BeginPlay()
{
	Super::BeginPlay();

	BirdPlayerRef = Cast<ABirdPlayer>(GetCharacter());
}

void ABirdController::ConfirmCollectables()
{
	if (CurrentCollectables.Num() <= 0) return;
	for (int i = 0; i < CurrentCollectables.Num(); i++)
	{
		CurrentCollectables[i]->Destroy();
		CollectableCount++;
	}
	CurrentCollectables.Empty();
}

void ABirdController::RemoveCurrentCollectables()
{
	for (int i = 0; i < CurrentCollectables.Num(); i++)
	{
		CurrentCollectables[i]->RespawnCollectable();
	}
	CurrentCollectables.Empty();
}

void ABirdController::AddCollectablePoint(class ACollectable* Col)
{
	if (!CurrentCollectables.Contains(Col))
	{
		CurrentCollectables.Add(Col);
	}
}

int ABirdController::GetCollectableCount()
{
	return CollectableCount;
}