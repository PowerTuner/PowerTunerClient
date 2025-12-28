cmake_minimum_required(VERSION 3.23)

project(PowerTunerClient VERSION 1.1 DESCRIPTION "Official PowerTuner Desktop Client" LANGUAGES CXX)

option(WITH_INTEL "Enable support for Intel CPUs" ON)
option(WITH_AMD "Enable support for AMD CPUs" ON)

set(PROJECT_AUTHOR "kylon")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PRIV_DEFS "")

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

add_subdirectory(src/external/libPWTShared)
add_subdirectory(src/external/libPWTClientCommon)
add_subdirectory(src/external/libPWTClientService)

configure_file(src/Resources/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

qt_add_resources(QT_RESOURCE
	src/Resources/resources.qrc
    src/external/commonResources/commonResource.qrc
)

set(PROJECT_SOURCES
	src/Include/Daemon.h

	src/Classes/ClientSettings.cpp
	src/Classes/ClientSettings.h

	src/Tabs/Include/ScrollableTab.h
	src/Tabs/Include/GridWidget.h
	src/Tabs/Include/AddonTab.h
	src/Tabs/Include/FlowLayout.cpp
	src/Tabs/Include/FlowLayout.h
	src/Tabs/Widgets/BaseSliderWidget.h
	src/Tabs/Widgets/BaseSliderWidget.cpp
	src/Tabs/Widgets/SliderLevelsWidget.h
	src/Tabs/Widgets/SliderLevelsWidget.cpp
	src/Tabs/Widgets/SliderWidget.h
	src/Tabs/Widgets/SliderWidget.cpp
	src/Tabs/Widgets/SliderUnitWidget.h
	src/Tabs/Widgets/SliderUnitWidget.cpp
	src/Tabs/TabUtils.h
	src/Tabs/TabUtils.cpp

	src/Tabs/Home/HomeTab.cpp
	src/Tabs/Home/HomeTab.h
	src/Tabs/Home/Widgets/LabelInfoWidget.h
	src/Tabs/Home/Widgets/LabelInfoWidget.cpp
	src/Tabs/Home/Widgets/CPUInfoGBox.cpp
	src/Tabs/Home/Widgets/CPUInfoGBox.h
	src/Tabs/Home/Widgets/SystemInfoGBox.cpp
	src/Tabs/Home/Widgets/SystemInfoGBox.h
	src/Tabs/Home/Widgets/ClientSettingsWidget.h
	src/Tabs/Home/Widgets/ClientSettingsWidget.cpp
	src/Tabs/Home/Widgets/GPUInfoGBox.cpp
	src/Tabs/Home/Widgets/GPUInfoGBox.h
	src/Tabs/Home/Widgets/PowerTunerGBox.h
	src/Tabs/Home/Widgets/PowerTunerGBox.cpp

	src/Tabs/Daemons/DaemonsTab.cpp
	src/Tabs/Daemons/DaemonsTab.h
	src/Tabs/Daemons/Widgets/DaemonBoxContainer.h
	src/Tabs/Daemons/Widgets/DaemonBox.cpp
	src/Tabs/Daemons/Widgets/DaemonBoxContainer.cpp
	src/Tabs/Daemons/Widgets/DaemonOptions/AddDaemonTab.h
	src/Tabs/Daemons/Widgets/DaemonOptions/AddDaemonTab.cpp
	src/Tabs/Daemons/Widgets/DaemonOptions/DaemonSettingsTab.h
	src/Tabs/Daemons/Widgets/DaemonOptions/DaemonSettingsTab.cpp
	src/Tabs/Daemons/Widgets/DaemonOptions/ActionsTab.h
	src/Tabs/Daemons/Widgets/DaemonOptions/ActionsTab.cpp
	src/Tabs/Daemons/Widgets/DaemonOptions/DaemonOptions.cpp
	src/Tabs/Daemons/Widgets/DaemonOptions/DaemonOptions.h

	src/Tabs/Log/LogTab.h
	src/Tabs/Log/LogTab.cpp

	src/Tabs/OS/Linux/SysFs/SysFsTab.h
	src/Tabs/OS/Linux/SysFs/SysFsTab.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUSMTGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUSMTGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUFrequencyGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUFrequencyGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUIdleGovernorGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUIdleGovernorGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/Include/MiscPowerManagementWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/Include/MiscPowerManagementWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementGBox.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/Include/BlockQueueSchedulerWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/Include/BlockQueueSchedulerWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerGBox.h
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerGBox.cpp
	src/Tabs/OS/Linux/IntelGPU/IntelGPUTab.h
	src/Tabs/OS/Linux/IntelGPU/IntelGPUTab.cpp
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUFrequencyGBox.h
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUFrequencyGBox.cpp
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUBoostGBox.h
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUBoostGBox.cpp
	src/Tabs/OS/Linux/AMDGPU/AMDGPUTab.h
	src/Tabs/OS/Linux/AMDGPU/AMDGPUTab.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/DPMForcePerformanceLevelGBox.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/DPMForcePerformanceLevelGBox.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/EngineClockSCLKGBox.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/EngineClockSCLKGBox.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/PowerDPMStateGBox.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/PowerDPMStateGBox.cpp

	src/Tabs/OS/Windows/PowerScheme/PowerSchemeTab.h
	src/Tabs/OS/Windows/PowerScheme/PowerSchemeTab.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/MinMaxSliderGBox.h
	src/Tabs/OS/Windows/PowerScheme/Include/MinMaxSliderGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/OptionsGBox.h
	src/Tabs/OS/Windows/PowerScheme/Include/OptionsGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/PowerSettingGBox.h
	src/Tabs/OS/Windows/PowerScheme/Include/PowerSettingGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/MiscGlobalOptionsGBox.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/MiscGlobalOptionsGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/CopySchemeSettingsGBox.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/CopySchemeSettingsGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/DuplicateSchemeGBox.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/DuplicateSchemeGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/SchemeActionsGBox.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/SchemeActionsGBox.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemesOptionsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemesOptionsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSelectInput.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSelectInput.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSettingsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSettingsWidget.cpp

	src/Tabs/FanControl/FanControlTab.h
	src/Tabs/FanControl/FanControlTab.cpp
	src/Tabs/FanControl/Widgets/FanCurveWidget.h
	src/Tabs/FanControl/Widgets/FanCurveWidget.cpp
	src/Tabs/FanControl/Widgets/FanControlGBox.h
	src/Tabs/FanControl/Widgets/FanControlGBox.cpp

	src/Widgets/ProfileActions.h
	src/Widgets/ProfileActions.cpp
	src/Widgets/UIActions.h
	src/Widgets/UIActions.cpp
	src/Widgets/StatusBar.h
	src/Widgets/StatusBar.cpp
	src/Widgets/TrayIcon.h
	src/Widgets/TrayIcon.cpp

	src/main.cpp
	src/mainwindow.cpp
	src/mainwindow.h
	src/mainwindow.ui
)

if (WITH_INTEL)
	message(STATUS "${PROJECT_NAME}: Intel CPU support is enabled")
	list(APPEND PRIV_DEFS WITH_INTEL)

	list(APPEND PROJECT_SOURCES
		src/Tabs/Vendor/Intel/CPU/CPUTab.h
		src/Tabs/Vendor/Intel/CPU/CPUTab.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerLimitGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerLimitGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/VRCurrentConfigGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/VRCurrentConfigGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PP1ConfigGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PP1ConfigGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboPowerCurrentGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboPowerCurrentGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/CPUPowerBalanceGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/CPUPowerBalanceGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/GPUPowerBalanceGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/GPUPowerBalanceGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/EnergyPerformanceBiasGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/EnergyPerformanceBiasGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscProcessorFeaturesGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscProcessorFeaturesGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPwrMgmt/MiscPowerManagementGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPwrMgmt/MiscPowerManagementNHLMGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPwrMgmt/MiscPowerManagementNHLMGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlNHLMGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlNHLMGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlSBGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlSBGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlCU1GBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlCU1GBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlGBox.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlGBox.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlSBWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlSBWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlCU1Widget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlCU1Widget.cpp

		src/Tabs/Vendor/Intel/MCHBAR/MCHBARTab.h
		src/Tabs/Vendor/Intel/MCHBAR/MCHBARTab.cpp
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitGBox.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitIVBGBox.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitIVBGBox.cpp
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitTGLGBox.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitTGLGBox.cpp

		src/Tabs/Vendor/Intel/HWP/HWPTab.h
		src/Tabs/Vendor/Intel/HWP/HWPTab.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestWidget.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestPkgGBox.h
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestPkgGBox.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestGBox.h
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestGBox.cpp
	)
endif ()

if (WITH_AMD)
	message(STATUS "${PROJECT_NAME}: AMD CPU support is enabled")
	list(APPEND PRIV_DEFS WITH_AMD)

	list(APPEND PROJECT_SOURCES
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/AMDPStateGBox.h
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/AMDPStateGBox.cpp
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/EPPWidget.h
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/EPPWidget.cpp

		src/Tabs/Vendor/AMD/RYCPU/Include/RADJSliderGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Include/RADJSliderGBox.cpp
		src/Tabs/Vendor/AMD/RYCPU/RYCPUTab.h
		src/Tabs/Vendor/AMD/RYCPU/RYCPUTab.cpp
		src/Tabs/Vendor/AMD/RYCPU/Widgets/StapmLimitGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/FastLimitGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/SlowLimitGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/TctlTempGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/APUSlowLimitGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/APUSkinTempGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/DGPUSkinTempGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMCurrentGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMMaxCurrentGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMSocCurrentGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMSocMaxCurrentGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/StaticGfxClkGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/MinGfxClockGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/MaxGfxClockGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/PowerProfileGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/PowerProfileGBox.cpp
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerAllGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerCoreGBox.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerCoreGBox.cpp

		src/Tabs/Vendor/AMD/CPU/CPUTab.h
		src/Tabs/Vendor/AMD/CPU/CPUTab.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostGBox.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostGBox.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCGBox.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCGBox.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateGBox.h
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateGBox.cpp
	)
endif ()

if (WIN32)
	add_subdirectory(src/external/libPWTWin32)
	configure_file(src/Resources/Windows/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

	list(APPEND PROJECT_SOURCES
		win.rc
	)
endif ()

qt_add_executable(${PROJECT_NAME} ${QT_RESOURCE} ${PROJECT_SOURCES})

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
	message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PRIV_DEFS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::Network PWT::ClientCommon PWT::Shared PWT::ClientService)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "kylon.pwtclient.com"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
	BUNDLE  DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
	TARGET ${PROJECT_NAME}
	OUTPUT_SCRIPT deploy_script
	NO_TRANSLATIONS
	NO_COMPILER_RUNTIME
	NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
