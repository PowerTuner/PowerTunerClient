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
#include "GPUPowerBalanceGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    GPUPowerBalanceGBox::GPUPowerBalanceGBox(): QGroupBox("GPU Power Balance") {
        QHBoxLayout *lyt = new QHBoxLayout();

        gpu = new SliderLevelsWidget("Low", "High", "GPU Priority");

        gpu->setRange(0, 31);
        gpu->setPageStep(5);

        lyt->addWidget(gpu);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void GPUPowerBalanceGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->pp1Priority.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        gpu->setValue(packet.intelData->pp1Priority.getValue());
    }

    void GPUPowerBalanceGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->pp1Priority.setValue(gpu->getValue(), true);
    }
}
