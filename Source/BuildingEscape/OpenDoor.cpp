// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitYaw = StartYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += StartYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate set."), *GetOwner()->GetName());
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	ActorThatOpens = PlayerController->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	AActor *Owner = GetOwner();
	FRotator CurrentRotator = Owner->GetActorRotation();
	StartYaw = FMath::Lerp(StartYaw, OpenAngle, DeltaTime*DoorOpenSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("%s Transfrom is %s, and Yaw is %f"), *Owner->GetName(), *Owner->GetActorTransform().ToString(), CurrentRotator.Yaw);
	CurrentRotator.Yaw = StartYaw;
	Owner->SetActorRotation(CurrentRotator);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	AActor *Owner = GetOwner();
	FRotator CurrentRotator = Owner->GetActorRotation();
	StartYaw = FMath::Lerp(StartYaw, InitYaw, DeltaTime*DoorCloseSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("%s Transfrom is %s, and Yaw is %f"), *Owner->GetName(), *Owner->GetActorTransform().ToString(), CurrentRotator.Yaw);
	CurrentRotator.Yaw = StartYaw;
	Owner->SetActorRotation(CurrentRotator);
}

