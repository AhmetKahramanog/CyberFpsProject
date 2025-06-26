#include "InteractItems/HiddenWall.h"
#include "Components/BoxComponent.h"
#include <MyCharacter.h>
#include "InteractItems/EnemiesZone.h"

AHiddenWall::AHiddenWall()
{
	PrimaryActorTick.bCanEverTick = true;

	HiddenWallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HiddenWallCollider"));
	HiddenWallCollider->SetupAttachment(RootComponent);

}

void AHiddenWall::BeginPlay()
{
	Super::BeginPlay();

}


void AHiddenWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyArea)
	{
		if (EnemyArea->IsPlayerInZone && EnemyArea->EnemyCount > 0)
		{
			WallHandle(ECollisionEnabled::QueryAndPhysics, true);
			HiddenWallCollider->SetCollisionResponseToAllChannels(ECR_Block);
		}
		if (EnemyArea->EnemyCount <= 0)
		{
			WallHandle(ECollisionEnabled::NoCollision, false);
			HiddenWallCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
			EnemyArea->IsPlayerInZone = false;
		}
	}

}

void AHiddenWall::WallHandle(ECollisionEnabled::Type CollisionType, bool OverlapEvent)
{
	HiddenWallCollider->SetCollisionEnabled(CollisionType);
	HiddenWallCollider->SetGenerateOverlapEvents(OverlapEvent);
}