// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BirdController.generated.h"

/**
 * 
 */
UCLASS()
class PAINTEDWINGS_API ABirdController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABirdController();

public:
	void AddCollectablePoint(class ACollectable* Col);
	void ConfirmCollectables();
	void RemoveCurrentCollectables();

	UFUNCTION(BlueprintCallable)
		int GetCollectableCount();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	class ABirdPlayer* BirdPlayerRef;
	
	int CollectableCount;
	TArray<ACollectable*> CurrentCollectables;
};
