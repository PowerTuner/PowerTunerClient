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
#include "Widgets/GPUFrequencyGBox.h"
#include "Widgets/GPUBoostGBox.h"

namespace PWT::UI::LNX::Intel {
    class IntelGPUTab final: public AddonTab {
    private:
        QPointer<GPUFrequencyGBox> gpuFrequencyGBox;
        QPointer<GPUBoostGBox> gpuBoostGBox;

    public:
        IntelGPUTab(const PWTS::DeviceInfoPacket &packet, int index);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;
    };
}
