#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "Kismet/GameplayStatics.h"
#include <BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h>
#include "SetProperties.h"

namespace
{
	const FName TargetActor{ TEXT("TargetActor") };
}

// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Blackboard and Behavior Tree Components
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// popluate the target so we move toward it
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	BlackboardComponent->SetValueAsObject(TargetActor, PlayerPawn);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// allow overriding BehaviorTree for testing
	if (!BehaviorTree)
	{
		BehaviorTree = NewObject<UBehaviorTree>(this, TEXT("BehaviorTree"));
		BehaviorTree->BlackboardAsset = NewObject<UBlackboardData>(this, TEXT("BlackboardData"));

		// Add keys to the Blackboard
		FBlackboardEntry Entry;
		Entry.EntryName = TargetActor;
		Entry.KeyType = NewObject<UBlackboardKeyType_Object>(BehaviorTree->BlackboardAsset);
		BehaviorTree->BlackboardAsset->Keys.Add(Entry);

		BlackboardComponent->InitializeBlackboard( *BehaviorTree->BlackboardAsset );

		// make a selector
		FBlackboardKeySelector BlackboardKeySelector;
		BlackboardKeySelector.SelectedKeyName = TargetActor;
		BlackboardKeySelector.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();
		BlackboardKeySelector.ResolveSelectedKey( *BehaviorTree->BlackboardAsset );
		check( static_cast<int32>(BlackboardKeySelector.GetSelectedKeyID() ) >= 0 );
		check(BlackboardKeySelector.GetSelectedKeyID() != FBlackboard::InvalidKey );

		// Create root node and moveto node
		BehaviorTree->RootNode = NewObject<UBTComposite_Sequence>(BehaviorTree, TEXT("RootNode"));

		UBTTask_RotateToFaceBBEntry* RotateToFaceNode = NewObject<UBTTask_RotateToFaceBBEntry>(BehaviorTree->RootNode, TEXT("RotateToFace"));
		SetBlackboardKeySelectorProperty(RotateToFaceNode, "BlackboardKey", BlackboardKeySelector);

		UBTTask_MoveTo* MoveToTaskNode = NewObject<UBTTask_MoveTo>(BehaviorTree->RootNode, TEXT("MoveToTask"));
		SetBlackboardKeySelectorProperty(MoveToTaskNode, "BlackboardKey", BlackboardKeySelector);

		// connect into tree
		BehaviorTree->RootNode->Children.Add({ .ChildTask = RotateToFaceNode });
		BehaviorTree->RootNode->Children.Add({ .ChildTask = MoveToTaskNode } );

		RunBehaviorTree( BehaviorTree );
	}
}


