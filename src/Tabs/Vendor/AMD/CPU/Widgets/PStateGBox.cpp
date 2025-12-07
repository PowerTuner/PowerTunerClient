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

#include "PStateGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    PStateGBox::PStateGBox(const int threadCount): QGroupBox("P-State Control") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *pstateAllLyt = new QHBoxLayout();
        QLabel *pstateAllLbl = new QLabel("P-State (All CPUs)");

        pstateWidget = new PStateWidget(threadCount);
        pstateAll = new QComboBox();

        pstateAll->setMinimumWidth(100);
        pstateAllLbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        pstateAllLyt->addStretch();
        pstateAllLyt->addWidget(pstateAllLbl);
        pstateAllLyt->addWidget(pstateAll);
        lyt->addLayout(pstateAllLyt);
        lyt->addSpacing(2);
        lyt->addWidget(pstateWidget);

        setLayout(lyt);
        setAlignment(Qt::AlignCenter);
        setEnabled(false);

        QObject::connect(pstateAll, &QComboBox::currentIndexChanged, this, &PStateGBox::onPStateAllChanged);
    }

    void PStateGBox::setData(const PWTS::DaemonPacket &packet) {
        const QSignalBlocker sblock {pstateAll};

        setEnabled(packet.amdData->pstateCurrentLimit.isValid());
        pstateAll->clear();

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::AMD::PStateCurrentLimit limits = packet.amdData->pstateCurrentLimit.getValue();

        pstateAll->addItem("");
        pstateWidget->setData(packet);

        for (int p=limits.curPStateLimit; p<=limits.pstateMaxValue; ++p)
            pstateAll->addItem(QString::number(p));
    }

    void PStateGBox::onPStateAllChanged(const int idx) const {
        pstateAll->setCurrentIndex(0);
        pstateWidget->setAllPStates(idx - 1);
    }
}
