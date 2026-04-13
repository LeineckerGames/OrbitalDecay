// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OrbitalDecay/ALanderPawn.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeALanderPawn() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_APawn();
ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
ORBITALDECAY_API UClass* Z_Construct_UClass_ALanderPawn();
ORBITALDECAY_API UClass* Z_Construct_UClass_ALanderPawn_NoRegister();
UPackage* Z_Construct_UPackage__Script_OrbitalDecay();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ALanderPawn **************************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_ALanderPawn;
UClass* ALanderPawn::GetPrivateStaticClass()
{
	using TClass = ALanderPawn;
	if (!Z_Registration_Info_UClass_ALanderPawn.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("LanderPawn"),
			Z_Registration_Info_UClass_ALanderPawn.InnerSingleton,
			StaticRegisterNativesALanderPawn,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_ALanderPawn.InnerSingleton;
}
UClass* Z_Construct_UClass_ALanderPawn_NoRegister()
{
	return ALanderPawn::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ALanderPawn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "ALanderPawn.h" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mesh_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Camera_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MyCurrentLevel_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentVelocity_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentAltitude_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Fuel_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxFuel_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Starting fuel, player can change this later\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Starting fuel, player can change this later" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultThrust_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoostedThrust_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Slows fall but doesn't stop it\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Slows fall but doesn't stop it" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FuelDrainIdle_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Applied when correct answer is given\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Applied when correct answer is given" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FuelDrainBoost_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Fuel per second at default thrust\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fuel per second at default thrust" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoostDuration_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Fuel per second during boost\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fuel per second during boost" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultForwardThrust_MetaData[] = {
		{ "Category", "Flight" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// How long a correct answer boost lasts in seconds\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "How long a correct answer boost lasts in seconds" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoostedForwardThrust_MetaData[] = {
		{ "Category", "Flight" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RotationStep_MetaData[] = {
		{ "Category", "Camera" },
		{ "ModuleRelativePath", "ALanderPawn.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RotationInterpSpeed_MetaData[] = {
		{ "Category", "Camera" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// How many degrees each correct answer rotates\n" },
#endif
		{ "ModuleRelativePath", "ALanderPawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "How many degrees each correct answer rotates" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class ALanderPawn constinit property declarations ******************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Mesh;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Camera;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MyCurrentLevel;
	static const UECodeGen_Private::FStructPropertyParams NewProp_CurrentVelocity;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_CurrentAltitude;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Fuel;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxFuel;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DefaultThrust;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_BoostedThrust;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FuelDrainIdle;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FuelDrainBoost;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_BoostDuration;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DefaultForwardThrust;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_BoostedForwardThrust;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RotationStep;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RotationInterpSpeed;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class ALanderPawn constinit property declarations ********************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALanderPawn>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_ALanderPawn_Statics

// ********** Begin Class ALanderPawn Property Definitions *****************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_Mesh = { "Mesh", nullptr, (EPropertyFlags)0x00100000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, Mesh), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mesh_MetaData), NewProp_Mesh_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_Camera = { "Camera", nullptr, (EPropertyFlags)0x00100000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, Camera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Camera_MetaData), NewProp_Camera_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_MyCurrentLevel = { "MyCurrentLevel", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, MyCurrentLevel), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MyCurrentLevel_MetaData), NewProp_MyCurrentLevel_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_CurrentVelocity = { "CurrentVelocity", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, CurrentVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentVelocity_MetaData), NewProp_CurrentVelocity_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_CurrentAltitude = { "CurrentAltitude", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, CurrentAltitude), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentAltitude_MetaData), NewProp_CurrentAltitude_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_Fuel = { "Fuel", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, Fuel), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Fuel_MetaData), NewProp_Fuel_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_MaxFuel = { "MaxFuel", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, MaxFuel), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxFuel_MetaData), NewProp_MaxFuel_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_DefaultThrust = { "DefaultThrust", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, DefaultThrust), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultThrust_MetaData), NewProp_DefaultThrust_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostedThrust = { "BoostedThrust", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, BoostedThrust), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoostedThrust_MetaData), NewProp_BoostedThrust_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_FuelDrainIdle = { "FuelDrainIdle", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, FuelDrainIdle), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FuelDrainIdle_MetaData), NewProp_FuelDrainIdle_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_FuelDrainBoost = { "FuelDrainBoost", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, FuelDrainBoost), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FuelDrainBoost_MetaData), NewProp_FuelDrainBoost_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostDuration = { "BoostDuration", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, BoostDuration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoostDuration_MetaData), NewProp_BoostDuration_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_DefaultForwardThrust = { "DefaultForwardThrust", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, DefaultForwardThrust), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultForwardThrust_MetaData), NewProp_DefaultForwardThrust_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostedForwardThrust = { "BoostedForwardThrust", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, BoostedForwardThrust), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoostedForwardThrust_MetaData), NewProp_BoostedForwardThrust_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_RotationStep = { "RotationStep", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, RotationStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RotationStep_MetaData), NewProp_RotationStep_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ALanderPawn_Statics::NewProp_RotationInterpSpeed = { "RotationInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALanderPawn, RotationInterpSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RotationInterpSpeed_MetaData), NewProp_RotationInterpSpeed_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ALanderPawn_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_Mesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_Camera,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_MyCurrentLevel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_CurrentVelocity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_CurrentAltitude,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_Fuel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_MaxFuel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_DefaultThrust,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostedThrust,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_FuelDrainIdle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_FuelDrainBoost,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostDuration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_DefaultForwardThrust,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_BoostedForwardThrust,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_RotationStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALanderPawn_Statics::NewProp_RotationInterpSpeed,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALanderPawn_Statics::PropPointers) < 2048);
// ********** End Class ALanderPawn Property Definitions *******************************************
UObject* (*const Z_Construct_UClass_ALanderPawn_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APawn,
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALanderPawn_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ALanderPawn_Statics::ClassParams = {
	&ALanderPawn::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ALanderPawn_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ALanderPawn_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ALanderPawn_Statics::Class_MetaDataParams), Z_Construct_UClass_ALanderPawn_Statics::Class_MetaDataParams)
};
void ALanderPawn::StaticRegisterNativesALanderPawn()
{
}
UClass* Z_Construct_UClass_ALanderPawn()
{
	if (!Z_Registration_Info_UClass_ALanderPawn.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ALanderPawn.OuterSingleton, Z_Construct_UClass_ALanderPawn_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ALanderPawn.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, ALanderPawn);
ALanderPawn::~ALanderPawn() {}
// ********** End Class ALanderPawn ****************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_ALanderPawn_h__Script_OrbitalDecay_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ALanderPawn, ALanderPawn::StaticClass, TEXT("ALanderPawn"), &Z_Registration_Info_UClass_ALanderPawn, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ALanderPawn), 4036200326U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_ALanderPawn_h__Script_OrbitalDecay_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_ALanderPawn_h__Script_OrbitalDecay_1248760891{
	TEXT("/Script/OrbitalDecay"),
	Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_ALanderPawn_h__Script_OrbitalDecay_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_ALanderPawn_h__Script_OrbitalDecay_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
