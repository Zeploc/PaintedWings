// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BirdPlayer.generated.h"
class UCapsuleComponent;

class UStaticMeshComponent;
class UMaterial;
class UMaterialInstanceDynamic;
UCLASS()
class PAINTEDWINGS_API ABirdPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABirdPlayer();
	int iHealth = 3;
	float fInvincibility = 0;
	bool bRespawning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* BoomArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* DeathParticleSystem;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		bool bClimbingVines;
	void Death();

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

	void MouseTurn(float Value);

	void MouseLookUp(float Value);


	float NormalGravity;
	float NormalRotationRate;
	float NormalAirControl;
	
	FTimerHandle JumpHoldTimerHandle;
	FTimerHandle DoubleJumpTimerHandle;
	FTimerHandle DashTimerHandle;
	FTimerHandle RespawnHandle;
	FTimerHandle CameraHandle;
	FTimerHandle CameraPlayerStillHandle;

	void StartGlide();
	bool JumpHeld = false;
	bool bCanGlide = true;
	bool bHasGlided = false;
	bool bTouchingNectar = false;

	bool bInputEnabled = true;
	int iInputDelay = 0;
	void StartJump();
	void StopJump();
	void ApplyDoubleJump();
	float FirstJumpSize;

	void Dash();
	void FinishDash();
	bool bCanDash = true;


	void SwitchGlide(bool IsGliding);

	void CameraMovement();
	void CameraLerpCheck();
	void CameraStillLerpCheck();
	bool HasMovedCamera = true;
	bool LerpCamera = false;
	bool IdleCameraLerp = false;
	
	
	class ABirdController* BirdControllerRef;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundAmbience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundAmbience2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* CicadaAmbience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Flap1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Flap2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Flap3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Flap4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Woosh1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* Woosh2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveToCheckpoint();

	UPROPERTY(BlueprintReadOnly, Category = "Gliding")
		bool bIsGliding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float GlidingGravity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float GlidingAirControl = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float JumpTimeToGlide = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float GlideCapFallSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float GlideRotationRate = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float DoubleJumpDelay = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		bool DoubleJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float DoubleJumpSize = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashing")
		float DashForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashing")
		float DashTimer = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashing")
		bool IsDashing = false;

	bool HasDoubleJumped = false;

	void NectarGathering();
	void InputDelayer();
	void ReplenishRebase();
	UCapsuleComponent* playerCapsuleTrigger;
	//overlap events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float MaxGlideRollRotate = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
		float MaxGlidePitchRotate = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		float DeathHeight = -150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		float RespawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
		float HungerLevel = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
		float HungerLossRate = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hunger")
		bool GettingNectar = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraLerpTimeout = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraLerpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float MinimumStartLerpMoveSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float DefaultCameraPitch = -25.0f;

	
	void SetDashAvaliability(bool _b);
	bool IsRespawning() {
		return bRespawning;			
	}

	FVector DirectionToVine;
	FRotator VineRotation;
	class AClimbableVines* ClimbRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* platformMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* MyMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* DynMaterial;
};
