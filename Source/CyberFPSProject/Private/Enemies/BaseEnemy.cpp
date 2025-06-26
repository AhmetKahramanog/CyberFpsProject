#include "Enemies/BaseEnemy.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraComponent.h"
#include "MyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "InteractItems/EnemiesZone.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyWeaponKeeper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponKeeper"));
	EnemyWeaponKeeper->SetupAttachment(GetMesh(), FName("WeaponSocket"));

}


void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyCurrentHealth = MaxHealth;

	PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

}



void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter) { return; }

	EnemyMovement(DeltaTime);

	//GetMesh()->HideBoneByName("head", EPhysBodyOp::PBO_None);

}

void ABaseEnemy::EnemyMovement(float DeltaTime)
{
	if (Distance() < FollowRange)
	{
		EnemyRotation(DeltaTime);
		if (Distance() > AttackRange)
		{
			FVector Direction = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			AddMovementInput(Direction * FollowSpeed * DeltaTime);
		}
	}
}

void ABaseEnemy::EnemyRotation(float DeltaTime)
{
	FVector Direction = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	FRotator SmoothLook = FMath::RInterpTo(GetActorRotation(), Direction.Rotation(), DeltaTime, RotationSpeed);
	SetActorRotation(SmoothLook);
}

float ABaseEnemy::Distance()
{
	if (PlayerCharacter)
	{
		return FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
	}
	return 0.f;
}

void ABaseEnemy::EnemyBackForce(float Force)
{
	FVector Forward = GetActorForwardVector();
	LaunchCharacter(-Forward * Force, true, false);
}

void ABaseEnemy::EnemyTakeDamage(float Amount)
{
	EnemyCurrentHealth -= Amount;
	if (EnemyCurrentHealth <= 0)
	{
		BloodEffectSpawn("spine_01");
		IsDie = true;
		Die();
	}
}

void ABaseEnemy::Die()
{
	if (OwnerZone)
	{
		OwnerZone->OnEnemyDied(this);
	}
	Destroy();
}

void ABaseEnemy::EnemyAttack()
{
	UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();
	if (EnemyAnimInstance && Distance() < AttackRange)
	{
		AttackIndex = FMath::FRandRange(0.f, AttackMontages.Num());
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("AttackIndex : %d"), AttackIndex));
		EnemyAnimInstance->Montage_Play(AttackMontages[AttackIndex]);
	}
}

void ABaseEnemy::BloodEffectDespawn(FTimerHandle& TimerHandle, UFXSystemComponent* Component)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([Component]
		{
			if (Component)
			{
				Component->Deactivate();
				Component->DestroyComponent();
			}
		}), 2.f, false);
}

void ABaseEnemy::BloodEffectSpawn(FName BoneName)
{
	if (!BloodEffect || !BloodHeadEffect) { return; }
	if (BloodComponent)
	{
		BloodComponent->Deactivate();
		BloodComponent->DestroyComponent();
	}
	if (BloodHeadComponent)
	{
		BloodHeadComponent->Deactivate();
		BloodHeadComponent->DestroyComponent();
	}
	FVector SpawnLocation = GetMesh()->GetBoneLocation(BoneName);

	if (BoneName == "head")
	{
		BloodHeadComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodHeadEffect, SpawnLocation, FRotator::ZeroRotator);
		BloodEffectDespawn(BloodHeadEffectHandle, BloodHeadComponent);
	}	
	else
	{
		BloodComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, SpawnLocation, FRotator::ZeroRotator);
		BloodEffectDespawn(BloodEffectHandle, BloodComponent);
	}
}



void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

