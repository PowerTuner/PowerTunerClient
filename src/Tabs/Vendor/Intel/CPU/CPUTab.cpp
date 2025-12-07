/*
 * This file is part of PowerTunerClient.
 * Copyright (C) 2025 kylon
 *
 * PowerTunerClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PowerTunerClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CPUTab.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "Widgets/PowerCtl/PowerCtlNHLMGBox.h"
#include "Widgets/PowerCtl/PowerCtlSBGBox.h"
#include "Widgets/PowerCtl/PowerCtlCU1GBox.h"
#include "Widgets/MiscPwrMgmt/MiscPowerManagementNHLMGBox.h"

namespace PWT::UI::INTEL {
    CPUTab::CPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::INTEL_PKG_POWER_LIMIT)) {
            powerLimitGBox = new PowerLimitGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerLimitGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_VR_CURRENT_CFG)) {
            vrCurrentConfigGBox = new VRCurrentConfigGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrCurrentConfigGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_PP1_CURRENT_CFG)) {
            pp1ConfigGBox = new PP1ConfigGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), pp1ConfigGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_TURBO_POWER_CURRENT_LIMIT_RW)) {
            turboPowerCurrentGBox = new TurboPowerCurrentGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), turboPowerCurrentGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_CPU_POWER_BALANCE)) {
            cpuPowerBalanceGBox = new CPUPowerBalanceGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuPowerBalanceGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_GPU_POWER_BALANCE)) {
            gpuPowerBalanceGBox = new GPUPowerBalanceGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuPowerBalanceGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_ENERGY_PERF_BIAS) && !features.contains(PWTS::Feature::INTEL_HWP_EPP)) {
            energyPerformanceBiasGBox = new EnergyPerformanceBiasGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), energyPerformanceBiasGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_TURBO_RATIO_LIMIT_RW)) {
            turboRatioLimitGBox = new TurboRatioLimitGBox(packet.cpuInfo.numCores);
            scrollWidgLyt->insertWidget(nextInsertIdx(), turboRatioLimitGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_IA32_MISC_ENABLE_GROUP)) {
            miscProcessorFeaturesGBox = new MiscProcessorFeaturesGBox(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), miscProcessorFeaturesGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_MISC_PWR_MGMT)) {
            if (features.contains(PWTS::Feature::INTEL_MISC_PWR_MGMT_NHLM))
                miscPowerManagementGBox = new MiscPowerManagementGBoxNHLM();

            scrollWidgLyt->insertWidget(nextInsertIdx(), miscPowerManagementGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_POWER_CTL)) {
            if (features.contains(PWTS::Feature::INTEL_POWER_CTL_NHLM))
                powerCtlGBox = new PowerCtlNHLMGBox();
            else if (features.contains(PWTS::Feature::INTEL_POWER_CTL_SB))
                powerCtlGBox = new PowerCtlSBGBox();
            else if (features.contains(PWTS::Feature::INTEL_POWER_CTL_CU1))
                powerCtlGBox = new PowerCtlCU1GBox();

            scrollWidgLyt->insertWidget(nextInsertIdx(), powerCtlGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_UNDERVOLT_GROUP)) {
            fivrControlGBox = new FIVRControlGBox(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), fivrControlGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL)) {
            pkgCstConfigControlGBox = new PKGCSTConfigControlGBox(packet.cpuInfo.numCores, features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), pkgCstConfigControlGBox);
        }
    }

    void CPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<InputRanges> inputRanges = InputRanges::getInstance();

        if (!powerLimitGBox.isNull()) {
            powerLimitGBox->setRanges(inputRanges->getIntelPl());
            powerLimitGBox->setData(packet);
        }

        if (!vrCurrentConfigGBox.isNull()) {
            vrCurrentConfigGBox->setRange(inputRanges->getIntelPl4());
            vrCurrentConfigGBox->setData(packet);
        }

        if (!pp1ConfigGBox.isNull()) {
            pp1ConfigGBox->setRange(inputRanges->getIntelPP1());
            pp1ConfigGBox->setData(packet);
        }

        if (!turboPowerCurrentGBox.isNull()) {
            turboPowerCurrentGBox->setRanges(inputRanges->getIntelTurboPwrCurrentTDP(), inputRanges->getIntelTurboPwrCurrentTDC());
            turboPowerCurrentGBox->setData(packet);
        }

        if (!cpuPowerBalanceGBox.isNull())
            cpuPowerBalanceGBox->setData(packet);

        if (!gpuPowerBalanceGBox.isNull())
            gpuPowerBalanceGBox->setData(packet);

        if (!energyPerformanceBiasGBox.isNull())
            energyPerformanceBiasGBox->setData(packet);

        if (!turboRatioLimitGBox.isNull())
            turboRatioLimitGBox->setData(packet);

        if (!miscProcessorFeaturesGBox.isNull())
            miscProcessorFeaturesGBox->setData(packet);

        if (!miscPowerManagementGBox.isNull())
            miscPowerManagementGBox->setData(packet);

        if (!powerCtlGBox.isNull())
            powerCtlGBox->setData(packet);

        if (!fivrControlGBox.isNull()) {
            fivrControlGBox->setRanges(inputRanges->getIntelFIVR());
            fivrControlGBox->setData(packet);
        }

        if (!pkgCstConfigControlGBox.isNull())
            pkgCstConfigControlGBox->setData(packet);
    }

    void CPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!powerLimitGBox.isNull())
            powerLimitGBox->setDataForPacket(packet);

        if (!vrCurrentConfigGBox.isNull())
            vrCurrentConfigGBox->setDataForPacket(packet);

        if (!pp1ConfigGBox.isNull())
            pp1ConfigGBox->setDataForPacket(packet);

        if (!turboPowerCurrentGBox.isNull())
            turboPowerCurrentGBox->setDataForPacket(packet);

        if (!cpuPowerBalanceGBox.isNull())
            cpuPowerBalanceGBox->setDataForPacket(packet);

        if (!gpuPowerBalanceGBox.isNull())
            gpuPowerBalanceGBox->setDataForPacket(packet);

        if (!energyPerformanceBiasGBox.isNull())
            energyPerformanceBiasGBox->setDataForPacket(packet);

        if (!turboRatioLimitGBox.isNull())
            turboRatioLimitGBox->setDataForPacket(packet);

        if (!miscProcessorFeaturesGBox.isNull())
            miscProcessorFeaturesGBox->setDataForPacket(packet);

        if (!miscPowerManagementGBox.isNull())
            miscPowerManagementGBox->setDataForPacket(packet);

        if (!powerCtlGBox.isNull())
            powerCtlGBox->setDataForPacket(packet);

        if (!fivrControlGBox.isNull())
            fivrControlGBox->setDataForPacket(packet);

        if (!pkgCstConfigControlGBox.isNull())
            pkgCstConfigControlGBox->setDataForPacket(packet);
    }
}
