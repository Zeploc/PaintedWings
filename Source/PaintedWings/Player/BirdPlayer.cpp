// Fill out your copyright notice in the Description page of Project Settings.

#include "BirdPlayer.h"

#include "playerCheckpointMechanics.h"
#include "Player/BirdController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystemComponent.h"


#include "playerCheckpointMechanics.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "NectarMechanics.h"
// Sets default values

ABirdPlayer::ABirdPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(40.f, 60.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	BoomArm->SetupAttachment(RootComponent);
	BoomArm->TargetArmLength = 560.0f; // The camera follows at this distance behind the character	
	BoomArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	BoomArm->SocketOffset = FVector(0.0f, 0.0f, 240.0f);

	// Create a follow camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ThirdPersonCamera->SetupAttachment(BoomArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	ThirdPersonCamera->SetRelativeRotation(FRotator(-25.0f, 0.0f, 0.0f));

	NormalGravity = GetCharacterMovement()->GravityScale;
	NormalRotationRate = GetCharacterMovement()->RotationRate.Yaw;

	DeathParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Particle System"));
	DeathParticleSystem->SetupAttachment(RootComponent);
	DeathParticleSystem->bAutoActivate = false;
}

// Called to bind functionality to input
void ABirdPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABirdPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABirdPlayer::StopJump);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABirdPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABirdPlayer::MoveRight);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABirdPlayer::Dash);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ABirdPlayer::MouseTurn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABirdPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ABirdPlayer::MouseLookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABirdPlayer::LookUpAtRate);
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &ABirdPlayer::MoveToCheckpoint);
	PlayerInputComponent->BindAction("NectarSuck", IE_Pressed, this, &ABirdPlayer::NectarGathering);
}

// Called when the game starts or when spawned
void ABirdPlayer::BeginPlay()
{
	Super::BeginPlay();
	bClimbingVines = false;
	NormalGravity = GetCharacterMovement()->GravityScale;
	NormalAirControl = GetCharacterMovement()->AirControl;

	playerCapsuleTrigger = FindComponentByClass<UCapsuleComponent>();
	playerCapsuleTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABirdPlayer::OnOverlapBegin);
	playerCapsuleTrigger->OnComponentEndOverlap.AddDynamic(this, &ABirdPlayer::OnOverlapEnd);
	FirstJumpSize = GetCharacterMovement()->JumpZVelocity;

	//// CAN't USE "GETCONTROLLER()" When restart playing was used (It must create it then posses, so at time of begin play, not have control)
	//BirdControllerRef = Cast<ABirdController>(GetWorld()->GetFirstPlayerController());
	//if (!BirdControllerRef)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Uh oh NO CONTROLLER"));
	//}
	//else
	//	DefaultCameraPitch = BirdControllerRef->GetControlRotation().Pitch;
}

// Called every frame
void ABirdPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (fInvincibility > 0)
	{
		fInvincibility -= DeltaTime;
	}
	if (iHealth <= 0)
	{
		Death();
	}

	HungerLevel -= HungerLossRate * DeltaTime;
	if ((HungerLevel <= 0.0f || GetActorLocation().Z < DeathHeight) && !bRespawning)
	{
		Death();
	}

	if (bClimbingVines == true)
	{
		GetCharacterMovement()->GravityScale = 0;
		UE_LOG(LogTemp, Warning, TEXT("NOGRAV: %f"), GetCharacterMovement()->GravityScale);
	}
	else if (bIsGliding)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Pitch = 0.0;
		SetActorRotation(NewRotation);
		if (GetVelocity().Z > 0)
		{
			GetCharacterMovement()->GravityScale = NormalGravity;
		}
		else
			GetCharacterMovement()->GravityScale = GlidingGravity;

		GetCharacterMovement()->RotationRate.Yaw = GlideRotationRate;
		
		GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "GLIDING");
	}
	else
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Pitch = 0.0;
		SetActorRotation(NewRotation);
		if (IsDashing)
		{
			GetCharacterMovement()->GravityScale = 0.0f;
		}
		else
		{
			GetCharacterMovement()->GravityScale = NormalGravity;
			GetCharacterMovement()->RotationRate.Yaw = NormalRotationRate;
		}
	}

	if (GetCharacterMovement()->IsFalling())
	{
		DoubleJump = false;
	}
	else
	{
		if (bHasGlided && bCanGlide)
		{
			//bCanGlide = false;
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Disable Can Glide");
		}
		HasDoubleJumped = false;
		bCanDash = true;
		if (bIsGliding) SwitchGlide(false);
	}
	InputDelayer();
	CameraMovement();
	HasMovedCamera = false;
}

void ABirdPlayer::TurnAtRate(float Rate)
{
	if (Rate != 0) HasMovedCamera = true;
	if (!bInputEnabled) return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	// calculate delta for this frame from the rate information
}

void ABirdPlayer::LookUpAtRate(float Rate)
{
	if (Rate != 0) HasMovedCamera = true;
	if (!bInputEnabled) return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABirdPlayer::MouseTurn(float Value)
{
	if (Value != 0) HasMovedCamera = true;
	AddControllerYawInput(Value);
}

void ABirdPlayer::MouseLookUp(float Value)
{
	if (Value != 0) HasMovedCamera = true;
	AddControllerPitchInput(Value);
}

void ABirdPlayer::CameraMovement()
{
	if (bRespawning) return;
	FVector NoHeightVelocity = GetVelocity();
	NoHeightVelocity.Z = 0.0f;
	if (!HasMovedCamera)//|| NoHeightVelocity.Size() <= 0.0f)
	{
		if (!CameraHandle.IsValid() && !LerpCamera)
			GetWorldTimerManager().SetTimer(CameraHandle, this, &ABirdPlayer::CameraLerpCheck, CameraLerpTimeout, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Red, "Camera Input");
		GetWorldTimerManager().ClearTimer(CameraHandle);
		LerpCamera = false;
		return;
	}

	if (LerpCamera)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "WOULD LERP CAMERA");
		FRotator CurrentControlRotation = GetController()->GetControlRotation();
		GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Purple, GetCapsuleComponent()->GetComponentRotation().ToString());
		FRotator FacingRotation = CurrentControlRotation;
		FacingRotation.Yaw = GetMesh()->GetComponentRotation().Yaw;
		FacingRotation.Pitch = DefaultCameraPitch;			

		float Angle = abs(CurrentControlRotation.Yaw - FacingRotation.Yaw);
		if (NoHeightVelocity.Size() <= 0.0f)
		{
			if (IdleCameraLerp)
			{
				CurrentControlRotation = FMath::Lerp(CurrentControlRotation, FacingRotation, CameraLerpSpeed * GetWorld()->DeltaTimeSeconds);
			}
			else
			{
				if (!CameraPlayerStillHandle.IsValid()) GetWorldTimerManager().SetTimer(CameraPlayerStillHandle, this, &ABirdPlayer::CameraStillLerpCheck, CameraLerpTimeout, false);				
			}
		}
		else if (NoHeightVelocity.Size() >= MinimumStartLerpMoveSpeed && ((Angle <= 170.0f && !bIsGliding) || (GetCharacterMovement()->IsFalling() && Angle <= 100.0f)))
		{
			IdleCameraLerp = false;
			float LerpSpeed = CameraLerpSpeed;
			float Ratio = (NoHeightVelocity.Size() - MinimumStartLerpMoveSpeed) / (GetCharacterMovement()->MaxWalkSpeed - MinimumStartLerpMoveSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "Current Ratio " + FString::SanitizeFloat(Ratio));
			Ratio = FMath::Clamp(Ratio, 0.0f, 1.0f);
			LerpSpeed *= Ratio;
			CurrentControlRotation = FMath::Lerp(CurrentControlRotation, FacingRotation, LerpSpeed * GetWorld()->DeltaTimeSeconds);
		}
		GetController()->SetControlRotation(CurrentControlRotation);
	}
}

void ABirdPlayer::CameraLerpCheck()
{
	GetWorldTimerManager().ClearTimer(CameraHandle);
	if (!HasMovedCamera) LerpCamera = true;
}

void ABirdPlayer::CameraStillLerpCheck()
{
	GetWorldTimerManager().ClearTimer(CameraPlayerStillHandle);
	FVector NoHeightVelocity = GetVelocity();
	NoHeightVelocity.Z = 0.0f;
	if (NoHeightVelocity.Size() <= 1.0f)
	{
		IdleCameraLerp = true;
	}
}


void ABirdPlayer::MoveForward(float Value)
{
	// find out which way is forward


	const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();//GetViewTarget()->GetActorRotation();// Controller->GetControlRotation();
	if (bIsGliding && Value >= 0)
	{
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, 1);
	}

	if (!bInputEnabled) return;	
	if (bClimbingVines && (Value != 0.0f))
	{
		GetCharacterMovement()->GravityScale = 0;
		const FVector Direction = FVector(0, 0, 1);
		
		AddMovementInput(Direction, Value);
		UE_LOG(LogTemp, Warning, TEXT("UP UP UP"));
	}
	else if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//const FVector Direction = FVector(0, 0, 1);
		UE_LOG(LogTemp, Warning, TEXT("Forward: %s"), *Direction.ToString());
		AddMovementInput(Direction, Value);

		//LerpCamera = false;
	}
}

void ABirdPlayer::MoveRight(float Value)
{
	if (!bInputEnabled) return;
	if (bClimbingVines && (Value != 0.0f))
	{
		GetCharacterMovement()->GravityScale = 0;
		const FVector Direction = DirectionToVine.RotateAngleAxis(90, FVector(0, 0, 1));

		AddMovementInput(Direction, Value);
		UE_LOG(LogTemp, Warning, TEXT("SIDEO"));
	}
	else if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();// GetViewTarget()->GetActorRotation();// Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		UE_LOG(LogTemp, Warning, TEXT("Right: %s"), *Direction.ToString());
		// add movement in that direction
		AddMovementInput(Direction, Value);

		//LerpCamera = false;
	}
}

void ABirdPlayer::StartJump()
{
	if (!bInputEnabled) return;
	if (bClimbingVines)
	{
		//AddMovementInput(-DirectionToVine, 5.0f);
		//GetCharacterMovement()->AddForce(-DirectionToVine * 100.0f);
		bClimbingVines = false;
		//BirdRef -> HasDoubleJumped = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCharacterMovement()->bOrientRotationToMovement = true;

	}

	SwitchGlide(false);
	GetWorldTimerManager().ClearTimer(JumpHoldTimerHandle);
	GetWorldTimerManager().SetTimer(JumpHoldTimerHandle, this, &ABirdPlayer::StartGlide, JumpTimeToGlide, false);
	JumpHeld = true;

	
	if (GetCharacterMovement()->IsFalling())
	{
		if (HasDoubleJumped) return;
		DoubleJump = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Flap1, this->GetActorLocation(), 1.0f, 1.0f, 0.0f);
		GetCharacterMovement()->JumpZVelocity = DoubleJumpSize;
		//UE_LOG(LogTemp, Warning, TEXT("Jump Velocity %s"), GetCharacterMovement()->JumpZVelocity);
		GetWorldTimerManager().ClearTimer(DoubleJumpTimerHandle);
		if (DoubleJumpDelay > 0) GetWorldTimerManager().SetTimer(DoubleJumpTimerHandle, this, &ABirdPlayer::ApplyDoubleJump, DoubleJumpDelay, false);
		else ApplyDoubleJump();
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Flap1, this->GetActorLocation(), 1.0f, 1.0f, 0.0f);
		GetCharacterMovement()->JumpZVelocity = FirstJumpSize;
		//UE_LOG(LogTemp, Warning, TEXT("Jump Velocity %f"), GetCharacterMovement()->JumpZVelocity);
		Jump();
	}
}

void ABirdPlayer::StopJump()
{
	if (!bInputEnabled) return;

	StopJumping();
	JumpHeld = false;
	SwitchGlide(false);
}

void ABirdPlayer::ApplyDoubleJump()
{
	GetWorldTimerManager().ClearTimer(DoubleJumpTimerHandle);
	Jump();
	this->bSimGravityDisabled = false;
	//JumpHeld = true;
	HasDoubleJumped = true;
}

void ABirdPlayer::Death()
{
	if (bRespawning) return;
	// Play Partical Effect
	// Disable Control
	// Disable Movement
	// Invisible
	if (!BirdControllerRef)
		BirdControllerRef = Cast<ABirdController>(GetController());
	BirdControllerRef->RemoveCurrentCollectables();
	DeathParticleSystem->Activate();
	bRespawning = true;
	//GetMesh()->SetVisibility(false);
	GetCharacterMovement()->DisableMovement();
	bInputEnabled = false;
	GetMesh()->SetCollisionProfileName("PhysicsActor");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
	//BirdControllerRef->SetViewTargetWithBlend(this, 0.1f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
	//BirdControllerRef->bAutoManageActiveCameraTarget = false;
	//GetCapsuleComponent()->SetSimulatePhysics(true);
	//GetController()->UnPossess();
	GetWorldTimerManager().SetTimer(RespawnHandle, this, &ABirdPlayer::MoveToCheckpoint, RespawnDelay, false);
}

void ABirdPlayer::StartGlide()
{
	if (!bInputEnabled || !bCanGlide) return;
	GetWorldTimerManager().ClearTimer(JumpHoldTimerHandle);
	if (JumpHeld)
	{     
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Start Glide");
		SwitchGlide(true);
	}
}

void ABirdPlayer::Dash()
{
	if (!bCanDash) return;
	bCanDash = false;
	GetCharacterMovement()->GravityScale = 0.0f;
	FVector DashDirectionForce = GetMesh()->GetForwardVector() * DashForce;
	DashDirectionForce.Z = 0.0f;
	LaunchCharacter(DashDirectionForce, true, true);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ABirdPlayer::FinishDash, DashTimer, false);
	IsDashing = true;
	bIsGliding = false;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Woosh1, this->GetActorLocation(), 1.0f, 1.0f, 0.0f);

	FRotator Rotation = Controller->GetControlRotation();
	Rotation.Yaw = DashDirectionForce.Rotation().Yaw;
	SetActorRotation(Rotation);// Controller->SetControlRotation(Rotation);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ABirdPlayer::FinishDash()
{
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetCharacterMovement()->GravityScale = NormalGravity;
	IsDashing = false;

	FRotator Rotation = GetMesh()->RelativeRotation;
	Rotation.Roll = 0.0f;
	Rotation.Pitch = 0.0f;
	GetMesh()->RelativeRotation = Rotation;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	if (JumpHeld) SwitchGlide(true);
}

void ABirdPlayer::SwitchGlide(bool IsGliding)
{
	bIsGliding = IsGliding;
	if (bIsGliding)
	{
		GetCharacterMovement()->GravityScale = GlidingGravity;
		GetCharacterMovement()->RotationRate.Yaw = GlideRotationRate;
		GetCharacterMovement()->AirControl = GlidingAirControl;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		//if (GetVelocity().Z < -GlideCapFallSpeed)
		//{
			//GetCharacterMovement()->velocity
			//GetCapsuleComponent()->ComponentVelocity
			//FVector CapsuleLinearSpeed = GetCharacterMovement()->velocity->Velocity;
			//GetCapsuleComponent()->SetPhysicsLinearVelocity()
		//}
		//bHasGlided = true;
	}
	else
	{
		GetCharacterMovement()->GravityScale = NormalGravity;
		GetCharacterMovement()->RotationRate.Yaw = NormalRotationRate;
		GetCharacterMovement()->AirControl = NormalAirControl;
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
	}
}



void ABirdPlayer::MoveToCheckpoint()
{
	GetWorldTimerManager().ClearTimer(RespawnHandle);
	if (BirdControllerRef) BirdControllerRef->Respawn();
	else UE_LOG(LogTemp, Warning, TEXT("CONTROLLER REFERENCE NOT SET"));

	//UE_LOG(LogTemp, Warning, TEXT("Checkpoint Activating"));
	//if (this)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OwnerFound"));
	//	if (this->FindComponentByClass<UplayerCheckpointMechanics>())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Mechanics Script Found"));
	//		/*if (BirdControllerRef)
	//		{
	//			BirdControllerRef->RemoveCurrentCollectables();
	//			BirdControllerRef->Possess(this);
	//		}*/

	//		//ReplenishRebase();
	//		//this->FindComponentByClass<UplayerCheckpointMechanics>()->MoveToCurrentCheckpoint();
	//		//GetMesh()->SetSimulatePhysics(false);
	//		//GetMesh()->SetCollisionProfileName("CharacterMesh");
	//		//UE_LOG(LogTemp, Warning, TEXT("Location Before %s"), *GetMesh()->GetComponentLocation().ToString());
	//		//GetMesh()->SetWorldLocationAndRotation(GetCapsuleComponent()->GetComponentLocation(), GetCapsuleComponent()->GetComponentRotation());//->GetComponentLocation()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f), false, nullptr, ETeleportType::ResetPhysics);
	//		//UE_LOG(LogTemp, Warning, TEXT("Location After %s"), *GetMesh()->GetComponentLocation().ToString());
	//		//GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//		//bInputEnabled = true;
	//		//GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	//		////GetMesh()->SetVisibility(true);
	//		//bRespawning = false;
	//	}
	//}
}

void ABirdPlayer::NectarGathering()
{
	if (bTouchingNectar)
	{
		iInputDelay = 60;
		bInputEnabled = false;
		bCanGlide = true;
		bHasGlided = false;
		HungerLevel = 1.0f;
		GettingNectar = true;
	}
}

void ABirdPlayer::InputDelayer()
{
	if (iInputDelay >= 0)
	{
		iInputDelay--;
		HungerLevel = 1.0f;
	}
	else
	{
		bInputEnabled = true;
		GettingNectar = false;
	}
}

void ABirdPlayer::ReplenishRebase()
{
	HungerLevel = 1.0f;
	if (BirdControllerRef) BirdControllerRef->ConfirmCollectables();
	else
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "CONTROLLER NOT FOUND");
}


void ABirdPlayer::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->FindComponentByClass<UNectarMechanics>())
	{
		bTouchingNectar = true;
	}
}

void ABirdPlayer::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->FindComponentByClass<UNectarMechanics>())
	{
		bTouchingNectar = false;
	}
}

void ABirdPlayer::SetDashAvaliability(bool _b)
{
	bCanDash = _b;
}
