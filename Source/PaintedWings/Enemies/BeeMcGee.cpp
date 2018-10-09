// Fill out your copyright notice in the Description page of Project Settings.

#include "BeeMcGee.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PaintedWings/Player/BirdPlayer.h"
// Sets default values
ABeeMcGee::ABeeMcGee()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(10.0f, 15.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	bCanMove = true;
	BeeTrigger = CreateDefaultSubobject<USphereComponent>("Box");
	BeeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABeeMcGee::OnOverlapBegin);
	BeeTrigger->OnComponentEndOverlap.AddDynamic(this, &ABeeMcGee::OnOverlapEnd);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABeeMcGee::OnKillOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABeeMcGee::OnKillOverlapEnd);
}

// Called when the game starts or when spawned
void ABeeMcGee::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = this->GetActorLocation();
	BeeTrigger->SetWorldLocation(this->GetActorLocation());
}

// Called every frame
void ABeeMcGee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerRef)
	{
		if (Cast<ABirdPlayer>(PlayerRef) && bCanMove)
		{
			FVector Direction = PlayerRef->GetActorLocation() - this->GetActorLocation();
			Direction.Normalize();
			AddMovementInput(Direction, 1.0);

		}
	}
	else
	{
		FVector Direction = OriginalLocation - this->GetActorLocation();
		Direction.Normalize();
		AddMovementInput(Direction, 1.0);
	}
}

// Called to bind functionality to input
void ABeeMcGee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABeeMcGee::ResetMovment()
{
	bCanMove = true;
}

void ABeeMcGee::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		PlayerRef = BirdRef;
	}
}

void ABeeMcGee::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		PlayerRef = nullptr;
	}
}

void ABeeMcGee::OnKillOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		if (BirdRef->fInvincibility <= 0)
		{
			//bCanMove = false;
			BirdRef->fInvincibility = 2.0f;
			BirdRef->iHealth -= 1;
			//FVector LaunchDir = GetActorLocation() - BirdRef->GetActorLocation();
			//LaunchDir.Normalize();
			//LaunchDir *= 10.0f;
			//LaunchCharacter(LaunchDir, false, false);

			//GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &ABeeMcGee::ResetMovment, 1.0f, false, 1.0f);
		}
	}
}

void ABeeMcGee::OnKillOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		if (BirdRef->fInvincibility <= 0)
		{
			//BirdRef->fInvincibility = 2.0f;
			//BirdRef->iHealth -= 1;
		}
	}
}




