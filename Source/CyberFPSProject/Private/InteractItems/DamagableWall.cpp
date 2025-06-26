#include "InteractItems/DamagableWall.h"
#include "Components/BoxComponent.h"
#include <Enemies/BaseEnemy.h>

ADamagableWall::ADamagableWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);

	WallBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBoxCollider"));
	WallBoxCollider->SetupAttachment(WallMesh);

}

void ADamagableWall::BeginPlay()
{
	Super::BeginPlay();

	WallBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADamagableWall::OnWallDamageBeginOverlap);
	
	WallMesh->OnComponentHit.AddDynamic(this,&ADamagableWall::OnWallHitGround);
	WallMesh->SetNotifyRigidBodyCollision(true);

}

void ADamagableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamagableWall::OnWallDamageBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
		{
			Enemy->EnemyTakeDamage(1000.f);
		}
	}
}

void ADamagableWall::OnWallHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag("Ground"))
	{
		WallBoxCollider->SetBoxExtent(FVector(0.f, 0.f, 0.f));
		WallMesh->SetSimulatePhysics(false);
	}
}

