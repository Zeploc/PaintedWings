// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingCollectable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/BirdPlayer.h"
#include "Player/BirdController.h"
#include "Engine/World.h"
#include "MovementPoint.h"

AMovingCollectable::AMovingCollectable()
{
	m_TBox = CreateDefaultSubobject<UBoxComponent>("Box");
}

void AMovingCollectable::BeginPlay()
{
	Super::BeginPlay();
	//SphereCollision->OnComponentBeginOverlap.Clear();
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMovingCollectable::OnCompOverlap);

	m_TBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingCollectable::OnBoxOverlapBegin);

	if (fMovementSpeed == 0.0f)
	{
		fMovementSpeed = 100.0f;
	}
	OriginLocation = this->GetActorLocation();
	iCurrentPoint = 1;
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	bForward = true;
}

void AMovingCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRepeating && !bMoving)
	{
		bMoving = true;
	}
	if (bMoving && bForward)
	{
		for (int i = 0; i < MovementPoints.Num(); i++)
		{
			if (MovementPoints[i]->iPointNum == iCurrentPoint)
			{
				FVector destination;
				FRotator rotation;
				MovementPoints[i]->GetSocketWorldLocationAndRotation(FName(), destination, rotation);
				MoveTowards(destination, DeltaTime);
				if ((destination - GetActorLocation()).Size() < 10.0f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Next Point: "));
					iCurrentPoint++;
				}
			}
		}
	}
	if (iCurrentPoint > MovementPoints.Num() && bRepeating)
	{
		iCurrentPoint = 1;
	}
	else if (iCurrentPoint> MovementPoints.Num())
	{
	}
}

void AMovingCollectable::RespawnCollectable()
{
	Super::RespawnCollectable();
	iCurrentPoint = 1;
	SetActorLocation(OriginLocation);
	bMoving = false;
	//UE_LOG(LogTemp, Warning, TEXT("Moving Respawn"));
}

//void AMovingCollectable::OnCompOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	Super::OnCompOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//	//UE_LOG(LogTemp, Warning, TEXT("SphereOver"));
//	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
//	if (BirdRef && !BirdRef->bRespawning)
//	{
//		bMoving = false;
//		//UE_LOG(LogTemp, Warning, TEXT("HIT"));
//	}
//
//}

void AMovingCollectable::OnBoxOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BoxOver"));
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef && !BirdRef->bRespawning && bMoving == false)
	{
		bMoving = true;
		UE_LOG(LogTemp, Warning, TEXT("Activated"));
	}
}

void AMovingCollectable::OnBoxOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void AMovingCollectable::MoveTowards(FVector _location, float DeltaTime)
{
	FVector PosNew;
	FVector Direction = _location - GetActorLocation();
	Direction.Normalize();
	Direction *= fMovementSpeed;
	PosNew = GetActorLocation() + (Direction * DeltaTime);
	SetActorLocation(PosNew);
	//UE_LOG(LogTemp, Warning, TEXT("Direction = %s"), *Direction.ToString());
}
