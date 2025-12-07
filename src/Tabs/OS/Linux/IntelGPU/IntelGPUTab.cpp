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
#include "IntelGPUTab.h"

namespace PWT::UI::LNX::Intel {
    IntelGPUTab::IntelGPUTab(const PWTS::DeviceInfoPacket &packet, const int index) {
        const QSet<PWTS::Feature> &features = packet.features.gpus.value(index).second;

        if (features.contains(PWTS::Feature::INTEL_GPU_RPS_FREQ_SYSFS)) {
            gpuFrequencyGBox = new GPUFrequencyGBox(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuFrequencyGBox);
        }

        if (features.contains(PWTS::Feature::INTEL_GPU_BOOST_SYSFS)) {
            gpuBoostGBox = new GPUBoostGBox(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuBoostGBox);
        }
    }

    void IntelGPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!gpuFrequencyGBox.isNull())
            gpuFrequencyGBox->setData(packet);

        if (!gpuBoostGBox.isNull())
            gpuBoostGBox->setData(packet);
    }

    void IntelGPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!gpuFrequencyGBox.isNull())
            gpuFrequencyGBox->setDataForPacket(packet);

        if (!gpuBoostGBox.isNull())
            gpuBoostGBox->setDataForPacket(packet);
    }
}
