#include "Enemies/BaseEnemyAnimNotify.h"
#include "Enemies/BaseEnemy.h"
#include <Kismet/KismetSystemLibrary.h>
#include <MyCharacter.h>


void UBaseEnemyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) { return; }
	BaseEnemyCharacter = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (!BaseEnemyCharacter) { return; }
	OpenCollision();
}

void UBaseEnemyAnimNotify::OpenCollision()
{
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(BaseEnemyCharacter);
	FVector Start = BaseEnemyCharacter->GetActorLocation();
	FVector End = Start + (BaseEnemyCharacter->GetActorForwardVector() * 500.f);
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		20.f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		IgnoredActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (IsHit)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(HitResult.GetActor());
		if (Player)
		{
			Player->GetDamage(20.f);
		}
	}
}


