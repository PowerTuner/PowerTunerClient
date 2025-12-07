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

#include "CPUScalingGovernorWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    CPUScalingGovernorWidget::CPUScalingGovernorWidget(const int threadCount): GridWidget(4) {
        for (int i=0; i<threadCount; ++i) {
            QVBoxLayout *cpuGovComboLyt = new QVBoxLayout();
            QComboBox *combo = new QComboBox();

            combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            combo->setEnabled(false);

            cpuGovComboLyt->addWidget(new QLabel(QString("CPU %1").arg(i)));
            cpuGovComboLyt->addWidget(combo);

            setNextSlot();
            gridLyt->addLayout(cpuGovComboLyt, row, column);
            comboList.append(combo);

            QObject::connect(combo, &QComboBox::currentIndexChanged, this, &CPUScalingGovernorWidget::onComboChanged);
        }
    }

    void CPUScalingGovernorWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::LinuxThreadData> &thdData = packet.linuxData->threadData;

        setEnabled(!comboList.isEmpty() && comboList.size() == thdData.size());
        relatedCPUs.clear();

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (int i=0,l=thdData.size(); i<l; ++i) {
            const QSignalBlocker sblock {comboList[i]};
            const PWTS::ROData<PWTS::LNX::CPUScalingAvailableGovernors> &availGovs = thdData[i].scalingAvailableGovernors;
            const bool isCpuOn = thdData[i].cpuOnlineStatus.isValid() && thdData[i].cpuOnlineStatus.getValue() == 1;
            const bool isValid = isCpuOn && availGovs.isValid() && thdData[i].scalingGovernor.isValid();

            comboList[i]->clear();
            comboList[i]->setEnabled(isValid);

            if (!isValid)
                continue;

            comboList[i]->addItems(availGovs.getValue().availableGovernors);
            comboList[i]->setCurrentIndex(comboList[i]->findText(thdData[i].scalingGovernor.getValue()));
            relatedCPUs.insert(i, availGovs.getValue().relatedCPUs);
        }
    }

    void CPUScalingGovernorWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=comboList.size(); i<l; ++i) {
            if (comboList[i]->isEnabled())
                packet.linuxData->threadData[i].scalingGovernor.setValue(comboList[i]->currentText(), true);
        }
    }

    void CPUScalingGovernorWidget::setGovernorAll(const QString &governor) const {
        for (QComboBox *combo: comboList) {
            if (!combo->isEnabled())
                continue;

            const int idx = combo->findText(governor);
            const QSignalBlocker sblock {combo};

            if (idx != -1)
                combo->setCurrentIndex(idx);
        }
    }

    void CPUScalingGovernorWidget::onComboChanged(const int idx) const {
        const QComboBox *combo = qobject_cast<QComboBox *>(QObject::sender());

        if (combo == nullptr)
            return;

        const qsizetype cpuNo = comboList.indexOf(combo);

        for (const QString &cpuIdx: relatedCPUs[cpuNo]) {
            bool res;
            const int comboIdx = cpuIdx.toInt(&res);

            if (!res)
                continue;

            const QSignalBlocker sblock {comboList[comboIdx]};

            comboList[comboIdx]->setCurrentIndex(idx);
        }
    }
}
