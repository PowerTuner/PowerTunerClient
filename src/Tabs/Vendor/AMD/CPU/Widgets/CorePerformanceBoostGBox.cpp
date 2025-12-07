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

#include "CorePerformanceBoostGBox.h"

namespace PWT::UI::AMD {
    CorePerformanceBoostGBox::CorePerformanceBoostGBox(const int threadCount): QGroupBox("Core Performance Boost") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *allLyt = new QHBoxLayout();

        corePerformanceBoostWidget = new CorePerformanceBoostWidget(threadCount);
        corePerfBoostAll = new QComboBox();

        corePerfBoostAll->addItems({"", "Enabled", "Disabled"});

        allLyt->addStretch();
        allLyt->addWidget(new QLabel("Core Performance Boost (All):"));
        allLyt->addWidget(corePerfBoostAll);
        lyt->addLayout(allLyt);
        lyt->addWidget(corePerformanceBoostWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(corePerfBoostAll, &QComboBox::currentIndexChanged, this, &CorePerformanceBoostGBox::onCorePerfBoostAllChanged);
    }

    void CorePerformanceBoostGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(true);
        corePerformanceBoostWidget->setData(packet);
    }

    void CorePerformanceBoostGBox::onCorePerfBoostAllChanged(const int idx) const {
        if (idx == 0)
            return;

        const QSignalBlocker sblock {corePerfBoostAll};

        corePerformanceBoostWidget->setCorePerformanceBoostAll(idx - 1);
        corePerfBoostAll->setCurrentIndex(0);
    }
}
