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
#include <QHBoxLayout>

#include "PowerProfileGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    PowerProfileGBox::PowerProfileGBox(): QGroupBox("Power Profile") {
        QHBoxLayout *lyt = new QHBoxLayout();

        powerSave = new QRadioButton("power saving");
        maxPerformance = new QRadioButton("max performance");
        dontSet = new QRadioButton("don't set");

        powerSave->setToolTip("Hidden option to improve power efficiency (set when AC unplugged): behavior depends on CPU, device and manufacturer");
        maxPerformance->setToolTip("Hidden option to improve performance (set when AC plugged in): behavior depends on CPU, device and manufacturer");

        lyt->addWidget(powerSave);
        lyt->addWidget(maxPerformance);
        lyt->addWidget(dontSet);
        lyt->addStretch();

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void PowerProfileGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.amdData->powerProfile.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const int val = packet.amdData->powerProfile.getValue();

        if (val == 0)
            powerSave->setChecked(true);
        else if (val == 1)
            maxPerformance->setChecked(true);
        else
            dontSet->setChecked(true);
    }

    void PowerProfileGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        if (powerSave->isChecked())
            packet.amdData->powerProfile.setValue(0, true);
        else if (maxPerformance->isChecked())
            packet.amdData->powerProfile.setValue(1, true);
        else
            packet.amdData->powerProfile.setValue(-1, true);
    }
}
