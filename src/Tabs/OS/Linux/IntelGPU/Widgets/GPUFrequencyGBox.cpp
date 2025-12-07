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
#include "GPUFrequencyGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX::Intel {
    GPUFrequencyGBox::GPUFrequencyGBox(const int idx): QGroupBox("Frequency") {
        const auto sliderUnitValCB = [](QLabel *unitV, const int v) { unitV->setNum(v); };
        QVBoxLayout *lyt = new QVBoxLayout();

        index = idx;
        min = new SliderUnitWidget("MHz", sliderUnitValCB);
        max = new SliderUnitWidget("MHz", sliderUnitValCB);

        min->setPageStep(100);
        min->setStaticLabel("Minimum");
        max->setPageStep(100);
        max->setStaticLabel("Maximum");

        lyt->addWidget(min);
        lyt->addWidget(max);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void GPUFrequencyGBox::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.linuxData->intelGpuData.contains(index) &&
                             packet.linuxData->intelGpuData[index].rpsLimits.isValid() &&
                             packet.linuxData->intelGpuData[index].frequency.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::LNX::Intel::GPURPSLimits limits = packet.linuxData->intelGpuData[index].rpsLimits.getValue();
        const PWTS::MinMax freq = packet.linuxData->intelGpuData[index].frequency.getValue();

        min->setRange(limits.rpn, limits.rp0);
        max->setRange(limits.rpn, limits.rp0);
        min->setValue(freq.min);
        max->setValue(freq.max);
    }

    void GPUFrequencyGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->intelGpuData[index].frequency.setValue({
            .min = min->getValue(),
            .max = max->getValue()
        }, isEnabled());
    }
}
