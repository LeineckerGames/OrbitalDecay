// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OrbitalDecay/OrbitalDecayGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOrbitalDecayGameMode() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
ORBITALDECAY_API UClass* Z_Construct_UClass_AOrbitalDecayGameMode();
ORBITALDECAY_API UClass* Z_Construct_UClass_AOrbitalDecayGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_OrbitalDecay();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AOrbitalDecayGameMode ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AOrbitalDecayGameMode;
UClass* AOrbitalDecayGameMode::GetPrivateStaticClass()
{
	using TClass = AOrbitalDecayGameMode;
	if (!Z_Registration_Info_UClass_AOrbitalDecayGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OrbitalDecayGameMode"),
			Z_Registration_Info_UClass_AOrbitalDecayGameMode.InnerSingleton,
			StaticRegisterNativesAOrbitalDecayGameMode,
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
	return Z_Registration_Info_UClass_AOrbitalDecayGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_AOrbitalDecayGameMode_NoRegister()
{
	return AOrbitalDecayGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AOrbitalDecayGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "OrbitalDecayGameMode.h" },
		{ "ModuleRelativePath", "OrbitalDecayGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GlobalLevel_MetaData[] = {
		{ "Category", "Game Progress" },
		{ "ModuleRelativePath", "OrbitalDecayGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AOrbitalDecayGameMode constinit property declarations ********************
	static const UECodeGen_Private::FIntPropertyParams NewProp_GlobalLevel;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AOrbitalDecayGameMode constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOrbitalDecayGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AOrbitalDecayGameMode_Statics

// ********** Begin Class AOrbitalDecayGameMode Property Definitions *******************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AOrbitalDecayGameMode_Statics::NewProp_GlobalLevel = { "GlobalLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AOrbitalDecayGameMode, GlobalLevel), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GlobalLevel_MetaData), NewProp_GlobalLevel_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AOrbitalDecayGameMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AOrbitalDecayGameMode_Statics::NewProp_GlobalLevel,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayGameMode_Statics::PropPointers) < 2048);
// ********** End Class AOrbitalDecayGameMode Property Definitions *********************************
UObject* (*const Z_Construct_UClass_AOrbitalDecayGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AOrbitalDecayGameMode_Statics::ClassParams = {
	&AOrbitalDecayGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AOrbitalDecayGameMode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayGameMode_Statics::PropPointers),
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AOrbitalDecayGameMode_Statics::Class_MetaDataParams)
};
void AOrbitalDecayGameMode::StaticRegisterNativesAOrbitalDecayGameMode()
{
}
UClass* Z_Construct_UClass_AOrbitalDecayGameMode()
{
	if (!Z_Registration_Info_UClass_AOrbitalDecayGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AOrbitalDecayGameMode.OuterSingleton, Z_Construct_UClass_AOrbitalDecayGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AOrbitalDecayGameMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AOrbitalDecayGameMode);
AOrbitalDecayGameMode::~AOrbitalDecayGameMode() {}
// ********** End Class AOrbitalDecayGameMode ******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayGameMode_h__Script_OrbitalDecay_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AOrbitalDecayGameMode, AOrbitalDecayGameMode::StaticClass, TEXT("AOrbitalDecayGameMode"), &Z_Registration_Info_UClass_AOrbitalDecayGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AOrbitalDecayGameMode), 2036890483U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayGameMode_h__Script_OrbitalDecay_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayGameMode_h__Script_OrbitalDecay_3260996707{
	TEXT("/Script/OrbitalDecay"),
	Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayGameMode_h__Script_OrbitalDecay_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayGameMode_h__Script_OrbitalDecay_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
