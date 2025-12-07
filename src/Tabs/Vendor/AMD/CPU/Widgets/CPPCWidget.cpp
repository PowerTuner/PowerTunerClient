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
#include "CPPCWidget.h"

namespace PWT::UI::AMD {
    CPPCWidget::CPPCWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        maxPerf = new SliderLevelsWidget("Lowest", "Highest", "Maximum performance hint");
        minPerf = new SliderLevelsWidget("Lowest", "Highest", "Minimum performance hint");
        desPerf = new SliderLevelsWidget("Lowest", "Highest", "Desired performance hint");
        epp = new SliderLevelsWidget("Lowest", "Highest", "Energy performance preference hint");

        epp->setRange(0, 255);

        lyt->addWidget(maxPerf);
        lyt->addSpacing(6);
        lyt->addWidget(minPerf);
        lyt->addSpacing(6);
        lyt->addWidget(desPerf);
        lyt->addSpacing(6);
        lyt->addWidget(epp);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(maxPerf, &SliderLevelsWidget::valueChanged, this, &CPPCWidget::onMaxPerfChanged);
        QObject::connect(desPerf, &SliderLevelsWidget::valueChanged, this, &CPPCWidget::onDesPerfChanged);
        QObject::connect(epp, &SliderLevelsWidget::valueChanged, this, &CPPCWidget::onEppChanged);
    }

    void CPPCWidget::setRanges(const PWTS::AMD::CPPCCapability1 &cap1) const {
        const QSignalBlocker maxBlock {maxPerf};
        const QSignalBlocker desBlock {desPerf};

        maxPerf->setRange(cap1.lowestPerf, cap1.highestPerf);
        minPerf->setRange(cap1.lowestPerf, cap1.highestPerf);
        desPerf->setRange(0, cap1.highestPerf);
    }

    void CPPCWidget::setData(const PWTS::AMD::CPPCRequest &data) const {
        const QSignalBlocker maxblock {maxPerf};
        const QSignalBlocker desblock {desPerf};
        const QSignalBlocker eppblock {epp};

        maxPerf->setValue(data.maxPerf);
        minPerf->setValue(data.minPerf);
        desPerf->setValue(data.desPerf);
        epp->setValue(data.epp);

        onMaxPerfChanged(maxPerf->getValue());
        onDesPerfChanged(desPerf->getValue());
        onEppChanged(epp->getValue());
    }

    PWTS::AMD::CPPCRequest CPPCWidget::getData() const {
        return {
            .maxPerf = maxPerf->getValue(),
            .minPerf = minPerf->getValue(),
            .desPerf = desPerf->getValue(),
            .epp = epp->getValue()
        };
    }

    void CPPCWidget::onMaxPerfChanged(const int v) const {
        minPerf->setMaximum(v);
    }

    void CPPCWidget::onDesPerfChanged(const int v) const {
        if (v == 0)
            desPerf->setLabel("Autonomous");
    }

    void CPPCWidget::onEppChanged(const int v) const {
        epp->setLabel(QString("%1 (%2 %)").arg(v).arg(qFloor((static_cast<float>(v) / epp->getMaximum()) * 100)));
    }
}
