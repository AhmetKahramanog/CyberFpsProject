#include "MyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <InteractItems/InteractItem.h>
#include "BaseWeapon.h"
#include "NiagaraComponent.h"
#include <NiagaraFunctionLibrary.h>
#include "Materials/MaterialInstance.h"
#include "Enemies/BaseEnemy.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include <InteractItems/DamagableWall.h>

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprinArm"));
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	WeaponKeeper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponKeeper"));
	WeaponKeeper->SetupAttachment(Camera);

}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	CurrentHealth = MaxHealth;
	
	if (PlayerUIWidgetClass)
	{
		PlayerUIWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerUIWidgetClass);
		if (PlayerUIWidget)
		{
			PlayerUIWidget->AddToViewport();
			AmmoText = Cast<UTextBlock>(PlayerUIWidget->GetWidgetFromName("AmmoText"));
			AmmoCapacitytText = Cast<UTextBlock>(PlayerUIWidget->GetWidgetFromName("AmmoCapacityText"));
			CrossImage = Cast<UImage>(PlayerUIWidget->GetWidgetFromName(TEXT("CrosshairImage")));
			HealthBarProgress = Cast<UProgressBar>(PlayerUIWidget->GetWidgetFromName("HealthBarProgress"));
			StaminaBar = Cast<UProgressBar>(PlayerUIWidget->GetWidgetFromName("StaminaBarProgress"));
			StaminaBar->SetRenderOpacity(0.f);
			InteractText = Cast<UTextBlock>(PlayerUIWidget->GetWidgetFromName("InteractText"));
			InteractText->SetVisibility(ESlateVisibility::Hidden);
			GoldCountText = Cast<UTextBlock>(PlayerUIWidget->GetWidgetFromName("GoldCountText"));
		}
	}

	UCapsuleComponent* CapsuleCollider = Cast<UCapsuleComponent>(GetCapsuleComponent());
	if (CapsuleCollider)
	{
		CapsuleCollider->OnComponentBeginOverlap.AddDynamic(this,&AMyCharacter::OnBeginOverlap);
	}

	CurrentStamina = MaxStamina;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MiddleLineTrace();

	ClimbWall();

	InterpDashTest(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(1, 0, FColor::Emerald, FString::Printf(TEXT("GoldAmount : %d"), GoldCount));
	
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += 0.35f;
	}
	if (CurrentStamina < 0)
	{
		CurrentStamina = 0.f;
	}

	if (IsShowStaminaBar && StaminaBar)
	{
		StaminaBar->SetPercent(CurrentStamina / MaxStamina);
		float CurrentOpactiy = StaminaBar->GetRenderOpacity();
		CurrentOpactiy -= DeltaTime * 0.5f;
		CurrentOpactiy = FMath::Clamp(CurrentOpactiy, 0.f, 1.f);
		StaminaBar->SetRenderOpacity(CurrentOpactiy);
	}


	if (CurrentlyGun)
	{
		if (IsFire)
		{
			GunAnimation(WeaponInitialRotation, 20.f, DeltaTime);
		}
		/*if (IsGunAnimated)
		{
			GunAnimation(WeaponInitialRotation, 35.f, DeltaTime);
		}*/
		if (IsReloaded)
		{
			GunAnimation(FRotator(50.f, 0.f, 0.f), 1.f, DeltaTime);
		}
	}
	GoldCountText->SetText(FText::FromString(FString::FromInt(GoldCount)));
}

void AMyCharacter::GunAnimation(FRotator TargetRotation, float RotationTimer,float DeltaTime)
{
	FRotator CurrentRotation = CurrentlyGun->GunMesh->GetRelativeRotation();
	CurrentlyGun->GunMesh->AddRelativeRotation(TargetRotation);
	float SmoothReload = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Pitch, DeltaTime, RotationTimer);
	CurrentRotation.Pitch = SmoothReload;
	CurrentlyGun->SetActorRelativeRotation(CurrentRotation);
}

void AMyCharacter::MiddleLineTrace()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 200.f);
	FHitResult TraceResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(TraceResult, Start, End, ECollisionChannel::ECC_Camera, Params);
	if (IsHit)
	{
		if (TraceResult.GetActor()->GetClass()->ImplementsInterface(UInteractItem::StaticClass()))
		{
			Interacted = Cast<IInteractItem>(TraceResult.GetActor());
			if (Interacted)
			{
				InteractText->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		InteractText->SetVisibility(ESlateVisibility::Hidden);
		Interacted = nullptr;
		CurrentWeapon = nullptr;
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveDirection = Value.Get<FVector2D>();
	if (Controller)
	{
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection,MoveDirection.Y);
		AddMovementInput(RightDirection, MoveDirection.X);
	}
}

void AMyCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMyCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMyCharacter::Dodge()
{
	/*if (!IsClimbing)
	{
		LaunchCharacter(GetActorForwardVector() * DodgeForce, true, false);
	}*/
	if (CurrentStamina > 60.f && StaminaBar)
	{
		IsShowStaminaBar = true;
		StaminaBar->SetRenderOpacity(1.f);
		IsDashing = true;
		CurrentStamina -= 60.f;
		GetWorld()->GetTimerManager().SetTimer(DashTestHandle, FTimerDelegate::CreateLambda([this]
			{
				IsDashing = false;
			}), 0.09f, false);
	}
}

void AMyCharacter::InterpDashTest(float DeltaTime)
{
	if (IsDashing)
	{
		FVector DashStart = GetActorLocation();
		FVector DashTarget = DashStart + (GetActorForwardVector() * 50.f);
		FVector DashDirection = FMath::VInterpTo(DashStart, DashTarget, DeltaTime, 50.f);
		SetActorLocation(DashDirection);
	}
}

void AMyCharacter::Jump()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		FVector Jump(0.f, 0.f, JumpForce);
		LaunchCharacter(Jump, false, true);
	}
}

void AMyCharacter::ClimbWall()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 200.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Camera, Params);
	if (IsHit)
	{
		if (Hit.GetActor()->ActorHasTag("ClimbWall"))
		{
			FVector WallNormal = Hit.ImpactNormal;

			GetCharacterMovement()->GravityScale = 0.f;

			GetCharacterMovement()->SetMovementMode(MOVE_Flying);

			IsClimbing = true;

			FVector WallForce = -WallNormal * 7000000.f;
			GetCharacterMovement()->AddForce(WallForce);
			GetWorld()->GetTimerManager().SetTimer(ClimbWallHandle, FTimerDelegate::CreateLambda([this]
				{
					GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					GetCharacterMovement()->GravityScale = 3.f;
					IsClimbing = false;
				}), 0.35f, false);
		}
	}
}

void AMyCharacter::HandleHitResult(FVector Start,FVector End)
{
	if (!CurrentlyGun) { return; }
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(FireResult, Start, End, ECC_Camera, Params);
	if (ADamagableWall* Wall = Cast<ADamagableWall>(FireResult.GetActor()))
	{
		Wall->WallMesh->SetSimulatePhysics(true);
	}
	BaseEnemyShoot = Cast<ABaseEnemy>(FireResult.GetActor());
	if (ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(FireResult.GetActor()))
	{
		if (CurrentlyGun->IsBleedUnlock)
		{
			CurrentlyGun->RifleShootCount++;
			if (CurrentlyGun->RifleShootCount >= 9)
			{
				// EnemyTakeDamage'i interface e taþý.
				CurrentlyGun->IsBleeding = true;
				GetWorld()->GetTimerManager().SetTimer(CurrentlyGun->TakeEnemyBleedingHandle, FTimerDelegate::CreateLambda([BaseEnemy]
					{
						GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Purple, TEXT("Bleeding"));
						BaseEnemy->EnemyTakeDamage(5.f);
					}), CurrentlyGun->BleedingCoolDown, true);
			}
		}
		FName ClosestBone = BaseEnemy->GetMesh()->FindClosestBone(FireResult.ImpactPoint);
		if (CurrentlyGun->IsStrongerBullet)
		{
			if (CurrentlyGun->ShotgunFireCount >= 4)
			{
				GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, TEXT("Five Bullet"));
				CurrentlyGun->GunDamage += CurrentlyGun->StrongerBullet;
			}
		}
		if (FireResult.BoneName == "head")
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("HeadShot"));
			BaseEnemy->EnemyTakeDamage(CurrentlyGun->GunDamage + CurrentlyGun->HeadShotDamage);
			BaseEnemy->EnemyBackForce(CurrentlyGun->FireForce);
		}
		else
		{
			BaseEnemy->EnemyTakeDamage(CurrentlyGun->GunDamage);
		}
		if (CurrentlyGun->ShotgunFireCount >= 4)
		{
			CurrentlyGun->GunDamage -= CurrentlyGun->StrongerBullet;
			CurrentlyGun->ShotgunFireCount = 0;
		}
		BaseEnemy->BloodEffectSpawn(ClosestBone);
	}
	else
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHole, FVector(20.f), FireResult.ImpactPoint, FireResult.ImpactNormal.Rotation(), 5.f);
	}
}

void AMyCharacter::FireLineTrace()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * CurrentlyGun->FireShootRange);
	HandleHitResult(Start, End);
}

void AMyCharacter::ShotgunTrace()
{
	for (int32 i = 0; i < 8; i++)
	{
		FVector Start = Camera->GetComponentLocation();
		FVector ForwardVector = Camera->GetForwardVector();
		FRotator SpreadRotation(FMath::FRandRange(-7.f, 7.f), FMath::FRandRange(-7.f, 7.f), FMath::FRandRange(-7.f,7.f));
		FVector ShotgunDirection = SpreadRotation.RotateVector(ForwardVector);
		FVector End = Start + (ShotgunDirection * 500.f);
		HandleHitResult(Start, End);
	}
}

void AMyCharacter::GetDamage(float Amount)
{
	if (!PlayerController || !BP_DamageCameraShake) { return; }
	CurrentHealth -= Amount;
	PlayerController->ClientStartCameraShake(BP_DamageCameraShake);
	HealthBarUpdate();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%f"), CurrentHealth));
}


void AMyCharacter::Fire()
{
	if (CurrentlyGun && CurrentlyGun->CurrentAmmo > 0 && !IsReloaded && !IsFire)
	{
		CurrentlyGun->CurrentAmmo--;
		GunFireEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GunFireEffect, CurrentlyGun->GunFireEFfectSpawn->GetComponentLocation());
		WeaponInitialRotation = CurrentlyGun->GunMesh->GetRelativeRotation();
		CurrentlyGun->GunMesh->AddRelativeRotation(FRotator(80.f, 0.f, 0.f));
		IsFire = true;
		FireCount++;
		AmmoTextUpdate();
		if (BaseEnemyShoot)
		{
			CurrentlyGun->ShotgunFireCount++;
		}
		IsGunAnimated = false;
		if (UsedWeaponType == EWeaponType::Shotgun)
		{
			ShotgunTrace();
		}
		else
		{
			FireLineTrace();
		}
		/*if (CurrentlyGun->GunMesh->GetRelativeRotation().Pitch >= 40.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, TEXT("Gun Animated"));
			IsGunAnimated = true;
		}*/
		GetWorld()->GetTimerManager().SetTimer(FireHandle, FTimerDelegate::CreateLambda([this]
			{
				IsFire = false;
			}), CurrentlyGun->FireTimer, false);
		GetWorld()->GetTimerManager().SetTimer(FireEffectHandle, FTimerDelegate::CreateLambda([this]
			{
				if (GunFireEffectComponent)
				{
					GunFireEffectComponent->Deactivate();
					GunFireEffectComponent->DestroyComponent();
				}
			}), 0.1f, false);
		if (FireCount <= CurrentlyGun->FireShootCount)
		{
			GetWorldTimerManager().SetTimer(FireDelayHandle, this, &AMyCharacter::Fire, 0.12f, false);
		}
		else
		{
			FireCount = 0;
		}
	}
}

void AMyCharacter::AssultRifleFireStart()
{
	if (UsedWeaponType == EWeaponType::AssultRifle)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AMyCharacter::Fire, CurrentlyGun->RifleFireShootSpeed, true);
	}
}

void AMyCharacter::AssultRifleFireStop()
{
	if (UsedWeaponType == EWeaponType::AssultRifle)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
	}
}

void AMyCharacter::AmmoTextUpdate()
{
	if (!AmmoText || !CurrentlyGun || !AmmoCapacitytText) { return; }

	AmmoText->SetText(FText::FromString(FString::FromInt(CurrentlyGun->CurrentAmmo)));
	AmmoCapacitytText->SetText(FText::FromString(FString::FromInt(CurrentlyGun->AmmoCount)));
}

void AMyCharacter::HealthBarUpdate()
{
	if (HealthBarProgress)
	{
		HealthBarProgress->SetPercent(CurrentHealth / MaxHealth);
	}
}

void AMyCharacter::OnWeaponReloaded()
{
	if (CurrentlyGun && !IsReloaded && CurrentlyGun->AmmoCount > 0)
	{
		IsReloaded = true;
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, FTimerDelegate::CreateLambda([this]
			{
				CurrentlyGun->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
				CurrentlyGun->CurrentAmmo = CurrentlyGun->Ammo;
				IsReloaded = false;
				CurrentlyGun->AmmoCount--;
				AmmoTextUpdate();
			}), CurrentlyGun->ReloadTimer, false);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookDirection = Value.Get<FVector2D>();
	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y * -1);
}

void AMyCharacter::SetGold(int32 GoldAmount)
{
	GoldCount += GoldAmount;
}

int32 AMyCharacter::GetGold()
{
	return GoldCount;
}

void AMyCharacter::SetAmmo(int32 AmmoAmount)
{
	if (CurrentlyGun)
	{
		CurrentlyGun->AmmoCount += AmmoAmount;
		AmmoTextUpdate();
	}
}

void AMyCharacter::SetHealth(int32 HealthAmount)
{
	CurrentHealth += HealthAmount;
	HealthBarUpdate();
}

void AMyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractItem::StaticClass()))
	{
		if (IInteractItem* InteractCollision = Cast<IInteractItem>(OtherActor))
		{
			InteractCollision->InteractCollision(this);
		}
	}
}

void AMyCharacter::PickUpWeapon()
{
	if (!Interacted) { return; }

	Interacted->Interact(this);

	if (CurrentWeapon)
	{
		FActorSpawnParameters Spawn;
		Spawn.Owner = this;
		WeaponCloned = GetWorld()->SpawnActor<ABaseWeapon>(CurrentWeapon->GetClass(),WeaponKeeper->GetComponentLocation(),FRotator::ZeroRotator, Spawn);
		if (WeaponCloned)
		{
			WeaponCloned->GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WeaponsInventory.Add(WeaponCloned);
			WeaponCloned->SetActorHiddenInGame(true);
			Interacted = nullptr;
		}
		CurrentWeapon->Destroy();
	}
}

void AMyCharacter::SelectWeapon(FKey PressedKey)
{
	if (PressedKey == EKeys::One)
	{
		EquippedWeapon(0);
	}
	else if (PressedKey == EKeys::Two)
	{
		EquippedWeapon(1);
	}
	else if (PressedKey == EKeys::Three)
	{
		EquippedWeapon(2);
	}
}

void AMyCharacter::EquippedWeapon(int32 Index)
{
	if (WeaponsInventory.IsValidIndex(Index))
	{
		if (CurrentlyGun)
		{
			CurrentlyGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentlyGun->SetActorHiddenInGame(true);
		}

		if (WeaponCloned)
		{
			GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
			UsedWeaponType = WeaponsInventory[Index]->WeaponType;
			IsReloaded = false;
			WeaponsInventory[Index]->AttachToComponent(WeaponKeeper, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			WeaponsInventory[Index]->SetActorHiddenInGame(false);
			CurrentlyGun = WeaponsInventory[Index];
			AmmoTextUpdate();
		}
		if (CrossImage)
		{
			if (UsedWeaponType == EWeaponType::Shotgun)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(ShotgunCrossImage);
				Brush.ImageSize = FVector2D(200.f, 200.f);
				CrossImage->SetBrush(Brush);
			}
			else
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(NormalCrossImage);
				Brush.ImageSize = FVector2D(64.f, 64.f);
				CrossImage->SetBrush(Brush);
			}
		}
	}
}


void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AMyCharacter::Dodge);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyCharacter::PickUpWeapon);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyCharacter::Fire);
		EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnWeaponReloaded);
	}
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
	PlayerInputComponent->BindAction("HoldFire", IE_Pressed, this, &AMyCharacter::AssultRifleFireStart);
	PlayerInputComponent->BindAction("HoldFire", IE_Released, this, &AMyCharacter::AssultRifleFireStop);
	PlayerInputComponent->BindAction("SelectedWeapon", IE_Pressed, this, &AMyCharacter::SelectWeapon);

}

