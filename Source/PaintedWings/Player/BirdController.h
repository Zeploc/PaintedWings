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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundBGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundAmbience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundAmbience2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* CicadaAmbience;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* BackgroundMusicAC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* UIAC;

public:
	void Respawn();
	void AddNewCheckpoint(AActor* NewCheckpoint);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look at Next")
		TMap<int, AActor*> LookAtNextActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look at Next")
		int CurrentLookAt = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	class ABirdPlayer* BirdPlayerRef;
	

	UPROPERTY(EditAnywhere)
		AActor* CurrentCheckpoint;

};
