// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DashWall.generated.h"

UCLASS()
class PAINTEDWINGS_API ADashWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADashWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ABirdController* PlayerControllerRef;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Web;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinimumSpeed = 800.0f;

	UFUNCTION()
		void OnCompOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
