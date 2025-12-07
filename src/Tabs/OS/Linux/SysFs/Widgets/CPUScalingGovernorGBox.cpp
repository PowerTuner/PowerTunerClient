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

#include "CPUScalingGovernorGBox.h"

namespace PWT::UI::LNX {
    CPUScalingGovernorGBox::CPUScalingGovernorGBox(const int threadCount): QGroupBox("CPU Scaling Governor") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *scalingAllCPULyt = new QHBoxLayout();

        cpuScalingGovernorWidget = new CPUScalingGovernorWidget(threadCount);
        scalingGovAllCombo = new QComboBox();

        scalingGovAllCombo->setMinimumSize(150, 0);

        scalingAllCPULyt->addStretch();
        scalingAllCPULyt->addWidget(new QLabel("Scaling governor (All CPUs)"));
        scalingAllCPULyt->addWidget(scalingGovAllCombo);
        lyt->addLayout(scalingAllCPULyt);
        lyt->addWidget(cpuScalingGovernorWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(scalingGovAllCombo, &QComboBox::currentTextChanged, this, &CPUScalingGovernorGBox::onScalingGovAllComboChanged);
    }

    void CPUScalingGovernorGBox::setAllScalingGovernorsCombo(const QSharedPointer<PWTS::LNX::LinuxData> &linuxData) const {
        const QSignalBlocker sblock {scalingGovAllCombo};
        QList<QString> governorList;

        for (const PWTS::LNX::LinuxThreadData &thdData: linuxData->threadData) {
            if (!thdData.scalingAvailableGovernors.isValid())
                continue;

            for (const QString &gov: thdData.scalingAvailableGovernors.getValue().availableGovernors) {
                if (!governorList.contains(gov))
                    governorList.append(gov);
            }
        }

        scalingGovAllCombo->clear();
        scalingGovAllCombo->addItem("");
        scalingGovAllCombo->addItems(governorList);
    }

    void CPUScalingGovernorGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(true);
        cpuScalingGovernorWidget->setData(packet);
        setAllScalingGovernorsCombo(packet.linuxData);
    }

    void CPUScalingGovernorGBox::onScalingGovAllComboChanged(const QString &text) const {
        if (text.isEmpty())
            return;

        const QSignalBlocker sblock {scalingGovAllCombo};

        scalingGovAllCombo->setEnabled(false);
        cpuScalingGovernorWidget->setGovernorAll(text);
        scalingGovAllCombo->setCurrentIndex(0);
        scalingGovAllCombo->setEnabled(true);
    }
}
