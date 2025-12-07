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

#include "../Include/SliderLimitGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    class VRMCurrentGBox final: public SliderLimitGBox {
    public:
        VRMCurrentGBox(): SliderLimitGBox("Thermal Design Current Limit (TDC VDD)",
                                            "VRM Current Limit",
                                            "Amps",
                                            [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); }) {}

        void setData(const PWTS::DaemonPacket &packet) override {
            setEnabled(packet.amdData->vrmCurrent.isValid());

            if (!isEnabled()) {
                UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
                return;
            }

            const int val = packet.amdData->vrmCurrent.getValue();

            if (val >= 0)
                slider->setValue(val);
        }

        void setDataForPacket(const PWTS::ClientPacket &packet) const override {
            if (!isEnabled())
                return;

            packet.amdData->vrmCurrent.setValue(slider->getValue(), true);
        }
    };
}
