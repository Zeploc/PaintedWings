// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpReset.generated.h"

UCLASS()
class PAINTEDWINGS_API AJumpReset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpReset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerCollider")
		class USphereComponent* TriggerSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* DashResetMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle RespawnHandler;

	void Respawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RespawnTime = 3.0f;
	
};
