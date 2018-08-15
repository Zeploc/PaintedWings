// Fill out your copyright notice in the Description page of Project Settings.

#include "BirdPlayer.h"


#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineUtils.h"

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

}

// Called when the game starts or when spawned
void ABirdPlayer::BeginPlay()
{
	Super::BeginPlay();
	NormalGravity = GetCharacterMovement()->GravityScale;
	NormalAirControl = GetCharacterMovement()->AirControl;	
}

// Called every frame
void ABirdPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	if (!IsJumpProvidingForce())
	{
		//bIsGliding = false;
	}
}

// Called to bind functionality to input
void ABirdPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABirdPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABirdPlayer::StopJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABirdPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABirdPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABirdPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABirdPlayer::LookUpAtRate);
}

void ABirdPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void ABirdPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABirdPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
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

			FVector CameraForward = ThirdPersonCamera->GetForwardVector();
			CameraForward.Y = 0.0f;

			UE_LOG(LogTemp, Warning, TEXT("Camera Rot %s"), *CameraForward.ToString());

			//GetMesh()->SetWorldRotation(Rotation);
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABirdPlayer::MoveRight(float Value)
{
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


void ABirdPlayer::StartGlide()
{
	GetWorldTimerManager().ClearTimer(JumpHoldTimerHandle);
	if (JumpHeld)
	{
		SwitchGlide(true);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Setting Glide");
	}
}

void ABirdPlayer::StartJump()
{
	Jump();
	JumpHeld = true;
	SwitchGlide(false);
	GetWorldTimerManager().SetTimer(JumpHoldTimerHandle, this, &ABirdPlayer::StartGlide, JumpTimeToGlide, false);
}
void ABirdPlayer::StopJump()
{
	StopJumping();
	JumpHeld = false;
	SwitchGlide(false);
}

void ABirdPlayer::SwitchGlide(bool IsGliding)
{
	bIsGliding = IsGliding;
	if (bIsGliding)
	{
		GetCharacterMovement()->GravityScale = GlidingGravity;
		GetCharacterMovement()->AirControl = GlidingAirControl;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->GravityScale = NormalGravity;
		GetCharacterMovement()->AirControl = NormalAirControl;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
	}
}
