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
#pragma once

#include "../../../Include/AddonTab.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "Widgets/PowerLimitGBox.h"
#include "Widgets/VRCurrentConfigGBox.h"
#include "Widgets/PP1ConfigGBox.h"
#include "Widgets/TurboPowerCurrentGBox.h"
#include "Widgets/CPUPowerBalanceGBox.h"
#include "Widgets/GPUPowerBalanceGBox.h"
#include "Widgets/EnergyPerformanceBiasGBox.h"
#include "Widgets/TurboRatioLimitGBox.h"
#include "Widgets/MiscProcessorFeaturesGBox.h"
#include "Widgets/MiscPwrMgmt/MiscPowerManagementGBox.h"
#include "Widgets/PowerCtl/PowerCtlGBox.h"
#include "Widgets/FIVRControlGBox.h"
#include "Widgets/PKGCSTConfigControl/PKGCSTConfigControlGBox.h"

namespace PWT::UI::INTEL {
    class CPUTab final: public AddonTab {
    private:
        QPointer<PowerLimitGBox> powerLimitGBox;
        QPointer<VRCurrentConfigGBox> vrCurrentConfigGBox;
        QPointer<PP1ConfigGBox> pp1ConfigGBox;
        QPointer<TurboPowerCurrentGBox> turboPowerCurrentGBox;
        QPointer<CPUPowerBalanceGBox> cpuPowerBalanceGBox;
        QPointer<GPUPowerBalanceGBox> gpuPowerBalanceGBox;
        QPointer<EnergyPerformanceBiasGBox> energyPerformanceBiasGBox;
        QPointer<TurboRatioLimitGBox> turboRatioLimitGBox;
        QPointer<MiscProcessorFeaturesGBox> miscProcessorFeaturesGBox;
        QPointer<MiscPowerManagementGBox> miscPowerManagementGBox;
        QPointer<PowerCtlGBox> powerCtlGBox;
        QPointer<FIVRControlGBox> fivrControlGBox;
        QPointer<PKGCSTConfigControlGBox> pkgCstConfigControlGBox;

    public:
        explicit CPUTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;
    };
}
