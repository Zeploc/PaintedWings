// Fill out your copyright notice in the Description page of Project Settings.

#include "BirdPlayer.h"

#include "playerCheckpointMechanics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineUtils.h"

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
	
}

// Called when the game starts or when spawned
void ABirdPlayer::BeginPlay()
{
	Super::BeginPlay();
	NormalGravity = GetCharacterMovement()->GravityScale;
	NormalAirControl = GetCharacterMovement()->AirControl;	

	playerCapsuleTrigger = FindComponentByClass<UCapsuleComponent>();
	playerCapsuleTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABirdPlayer::OnOverlapBegin);
	playerCapsuleTrigger->OnComponentEndOverlap.AddDynamic(this, &ABirdPlayer::OnOverlapEnd);
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
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABirdPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABirdPlayer::LookUpAtRate);
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &ABirdPlayer::MoveToCheckpoint);
	PlayerInputComponent->BindAction("NectarSuck", IE_Pressed, this, &ABirdPlayer::NectarGathering);
}

// Called every frame
void ABirdPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGliding)
	{
		if (GetVelocity().Z > 0)
		{
			GetCharacterMovement()->GravityScale = NormalGravity;
		}
		else
			GetCharacterMovement()->GravityScale = GlidingGravity;
		GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, "GLIDING");
	}
	else
	{
		GetCharacterMovement()->GravityScale = NormalGravity;
	}

	if(this->GetActorLocation().Z < -100.0f)
	{
		UplayerCheckpointMechanics* CheckpointMech = this->FindComponentByClass<UplayerCheckpointMechanics>();
		if (CheckpointMech) CheckpointMech->MoveToCurrentCheckpoint();
	
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
	}
	InputDelayer();
}


void ABirdPlayer::TurnAtRate(float Rate)
{
	if (!bInputEnabled) return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	// calculate delta for this frame from the rate information
}


void ABirdPlayer::LookUpAtRate(float Rate)
{
	if (!bInputEnabled) return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABirdPlayer::MoveForward(float Value)
{
	if (!bInputEnabled) return;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		if (bIsGliding)
		{			
			/*FVector CameraForward = ThirdPersonCamera->GetForwardVector();
			CameraForward.Y = 0.0f;
			UE_LOG(LogTemp, Warning, TEXT("Before %s"), *Rotation.ToString());
			Rotation.Pitch -= Value;
			Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -MaxGlidePitchRotate, MaxGlidePitchRotate);
			UE_LOG(LogTemp, Warning, TEXT("After %s"), *Rotation.ToString());

			GetMesh()->SetRelativeRotation(Rotation);*/


			/*FVector CameraForward = ThirdPersonCamera->GetRightVector();
			CameraForward.Y = 0.0f;

			FVector DirectionVector = CameraForward.RotateAngleAxis(Value, FVector(0, 1, 0));
			FRotator DirectionRotation = DirectionVector.Rotation();

			FRotator Rotation = GetMesh()->GetComponentRotation();

			Rotation += DirectionRotation;
			Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -MaxGlidePitchRotate, MaxGlidePitchRotate);*/

			//FRotator CameraRotation = CameraForward.Rotation();
			//CameraRotation.Pitch = 0.0f;
			//Rotation.RotateVector(CameraForward);

			//FRotator CameraRotation = CameraForward.Rotation();
			//CameraRotation.Pitch = 0.0f;

			//UE_LOG(LogTemp, Warning, TEXT("Camera Rot %s"), *CameraRotation.ToString());

			////FRotator Rotation = GetMesh()->RelativeRotation;
			//CameraRotation.Pitch -= Value * MaxGlideRollRotate;
			//CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch, -MaxGlidePitchRotate, MaxGlidePitchRotate);
			//CameraRotation.Yaw = GetMesh()->GetComponentRotation().Yaw;


			FRotator RotationApply;
			RotationApply.Pitch = MaxGlidePitchRotate * Value;

			FVector CameraForward = ThirdPersonCamera->GetForwardVector();
			//CameraForward.Y = 0.0f;
			CameraForward.Y = Value * MaxGlidePitchRotate;

			/*FVector CamRot = RotationApply.RotateVector(CameraForward);
			FRotator DirectionRotation = CamRot.Rotation();*/

			FQuat NewCamQuat = CameraForward.ToOrientationQuat();
			FRotator NewCameraRotation =  ThirdPersonCamera->RelativeRotation;// CameraForward.ToOrientationRotator();// .Rotation();
			NewCameraRotation.Pitch = Value * MaxGlidePitchRotate;

			/*CameraForward.Y = 0.0f;
			
			FRotator CamDir = ThirdPersonCamera->GetForwardVector().Rotation();
			CamDir.Pitch = 0.0f;
			CamDir.Roll = 0.0f;


			
			FVector RotatedVector = RotationApply.RotateVector(ThirdPersonCamera->GetForwardVector());
			FRotator Rotated = RotatedVector.Rotation(); 
			Rotated.Yaw = GetCapsuleComponent()->RelativeRotation.Yaw;
			

			CameraForward.RotateAngleAxis(Value * MaxGlidePitchRotate, FVector(1, 0, 0));

			FRotator DirectionRotation = CameraForward.Rotation();
			FVector VCamDir = CamDir.Vector();
			VCamDir = DirectionRotation.RotateVector(VCamDir);
			FRotator NewRotation = Rotation;
			NewRotation.Pitch = VCamDir.X * MaxGlidePitchRotate * Value;
			NewRotation.Roll = VCamDir.Y * MaxGlidePitchRotate * Value;*/
			//DirectionRotation.Pitch = 0.0f;
			//DirectionRotation -= Rotation;
			//DirectionRotation.Pitch += Value * MaxGlidePitchRotate;

			//DirectionRotation.RotateVector(GetMesh()->GetRightVector());


			UE_LOG(LogTemp, Warning, TEXT("Camera Rot %s"), *NewCamQuat.ToString());

			//GetMesh()->SetWorldRotation(Rotated);
			//GetMesh()->SetWorldRotation(CameraForward.Rotation());
			//FRotator MeshLocalRot = GetMesh()->RelativeRotation;
			//MeshLocalRot.Yaw = 0.0f;
			//GetMesh()->SetRelativeRotation(MeshLocalRot);
		}

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);


	}
}

void ABirdPlayer::MoveRight(float Value)
{
	if (!bInputEnabled) return;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (bIsGliding)
		{
			//FVector CameraForward = ThirdPersonCamera->GetForwardVector();
			//CameraForward.Y = 0.0f;

			//FRotator CameraRotation = CameraForward.Rotation();

			////FRotator Rotation = GetMesh()->RelativeRotation;
			////UE_LOG(LogTemp, Warning, TEXT("Before %s"), *Rotation.ToString());
			//CameraRotation.Roll += Value * MaxGlideRollRotate;
			//CameraRotation.Roll = FMath::Clamp(CameraRotation.Roll, -MaxGlideRollRotate, MaxGlideRollRotate);

			//GetMesh()->SetWorldRotation(CameraRotation);
			//UE_LOG(LogTemp, Warning, TEXT("After %s"), *Rotation.ToString());

			//GetMesh()->SetRelativeRotation(Rotation);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, Rotation.ToString());
		}

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABirdPlayer::StartJump()
{
	if (!bInputEnabled) return;
	SwitchGlide(false);
	GetWorldTimerManager().SetTimer(JumpHoldTimerHandle, this, &ABirdPlayer::StartGlide, JumpTimeToGlide, false);
	JumpHeld = true;
	if (GetCharacterMovement()->IsFalling())
	{
		DoubleJump = true;
		GetWorldTimerManager().SetTimer(DoubleJumpTimerHandle, this, &ABirdPlayer::ApplyDoubleJump, DoubleJumpDelay, false);
	}
	else
	{
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
	//JumpHeld = true;
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
	GetCharacterMovement()->GravityScale = 0.0f;
	FVector DashDirectionForce = ThirdPersonCamera->GetForwardVector() * DashForce;
	DashDirectionForce.Z = 0.0f;
	LaunchCharacter(DashDirectionForce, true, true);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ABirdPlayer::FinishDash, DashTimer, false);
	IsDashing = true;
	bIsGliding = false;

	FRotator Rotation = Controller->GetControlRotation();
	Rotation.Yaw = DashDirectionForce.Rotation().Yaw;
	Controller->SetControlRotation(Rotation);
}

void ABirdPlayer::FinishDash()
{
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetCharacterMovement()->GravityScale = NormalGravity;
	IsDashing = false;

	FRotator Rotation = GetMesh()->RelativeRotation;
	Rotation.Roll = 0.0f;
	GetMesh()->RelativeRotation = Rotation;
}

void ABirdPlayer::SwitchGlide(bool IsGliding)
{
	bIsGliding = IsGliding;
	if (bIsGliding)
	{
		GetCharacterMovement()->GravityScale = GlidingGravity;
		GetCharacterMovement()->AirControl = GlidingAirControl;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//bHasGlided = true;
	}
	else
	{
		GetCharacterMovement()->GravityScale = NormalGravity;
		GetCharacterMovement()->AirControl = NormalAirControl;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void ABirdPlayer::MoveToCheckpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Activating"));
	if (this)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerFound"));
		if (this->FindComponentByClass<UplayerCheckpointMechanics>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Mechanics Script Found"));
			this->FindComponentByClass<UplayerCheckpointMechanics>()->MoveToCurrentCheckpoint();
		}
	}
}

void ABirdPlayer::NectarGathering()
{
	if (bTouchingNectar)
	{
		iInputDelay = 60;
		bInputEnabled = false;
		bCanGlide = true;
		bHasGlided = false;
	}
}

void ABirdPlayer::InputDelayer()
{
	if (iInputDelay >= 0)
	{
		iInputDelay--;
	}
	else
	{
		bInputEnabled = true;
	}
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
