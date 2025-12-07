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
#include "EngineClockSCLKGBox.h"

namespace PWT::UI::LNX::AMD {
    EngineClockSCLKGBox::EngineClockSCLKGBox(const int idx): QGroupBox("Engine Clock (OD SCLK)") {
        const auto unitVCB = [](QLabel *unitV, const int v) { unitV->setNum(v); };
        QVBoxLayout *lyt = new QVBoxLayout();

        index = idx;
        min = new SliderUnitWidget("MHz", unitVCB);
        max = new SliderUnitWidget("MHz", unitVCB);

        min->setStaticLabel("Minimum");
        min->setPageStep(100);
        max->setStaticLabel("Maximum");
        max->setPageStep(100);

        lyt->addWidget(min);
        lyt->addWidget(max);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
    }

    void EngineClockSCLKGBox::setData(const PWTS::DaemonPacket &packet) const {
        const PWTS::LNX::AMD::GPUODRanges odRanges = packet.linuxData->amdGpuData[index].odRanges.getValue();
        const PWTS::LNX::AMD::GPUDPMForcePerfLevel dpmLvl = packet.linuxData->amdGpuData[index].dpmForcePerfLevel.getValue();

        min->setRange(odRanges.sclk.min, odRanges.sclk.max);
        max->setRange(odRanges.sclk.min, odRanges.sclk.max);
        min->setValue(dpmLvl.sclk.min);
        max->setValue(dpmLvl.sclk.max);
    }

    PWTS::MinMax EngineClockSCLKGBox::getData() const {
        return {
            .min = min->getValue(),
            .max = max->getValue()
        };
    }
}
