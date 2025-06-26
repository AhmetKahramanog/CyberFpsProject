#include "Enemies/RangerEnemy.h"
#include "MyCharacter.h"
#include "NiagaraComponent.h"
#include <NiagaraFunctionLibrary.h>

ARangerEnemy::ARangerEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GunFireEffectSpawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunFireEffectSpawn"));
	GunFireEffectSpawn->SetupAttachment(RootComponent);

}


void ARangerEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ShootHandle,this,&ARangerEnemy::EnemyAttack,AttackTimer,true);
	
}

void ARangerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARangerEnemy::EnemyAttack()
{
	Super::EnemyAttack();
	if (Distance() < AttackRange)
	{
		GetWorldTimerManager().SetTimer(DelatShootHandle, this, &ARangerEnemy::DelayShooting, 0.1f, false);
		GunFireEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GunFireEffect, GunFireEffectSpawn->GetComponentLocation(), FRotator::ZeroRotator);
		GetWorld()->GetTimerManager().SetTimer(GunFireHandle, FTimerDelegate::CreateLambda([this]
			{
				if (GunFireEffectComponent)
				{
					GunFireEffectComponent->Deactivate();
					GunFireEffectComponent->DestroyComponent();
				}
			}), 0.3f, false);
	}
}

void ARangerEnemy::DelayShooting()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Ranger Attack!!"));
	if (!PlayerCharacter) { return; }
	FVector Start = EnemyWeaponKeeper->GetComponentLocation();
	/*FVector Direction = (PlayerCharacter->GetActorLocation() - Start).GetSafeNormal();*/
	FVector End = Start + (GetActorForwardVector() * AttackRange);
	FHitResult ShootResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(ShootResult, Start, End, ECollisionChannel::ECC_Pawn, Params);
	if (IsHit)
	{
		AMyCharacter* TargetPlayer = Cast<AMyCharacter>(ShootResult.GetActor());
		if (TargetPlayer)
		{
			TargetPlayer->GetDamage(30.f);
		}
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.0f);
}

void ARangerEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

