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

#include "PStateWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    PStateWidget::PStateWidget(const int threadCount): GridWidget(8) {
        for (int i=0; i<threadCount; ++i) {
            QVBoxLayout *cpuLyt = new QVBoxLayout();
            QComboBox *pstate = new QComboBox();

            pstate->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

            cpuLyt->addWidget(new QLabel(QString("CPU %1").arg(i)));
            cpuLyt->addWidget(pstate);

            pstateList.append(pstate);

            setNextSlot();
            gridLyt->addLayout(cpuLyt, row, column);
        }
    }

    void PStateWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(!pstateList.isEmpty() && pstateList.size() == packet.amdData->threadData.size());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (int i=0,l=packet.amdData->threadData.size(); i<l; ++i) {
            pstateList[i]->setEnabled(packet.amdData->pstateCurrentLimit.isValid() && packet.amdData->threadData[i].pstateCmd.isValid());
            pstateList[i]->clear();

            if (!pstateList[i]->isEnabled())
                continue;

            const PWTS::AMD::PStateCurrentLimit limits = packet.amdData->pstateCurrentLimit.getValue();

            for (int p=limits.curPStateLimit; p<=limits.pstateMaxValue; ++p)
                pstateList[i]->addItem(QString::number(p));

            pstateList[i]->setCurrentIndex(packet.amdData->threadData[i].pstateCmd.getValue());
        }
    }

    void PStateWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=pstateList.size(); i<l; ++i)
            packet.amdData->threadData[i].pstateCmd.setValue(pstateList[i]->currentIndex(), pstateList[i]->isEnabled());
    }

    void PStateWidget::setAllPStates(const int pstate) const {
        for (QComboBox *combo: pstateList) {
            if (combo->isEnabled())
                combo->setCurrentIndex(pstate);
        }
    }
}
