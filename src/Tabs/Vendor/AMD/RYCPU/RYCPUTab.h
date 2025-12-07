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
#include "Widgets/StapmLimitGBox.h"
#include "Widgets/FastLimitGBox.h"
#include "Widgets/SlowLimitGBox.h"
#include "Widgets/TctlTempGBox.h"
#include "Widgets/APUSlowLimitGBox.h"
#include "Widgets/APUSkinTempGBox.h"
#include "Widgets/DGPUSkinTempGBox.h"
#include "Widgets/VRMCurrentGBox.h"
#include "Widgets/VRMSocCurrentGBox.h"
#include "Widgets/VRMMaxCurrentGBox.h"
#include "Widgets/VRMSocMaxCurrentGBox.h"
#include "Widgets/StaticGfxClkGBox.h"
#include "Widgets/MinGfxClockGBox.h"
#include "Widgets/MaxGfxClockGBox.h"
#include "Widgets/PowerProfileGBox.h"
#include "Widgets/CurveOptimizerAllGBox.h"
#include "Widgets/CurveOptimizerCoreGBox.h"

namespace PWT::UI::AMD {
    class RYCPUTab final: public AddonTab {
        Q_OBJECT

    private:
        QPointer<FastLimitGBox> fastLimitGBox;
        QPointer<SlowLimitGBox> slowLimitGBox;
        QPointer<StapmLimitGBox> stapmLimitGBox;
        QPointer<TctlTempGBox> tctlTempGBox;
        QPointer<APUSlowLimitGBox> apuSlowLimitGBox;
        QPointer<APUSkinTempGBox> apuSkinTempGBox;
        QPointer<DGPUSkinTempGBox> dgpuSkinTempGBox;
        QPointer<VRMCurrentGBox> vrmCurrentGBox;
        QPointer<VRMSocCurrentGBox> vrmSocCurrentGBox;
        QPointer<VRMMaxCurrentGBox> vrmMaxCurrentGBox;
        QPointer<VRMSocMaxCurrentGBox> vrmSocMaxCurrentGBox;
        QPointer<CurveOptimizerAllGBox> curveOptimizerAllGBox;
        QPointer<CurveOptimizerCoreGBox> curveOptimizerCoreGBox;
        QPointer<StaticGfxClkGBox> staticGfxClkGBox;
        QPointer<MinGfxClockGBox> minGfxClockGBox;
        QPointer<MaxGfxClockGBox> maxGfxClockGBox;
        QPointer<PowerProfileGBox> powerProfileGBox;

    public:
        explicit RYCPUTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;

    private slots:
        void onFastLimitChanged(int v) const;
        void onSlowLimitChanged(int v) const;
    };
}
