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

#include "../Include/RADJSliderGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    class StaticGfxClkGBox final: public RADJSliderGBox {
    public:
        StaticGfxClkGBox(): RADJSliderGBox("Force static GPU clock",
                                            "Forced Clock Speed",
                                            "MHz",
                                            [](QLabel *unitV, const int v) { unitV->setNum(v); },
                                            false) {
            slider->setPageStep(100);
        }

        void setData(const PWTS::DaemonPacket &packet) override {
            setEnabled(packet.amdData->staticGfxClock.isValid());

            if (!isEnabled()) {
                UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
                return;
            }

            if (!enableChk.isNull()) {
                const QSignalBlocker sblock {enableChk};

                enableChk->setChecked(packet.hasProfileData ? !packet.amdData->staticGfxClock.isIgnored() : enableChecked);
            }

            slider->setValue(packet.amdData->staticGfxClock.getValue());
        }

        void setDataForPacket(const PWTS::ClientPacket &packet) const override {
            if (!isEnabled())
                return;

            packet.amdData->staticGfxClock.setValue(slider->getValue(), true, !enableChk.isNull() && !enableChk->isChecked());
        }
    };
}
