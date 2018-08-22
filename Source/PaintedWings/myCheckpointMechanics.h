// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "myCheckpointMechanics.generated.h"

class UBoxComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINTEDWINGS_API AmyCheckpointMechanics : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AmyCheckpointMechanics();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* checkpointTrigger;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
