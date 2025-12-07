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

#include "CorePerformanceBoostWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    CorePerformanceBoostWidget::CorePerformanceBoostWidget(const int threadCount): GridWidget(4) {
        for (int i=0; i<threadCount; ++i) {
            QVBoxLayout *cpuLyt = new QVBoxLayout();
            QComboBox *perfBoost = new QComboBox();

            perfBoost->addItems({"Enabled", "Disabled"});
            perfBoost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            perfBoost->setEnabled(false);

            cpuLyt->addWidget(new QLabel(QString("Core %1").arg(i)));
            cpuLyt->addWidget(perfBoost);

            comboList.append(perfBoost);

            setNextSlot();
            gridLyt->addLayout(cpuLyt, row, column);
        }
    }

    void CorePerformanceBoostWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(!comboList.isEmpty() && comboList.size() == packet.amdData->threadData.size());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (int i=0,l=comboList.size(); i<l; ++i) {
            comboList[i]->setEnabled(packet.amdData->threadData[i].corePerfBoost.isValid());

            if (comboList[i]->isEnabled())
                comboList[i]->setCurrentIndex(packet.amdData->threadData[i].corePerfBoost.getValue());
        }
    }

    void CorePerformanceBoostWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=comboList.size(); i<l; ++i) {
            if (!comboList[i]->isEnabled())
                continue;

            packet.amdData->threadData[i].corePerfBoost.setValue(comboList[i]->currentIndex(), true);
        }
    }

    void CorePerformanceBoostWidget::setCorePerformanceBoostAll(const int idx) const {
        for (QComboBox *combo: comboList) {
            if (combo->isEnabled())
                combo->setCurrentIndex(idx);
        }
    }
}
