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

namespace PWT::UI::AMD {
    CPUTab::CPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::AMD_HWPSTATE)) {
            pstateGBox = new PStateGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), pstateGBox);
        }

        if (features.contains(PWTS::Feature::AMD_CORE_PERFORMANCE_BOOST)) {
            corePerformanceBoostGBox = new CorePerformanceBoostGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), corePerformanceBoostGBox);
        }

        if (features.contains(PWTS::Feature::AMD_CPPC)) {
            cppcGBox = new CPPCGBox(features, packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), cppcGBox);
        }
    }

    void CPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!pstateGBox.isNull())
            pstateGBox->setData(packet);

        if (!corePerformanceBoostGBox.isNull())
            corePerformanceBoostGBox->setData(packet);

        if (!cppcGBox.isNull())
            cppcGBox->setData(packet);
    }

    void CPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!pstateGBox.isNull())
            pstateGBox->setDataForPacket(packet);

        if (!corePerformanceBoostGBox.isNull())
            corePerformanceBoostGBox->setDataForPacket(packet);

        if (!cppcGBox.isNull())
            cppcGBox->setDataForPacket(packet);
    }
}
