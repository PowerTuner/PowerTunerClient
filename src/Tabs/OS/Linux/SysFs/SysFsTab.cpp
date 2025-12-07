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
#include "SysFsTab.h"

namespace PWT::UI::LNX {
    SysFsTab::SysFsTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        cpuVendor = packet.cpuInfo.vendor;

        if (features.contains(PWTS::Feature::CPU_PARK_SYSFS)) {
            cpuParkGBox = new CPUParkGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuParkGBox);
        }

        if (features.contains(PWTS::Feature::CPU_SMT_SYSFS)) {
            cpuSmtGBox = new CPUSMTGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuSmtGBox);
        }

        if (features.contains(PWTS::Feature::CPUFREQ_SYSFS)) {
            cpuScalingGovernorGBox = new CPUScalingGovernorGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuScalingGovernorGBox);

            cpuFrequencyGBox = new CPUFrequencyGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuFrequencyGBox);
        }

        if (features.contains(PWTS::Feature::CPUIDLE_GOV_SYSFS)) {
            cpuIdleGovernorGBox = new CPUIdleGovernorGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuIdleGovernorGBox);
        }

#ifdef WITH_AMD
        if (features.contains(PWTS::Feature::AMD_PSTATE_SYSFS)) {
            amdpstateGBox = new AMDPStateGBox(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), amdpstateGBox);
        }
#endif

        blockQueueSchedulerGBox = new BlockQueueSchedulerGBox();
        scrollWidgLyt->insertWidget(nextInsertIdx(), blockQueueSchedulerGBox);

        miscPowerManagementGBox = new MiscPowerManagementGBox();
        scrollWidgLyt->insertWidget(nextInsertIdx(), miscPowerManagementGBox);
    }

    void SysFsTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!cpuParkGBox.isNull())
            cpuParkGBox->setData(packet);

        if (!cpuSmtGBox.isNull())
            cpuSmtGBox->setData(packet);

        if (!cpuScalingGovernorGBox.isNull())
            cpuScalingGovernorGBox->setData(packet);

        if (!cpuFrequencyGBox.isNull())
            cpuFrequencyGBox->setData(packet);

        if (!cpuIdleGovernorGBox.isNull())
            cpuIdleGovernorGBox->setData(packet);

        blockQueueSchedulerGBox->setData(packet);
        miscPowerManagementGBox->setData(packet);

        switch (cpuVendor) {
#ifdef WITH_AMD
            case PWTS::CPUVendor::AMD:
                refreshTabAMD(packet);
                break;
#endif
            default:
                break;
        }
    }

    void SysFsTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!cpuParkGBox.isNull())
            cpuParkGBox->setDataForPacket(packet);

        if (!cpuSmtGBox.isNull())
            cpuSmtGBox->setDataForPacket(packet);

        if (!cpuScalingGovernorGBox.isNull())
            cpuScalingGovernorGBox->setDataForPacket(packet);

        if (!cpuFrequencyGBox.isNull())
            cpuFrequencyGBox->setDataForPacket(packet);

        if (!cpuIdleGovernorGBox.isNull())
            cpuIdleGovernorGBox->setDataForPacket(packet);

        blockQueueSchedulerGBox->setDataForPacket(packet);
        miscPowerManagementGBox->setDataForPacket(packet);

        switch (cpuVendor) {
#ifdef WITH_AMD
            case PWTS::CPUVendor::AMD:
                setDataForPacketAMD(packet);
                break;
#endif
            default:
                break;
        }
    }

#ifdef WITH_AMD
    void SysFsTab::refreshTabAMD(const PWTS::DaemonPacket &packet) const {
        if (!amdpstateGBox.isNull())
            amdpstateGBox->setData(packet);
    }

    void SysFsTab::setDataForPacketAMD(const PWTS::ClientPacket &packet) const {
        if (!amdpstateGBox.isNull())
            amdpstateGBox->setDataForPacket(packet);
    }
#endif
}
