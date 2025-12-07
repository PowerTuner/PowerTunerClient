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
#include "AMDGPUTab.h"

namespace PWT::UI::LNX::AMD {
    AMDGPUTab::AMDGPUTab(const PWTS::DeviceInfoPacket &packet, const int index) {
        const QSet<PWTS::Feature> &features = packet.features.gpus.value(index).second;

        if (features.contains(PWTS::Feature::AMD_GPU_DPM_FORCE_PERF_LEVEL_SYSFS)) {
            dpmForcePerformanceLevelGBox = new DPMForcePerformanceLevelGBox(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), dpmForcePerformanceLevelGBox);
        }

        if (features.contains(PWTS::Feature::AMD_GPU_POWER_DPM_STATE_SYSFS)) {
            powerDpmStateGBox = new PowerDPMStateGBox(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerDpmStateGBox);
        }
    }

    void AMDGPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!dpmForcePerformanceLevelGBox.isNull())
            dpmForcePerformanceLevelGBox->setData(packet);

        if (!powerDpmStateGBox.isNull())
            powerDpmStateGBox->setData(packet);
    }

    void AMDGPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!dpmForcePerformanceLevelGBox.isNull())
            dpmForcePerformanceLevelGBox->setDataForPacket(packet);

        if (!powerDpmStateGBox.isNull())
            powerDpmStateGBox->setDataForPacket(packet);
    }
}
