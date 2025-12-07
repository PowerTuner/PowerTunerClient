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
#include "TurboRatioLimitWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    TurboRatioLimitWidget::TurboRatioLimitWidget(const int coreCount): GridWidget(2) {
        for (int i=0; i<coreCount && i<8; ++i) {
            SliderWidget *slider = new SliderWidget(QString("Maximum ratio limit for %1 cores").arg(i + 1));

            slider->setRange(1, 255);
            slider->setPageStep(10);

            setNextSlot();
            gridLyt->addWidget(slider, row, column);
            turboLimitList.append(slider);
        }
    }

    void TurboRatioLimitWidget::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::Intel::TurboRatioLimit data = packet.intelData->turboRatioLimit.getValue();
        const QList<int> ratioLimits {
            data.maxRatioLimit1C,
            data.maxRatioLimit2C,
            data.maxRatioLimit3C,
            data.maxRatioLimit4C,
            data.maxRatioLimit5C,
            data.maxRatioLimit6C,
            data.maxRatioLimit7C,
            data.maxRatioLimit8C
        };

        for (int i=0,l=turboLimitList.size(); i<l; ++i)
            turboLimitList[i]->setValue(ratioLimits[i]);
    }

    void TurboRatioLimitWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        QList<int> ratioLimits(8, 0);

        for (int i=0,l=turboLimitList.size(); i<l; ++i)
            ratioLimits[i] = turboLimitList[i]->getValue();

        packet.intelData->turboRatioLimit.setValue({
            .maxRatioLimit1C = ratioLimits[0],
            .maxRatioLimit2C = ratioLimits[1],
            .maxRatioLimit3C = ratioLimits[2],
            .maxRatioLimit4C = ratioLimits[3],
            .maxRatioLimit5C = ratioLimits[4],
            .maxRatioLimit6C = ratioLimits[5],
            .maxRatioLimit7C = ratioLimits[6],
            .maxRatioLimit8C = ratioLimits[7]
        }, true);
    }
}
