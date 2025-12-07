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
#include <QVBoxLayout>
#include <QLabel>

#include "PowerDPMStateGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX::AMD {
    PowerDPMStateGBox::PowerDPMStateGBox(const int idx): QGroupBox("Power DPM State") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *stateLyt = new QHBoxLayout();

        index = idx;
        stateCombo = new QComboBox();

        stateCombo->addItems({"battery", "balanced", "performance"});

        stateLyt->addWidget(new QLabel("DPM State: "));
        stateLyt->addWidget(stateCombo);
        stateLyt->addStretch();
        lyt->addLayout(stateLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void PowerDPMStateGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.linuxData->amdGpuData.contains(index) && packet.linuxData->amdGpuData[index].powerDpmState.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        stateCombo->setCurrentIndex(stateCombo->findText(packet.linuxData->amdGpuData[index].powerDpmState.getValue()));
    }

    void PowerDPMStateGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->amdGpuData[index].powerDpmState.setValue(stateCombo->currentText(), isEnabled());
    }
}
