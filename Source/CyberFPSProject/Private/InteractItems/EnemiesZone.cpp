#include "InteractItems/EnemiesZone.h"
#include "Components/BoxComponent.h"
#include "Enemies/BaseEnemy.h"
#include <MyCharacter.h>

AEnemiesZone::AEnemiesZone()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemiesZoneCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemiesZoneCollider"));
	EnemiesZoneCollider->SetupAttachment(RootComponent);
	EnemiesZoneCollider->SetCollisionProfileName(TEXT("Trigger"));
	EnemiesZoneCollider->SetGenerateOverlapEvents(true);

}


void AEnemiesZone::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FindEnemyHandle, this, &AEnemiesZone::FindEnemiesInZone, 0.05f, false);

	EnemiesZoneCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemiesZone::OnPlayerBeginOverlap);
}

void AEnemiesZone::FindEnemiesInZone()
{
	TArray<AActor*> OverlappedActors;
	EnemiesZoneCollider->GetOverlappingActors(OverlappedActors, ABaseEnemy::StaticClass());
	for (AActor* Actor : OverlappedActors)
	{
		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Actor))
		{
			Enemy->OwnerZone = this;
			EnemiesZone.Add(Enemy);
		}
	}
	EnemyCount = EnemiesZone.Num();
}


void AEnemiesZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Orange, FString::Printf(TEXT("EnemyCount : %d"), EnemyCount));

}

void AEnemiesZone::OnEnemyDied(ABaseEnemy* DeadEnemy)
{
	if (EnemiesZone.Contains(DeadEnemy))
	{
		EnemyCount--;
	}
}


void AEnemiesZone::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
		if (Player && EnemyCount > 0)
		{
			IsPlayerInZone = true;
		}
	}
}

