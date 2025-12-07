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
#include "Widgets/CPUParkGBox.h"
#include "Widgets/CPUSMTGBox.h"
#include "Widgets/CPUScalingGovernorGBox.h"
#include "Widgets/CPUFrequencyGBox.h"
#include "Widgets/CPUIdleGovernorGBox.h"
#include "Widgets/BlockQueueSchedulerGBox.h"
#include "Widgets/MiscPowerManagementGBox.h"
#ifdef WITH_AMD
#include "Widgets/AMD/AMDPStateGBox.h"
#endif

namespace PWT::UI::LNX {
    class SysFsTab final: public AddonTab {
    private:
        PWTS::CPUVendor cpuVendor = PWTS::CPUVendor::Unknown;
        BlockQueueSchedulerGBox *blockQueueSchedulerGBox = nullptr;
        MiscPowerManagementGBox *miscPowerManagementGBox = nullptr;
        QPointer<CPUParkGBox> cpuParkGBox;
        QPointer<CPUSMTGBox> cpuSmtGBox;
        QPointer<CPUScalingGovernorGBox> cpuScalingGovernorGBox;
        QPointer<CPUFrequencyGBox> cpuFrequencyGBox;
        QPointer<CPUIdleGovernorGBox> cpuIdleGovernorGBox;

#ifdef WITH_AMD
        QPointer<AMDPStateGBox> amdpstateGBox;

        void refreshTabAMD(const PWTS::DaemonPacket &packet) const;
        void setDataForPacketAMD(const PWTS::ClientPacket &packet) const;
#endif

    public:
        explicit SysFsTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;
    };
}
