// Copyright 2022 Eidos-Montreal / Eidos-Sherbrooke

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "VolumeRuntimeModule.h"
#include "VdbCommon.h"
#include "Rendering/VdbRendering.h"
#include "Rendering/VdbResearchRendering.h"

#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FVolumeRuntimeModule"

void FVolumeRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("SparseVolumetrics"));

	FString PluginShaderDir = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/VdbVolume"), PluginShaderDir);

	RegisterVolumeTrackHandler(&VdbVolumeSequenceTrackHandler);
}

void FVolumeRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnregisterVolumeTrackHandler(&VdbVolumeSequenceTrackHandler);

	if (VdbRenderExtension) VdbRenderExtension->Release();
	if (VdbRenderResearchMgr) VdbRenderResearchMgr->Release();
}

FVolumeRuntimeModule::TRenderExtensionPtr FVolumeRuntimeModule::GetRenderExtension()
{
	static const FName ModuleName = "VolumeRuntime";
	auto& ModuleInterface = FModuleManager::LoadModuleChecked<FVolumeRuntimeModule>(ModuleName);
	return ModuleInterface.GetOrCreateRenderExtension();
}

FVolumeRuntimeModule::TRenderExtensionPtr FVolumeRuntimeModule::GetOrCreateRenderExtension()
{
	if (!VdbRenderExtension.IsValid())
	{
		VdbRenderExtension = FSceneViewExtensions::NewExtension<FVdbRendering>();
		VdbRenderExtension->Init();
	}
	return VdbRenderExtension;
}

FVolumeRuntimeModule::TRenderResearchPtr FVolumeRuntimeModule::GetRenderReseachMgr()
{
	static const FName ModuleName = "VolumeRuntime";
	auto& ModuleInterface = FModuleManager::LoadModuleChecked<FVolumeRuntimeModule>(ModuleName);
	return ModuleInterface.GetOrCreateRenderResearchMgr();
}

FVolumeRuntimeModule::TRenderResearchPtr FVolumeRuntimeModule::GetOrCreateRenderResearchMgr()
{
	if (!VdbRenderResearchMgr.IsValid())
	{
		VdbRenderResearchMgr = FSceneViewExtensions::NewExtension<FVdbResearchRendering>();
		VdbRenderResearchMgr->Init();
	}
	return VdbRenderResearchMgr;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVolumeRuntimeModule, VolumeRuntime)
