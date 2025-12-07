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
#include <QLabel>

#include "CPUSMTGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    CPUSMTGBox::CPUSMTGBox(): QGroupBox("Simultaneous Multi-Threading / Hyper-Threading") {
        QHBoxLayout *lyt = new QHBoxLayout();

        smt = new QComboBox();

        smt->addItems({"on", "off", "forceoff"});
        smt->setMinimumWidth(100);

        lyt->addWidget(new QLabel("SMT state"));
        lyt->addStretch();
        lyt->addWidget(smt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void CPUSMTGBox::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::RWData<QString> state = packet.linuxData->smtState;

        setEnabled(state.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        smt->setCurrentIndex(smt->findText(state.getValue()));
    }

    void CPUSMTGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.linuxData->smtState.setValue(smt->currentText(), true);
    }
}
