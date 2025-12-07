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
#include <QLabel>

#include "AMDPStateGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    AMDPStateGBox::AMDPStateGBox(const int threadCount): QGroupBox("AMD PState Scaling Driver") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *statusLyt = new QHBoxLayout();
        QHBoxLayout *eppAllCPULyt = new QHBoxLayout();

        eppWidget = new EPPWidget(threadCount);
        pstateStatus = new QComboBox();
        eppAllCombo = new QComboBox();

        pstateStatus->addItems({"", "disable", "active", "passive", "guided"});
        eppAllCombo->addItem("");
        eppAllCombo->setMinimumSize(150, 0);

        statusLyt->addWidget(pstateStatus);
        statusLyt->addStretch();
        eppAllCPULyt->addStretch();
        eppAllCPULyt->addWidget(new QLabel("Energy Performance Preference (All CPUs)"));
        eppAllCPULyt->addWidget(eppAllCombo);
        lyt->addLayout(statusLyt);
        lyt->addLayout(eppAllCPULyt);
        lyt->addWidget(eppWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(pstateStatus, &QComboBox::currentTextChanged, this, &AMDPStateGBox::onStatusChanged);
        QObject::connect(eppAllCombo, &QComboBox::currentTextChanged, this, &AMDPStateGBox::onEPPAllComboChanged);
    }

    void AMDPStateGBox::setEPPAllCombo(const QList<PWTS::LNX::AMD::LinuxAMDThreadData> &thdData) const {
        QList<QString> allEppList;

        for (const PWTS::LNX::AMD::LinuxAMDThreadData &thd: thdData) {
            if (!thd.pstateData.isValid())
                continue;

            const QList<QString> availableEPPPrefs = thd.pstateData.getValue().eppAvailablePrefs;

            for (const QString &epp: availableEPPPrefs) {
                if (!allEppList.contains(epp))
                    allEppList.append(epp);
            }
        }

        eppAllCombo->clear();
        eppAllCombo->addItems(allEppList);
    }

    void AMDPStateGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.linuxAmdData->pstateStatus.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        pstateStatus->setCurrentIndex(pstateStatus->findText(packet.linuxAmdData->pstateStatus.getValue()));
        setEPPAllCombo(packet.linuxAmdData->threadData);
        eppWidget->setData(packet);
    }

    void AMDPStateGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxAmdData->pstateStatus.setValue(pstateStatus->currentText(), isEnabled());
        eppWidget->setDataForPacket(packet);
    }

    void AMDPStateGBox::onStatusChanged(const QString &text) const {
        const bool hasEPP = text == "active";

        eppAllCombo->setEnabled(hasEPP);
        eppWidget->setEnabled(hasEPP);
    }

    void AMDPStateGBox::onEPPAllComboChanged(const QString &text) const {
        if (text.isEmpty())
            return;

        const QSignalBlocker sblock {eppAllCombo};

        eppAllCombo->setEnabled(false);
        eppWidget->setEPPAll(text);
        eppAllCombo->setCurrentIndex(-1);
        eppAllCombo->setEnabled(true);
    }
}
