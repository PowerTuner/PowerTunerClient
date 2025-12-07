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
#include "Widgets/PStateGBox.h"
#include "Widgets/CorePerformanceBoostGBox.h"
#include "Widgets/CPPCGBox.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"

namespace PWT::UI::AMD {
    class CPUTab final: public AddonTab {
    private:
        QPointer<PStateGBox> pstateGBox;
        QPointer<CorePerformanceBoostGBox> corePerformanceBoostGBox;
        QPointer<CPPCGBox> cppcGBox;

    public:
        explicit CPUTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;
    };
}
