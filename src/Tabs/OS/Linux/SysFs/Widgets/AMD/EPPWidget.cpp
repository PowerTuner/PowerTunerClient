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

#include "EPPWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    EPPWidget::EPPWidget(const int threadCount): GridWidget(4) {
        for (int i=0; i<threadCount; ++i) {
            QVBoxLayout *eppComboLyt = new QVBoxLayout();
            QComboBox *combo = new QComboBox();

            combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            eppComboLyt->addWidget(new QLabel(QString("CPU %1").arg(i)));
            eppComboLyt->addWidget(combo);

            setNextSlot();
            gridLyt->addLayout(eppComboLyt, row, column);
            comboList.append(combo);
        }
    }

    void EPPWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::AMD::LinuxAMDThreadData> &thdData = packet.linuxAmdData->threadData;
        const QList<PWTS::LNX::LinuxThreadData> &lThdData = packet.linuxData->threadData;

        setEnabled(!comboList.isEmpty() && comboList.size() == thdData.size());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (int i=0,l=thdData.size(); i<l; ++i) {
            const bool isCpuOn = lThdData[i].cpuOnlineStatus.isValid() && lThdData[i].cpuOnlineStatus.getValue() == 1;
            const bool isValid = isCpuOn && thdData[i].epp.isValid() && thdData[i].pstateData.isValid();
            const QSignalBlocker sblock {comboList[i]};

            comboList[i]->clear();
            comboList[i]->setEnabled(isValid);

            if (!isValid)
                continue;

            comboList[i]->addItems(thdData[i].pstateData.getValue().eppAvailablePrefs);
            comboList[i]->setCurrentIndex(comboList[i]->findText(thdData[i].epp.getValue()));
        }
    }

    void EPPWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        int i = 0;

        for (const QComboBox *combo: comboList)
            packet.linuxAmdData->threadData[i++].epp.setValue(combo->currentText(), combo->isEnabled());
    }

    void EPPWidget::setEPPAll(const QString &epp) const {
        for (QComboBox *combo: comboList) {
            if (!combo->isEnabled())
                continue;

            const int idx = combo->findText(epp);
            const QSignalBlocker sblock {combo};

            if (idx != -1)
                combo->setCurrentIndex(idx);
        }
    }
}
