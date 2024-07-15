// Fill out your copyright notice in the Description page of Project Settings.


#include "AIExample/Public/EnemyCharacter.h"
#include "EnemyAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SetProperties.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAIController::StaticClass();
	
	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	 
	// set in BP instead
	SetBoolProperty( MovementComponent, "bUseAccelerationForPaths", true );

	const bool Value = MovementComponent->UseAccelerationForPathFollowing();
	check(Value);

}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if( !Controller )
	{
		// AIControllerClass set in ctor, if placed in level this is not needed
		// but if dynamically spawned it is
		SpawnDefaultController();
	}
}

