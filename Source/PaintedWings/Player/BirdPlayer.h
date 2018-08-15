// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BirdPlayer.generated.h"

UCLASS()
class PAINTEDWINGS_API ABirdPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABirdPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* BoomArm;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	float NormalGravity;

	float NormalAirControl;
	
	FTimerHandle JumpHoldTimerHandle;
	void StartGlide();
	bool JumpHeld = false;

	void StartJump();
	void StopJump();

	void SwitchGlide(bool IsGliding);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly)
		bool bIsGliding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GlidingGravity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GlidingAirControl = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpTimeToGlide = 0.2f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxGlideRollRotate = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxGlidePitchRotate = 20.0f;

};
