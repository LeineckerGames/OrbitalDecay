// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOrbitalDecay_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");	ORBITALDECAY_API UFunction* Z_Construct_UDelegateFunction_OrbitalDecay_BulletCountUpdatedDelegate__DelegateSignature();
	ORBITALDECAY_API UFunction* Z_Construct_UDelegateFunction_OrbitalDecay_DamagedDelegate__DelegateSignature();
	ORBITALDECAY_API UFunction* Z_Construct_UDelegateFunction_OrbitalDecay_PawnDeathDelegate__DelegateSignature();
	ORBITALDECAY_API UFunction* Z_Construct_UDelegateFunction_OrbitalDecay_SprintStateChangedDelegate__DelegateSignature();
	ORBITALDECAY_API UFunction* Z_Construct_UDelegateFunction_OrbitalDecay_UpdateSprintMeterDelegate__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_OrbitalDecay;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_OrbitalDecay()
	{
		if (!Z_Registration_Info_UPackage__Script_OrbitalDecay.OuterSingleton)
		{
		static UObject* (*const SingletonFuncArray[])() = {
			(UObject* (*)())Z_Construct_UDelegateFunction_OrbitalDecay_BulletCountUpdatedDelegate__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_OrbitalDecay_DamagedDelegate__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_OrbitalDecay_PawnDeathDelegate__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_OrbitalDecay_SprintStateChangedDelegate__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_OrbitalDecay_UpdateSprintMeterDelegate__DelegateSignature,
		};
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/OrbitalDecay",
			SingletonFuncArray,
			UE_ARRAY_COUNT(SingletonFuncArray),
			PKG_CompiledIn | 0x00000000,
			0x3BB144D1,
			0xCD4F76AB,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_OrbitalDecay.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_OrbitalDecay.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_OrbitalDecay(Z_Construct_UPackage__Script_OrbitalDecay, TEXT("/Script/OrbitalDecay"), Z_Registration_Info_UPackage__Script_OrbitalDecay, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x3BB144D1, 0xCD4F76AB));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
