// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OrbitalDecayCameraManager.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOrbitalDecayCameraManager() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_APlayerCameraManager();
ORBITALDECAY_API UClass* Z_Construct_UClass_AOrbitalDecayCameraManager();
ORBITALDECAY_API UClass* Z_Construct_UClass_AOrbitalDecayCameraManager_NoRegister();
UPackage* Z_Construct_UPackage__Script_OrbitalDecay();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AOrbitalDecayCameraManager ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AOrbitalDecayCameraManager;
UClass* AOrbitalDecayCameraManager::GetPrivateStaticClass()
{
	using TClass = AOrbitalDecayCameraManager;
	if (!Z_Registration_Info_UClass_AOrbitalDecayCameraManager.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OrbitalDecayCameraManager"),
			Z_Registration_Info_UClass_AOrbitalDecayCameraManager.InnerSingleton,
			StaticRegisterNativesAOrbitalDecayCameraManager,
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
	return Z_Registration_Info_UClass_AOrbitalDecayCameraManager.InnerSingleton;
}
UClass* Z_Construct_UClass_AOrbitalDecayCameraManager_NoRegister()
{
	return AOrbitalDecayCameraManager::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AOrbitalDecayCameraManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Basic First Person camera manager.\n *  Limits min/max look pitch.\n */" },
#endif
		{ "IncludePath", "OrbitalDecayCameraManager.h" },
		{ "ModuleRelativePath", "OrbitalDecayCameraManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Basic First Person camera manager.\nLimits min/max look pitch." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class AOrbitalDecayCameraManager constinit property declarations ***************
// ********** End Class AOrbitalDecayCameraManager constinit property declarations *****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOrbitalDecayCameraManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AOrbitalDecayCameraManager_Statics
UObject* (*const Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerCameraManager,
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::ClassParams = {
	&AOrbitalDecayCameraManager::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::Class_MetaDataParams), Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::Class_MetaDataParams)
};
void AOrbitalDecayCameraManager::StaticRegisterNativesAOrbitalDecayCameraManager()
{
}
UClass* Z_Construct_UClass_AOrbitalDecayCameraManager()
{
	if (!Z_Registration_Info_UClass_AOrbitalDecayCameraManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AOrbitalDecayCameraManager.OuterSingleton, Z_Construct_UClass_AOrbitalDecayCameraManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AOrbitalDecayCameraManager.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AOrbitalDecayCameraManager);
AOrbitalDecayCameraManager::~AOrbitalDecayCameraManager() {}
// ********** End Class AOrbitalDecayCameraManager *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_VN_Documents_Work_Projects_Unreal_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayCameraManager_h__Script_OrbitalDecay_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AOrbitalDecayCameraManager, AOrbitalDecayCameraManager::StaticClass, TEXT("AOrbitalDecayCameraManager"), &Z_Registration_Info_UClass_AOrbitalDecayCameraManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AOrbitalDecayCameraManager), 2233456372U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_VN_Documents_Work_Projects_Unreal_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayCameraManager_h__Script_OrbitalDecay_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_VN_Documents_Work_Projects_Unreal_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayCameraManager_h__Script_OrbitalDecay_509491871{
	TEXT("/Script/OrbitalDecay"),
	Z_CompiledInDeferFile_FID_Users_VN_Documents_Work_Projects_Unreal_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayCameraManager_h__Script_OrbitalDecay_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_VN_Documents_Work_Projects_Unreal_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_OrbitalDecayCameraManager_h__Script_OrbitalDecay_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
