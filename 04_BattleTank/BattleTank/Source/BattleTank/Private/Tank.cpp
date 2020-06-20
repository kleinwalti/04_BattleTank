// Copyright Michael Waltersdorf.

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Adds a Static Mesh 'CollisionMesh' as a component to the Projectile Class
	TankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Mesh"));
	// Forces the Collision Mesh to be the root component of the Projectile Class
	SetRootComponent(TankMesh);
	// Defaults the Details->Collision->'Simulation Generats Hit Events' Setting to true
	TankMesh->SetNotifyRigidBodyCollision(true);
	// Defaults the 'Simulate Physics' setting to true;
	TankMesh->SetSimulatePhysics(true);

	// Adds a Particle System 'Death Explosion' as a component to the Projectile Class
	DeathExplosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Explosion"));
	// Forces the DeathExplosion to be attached to the Tank Body (i.e. TankMesh)
	DeathExplosion->AttachTo( TankMesh );
	// Do not auto-activate the Death Explosion when the Tank spawns
	DeathExplosion->bAutoActivate = false;

	// Adds an Audio Component FiringSound
	FiringSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FiringSound"));
	// Attach it to root component (TankMesh) and set AutoActivate to false as default
	FiringSound->AttachTo( TankMesh );
	FiringSound->bAutoActivate = false;

	// Adds an Audio Component
	ImpactSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Impact Sound"));
	// Attach it to root component (TankMesh) and set AutoActivate to false as default
	ImpactSound->AttachTo( TankMesh );
	ImpactSound->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	// if( ensure( GetRootComponent() ) )
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Root Component is: %s"), *GetRootComponent()->GetName());
	// }
}

// TODO: I think he deleted this one
// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ATank::TakeDamage( float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser )
{
	UE_LOG(LogTemp, Warning, TEXT("%s is taking damage! Damage Amount: %f"), *GetName(), DamageAmount);

	// Reduce health

	// Convert to integer, so we can go to exactly 0
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);

	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);

	// Reduce Current Health by the Amount of Damage applied
	CurrentHealth -= DamageToApply;

	// Play an Impact sound to indicate that the tank was hit
	ImpactSound->Activate();

	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor shuld die"));
		// Use the delegate to broadcast Tanks Death (to the AIController and the Player Controller)
		OnTankDeath();
	}
	
	return DamageToApply;
}

float ATank::GetCurrentHealth() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

// Call this function when the Tank should become dead
void ATank::OnTankDeath()
{
	// Activate the Death Explosion
	DeathExplosion->Activate();
	// Use the delegate to broadcast Tanks Death (to the AIController and the Player Controller)
	OnTankDeathDelegate.Broadcast();
}