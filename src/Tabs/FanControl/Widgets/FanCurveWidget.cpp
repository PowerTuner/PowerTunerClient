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

#include "FanCurveWidget.h"

namespace PWT::UI::FANS {
    FanCurveWidget::FanCurveWidget(): QGroupBox(QStringLiteral("Trip Point")) {
        QVBoxLayout *lyt = new QVBoxLayout();

        temperature = new QSpinBox();
        speedPerc = new QSpinBox();
        removeBtn = new QPushButton("remove");

        temperature->setRange(0, 100);
        temperature->setValue(temperature->minimum());
        temperature->setMinimumWidth(120);
        speedPerc->setRange(0, 100);
        speedPerc->setValue(speedPerc->minimum());
        speedPerc->setMinimumWidth(120);

        lyt->addWidget(new QLabel("Temperature (°C)"));
        lyt->addWidget(temperature);
        lyt->addSpacing(4);
        lyt->addWidget(new QLabel("Speed (%)"));
        lyt->addWidget(speedPerc);
        lyt->addSpacing(4);
        lyt->addWidget(removeBtn);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(removeBtn, &QPushButton::clicked, this, &FanCurveWidget::onRemoveBtnClicked);
    }

    void FanCurveWidget::setData(const int temp, const int speed) const {
        temperature->setValue(temp);
        speedPerc->setValue(speed);
    }

    void FanCurveWidget::onRemoveBtnClicked() {
        emit removePoint(this);
    }
}
