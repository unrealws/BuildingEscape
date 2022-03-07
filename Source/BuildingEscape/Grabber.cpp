// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerLocation;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	
	// UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString());
	DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + Reach * PlayerRotation.Vector(), FColor(0xffff00), false, 0.f, 0U, 2.f);
}
