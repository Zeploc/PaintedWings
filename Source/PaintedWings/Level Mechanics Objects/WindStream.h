// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindStream.generated.h"
class UBoxComponent;
UCLASS()
class PAINTEDWINGS_API AWindStream : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindStream();
	FRotator Rotation;
	FRotator YawRotation;
	FVector Direction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* windTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UArrowComponent* WindDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch")
		float LaunchSize = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stream")
		float StreamSize = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStream = false;
	bool bInside = false;
	ACharacter* player;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* WindstreamAC;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* WindstreamWhoosh;
};
