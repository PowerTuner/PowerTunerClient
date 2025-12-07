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
#include "SliderUnitWidget.h"

namespace PWT::UI {
    SliderUnitWidget::SliderUnitWidget(const QString &unit, const std::function<void(QLabel *, int)> &unitValueCallback, const bool hasLabel, const QString &title) {
        QHBoxLayout *unitLyt = new QHBoxLayout();

        unitLabel = new QLabel(unit);
        unitValue = new QLabel("0");
        unitValueCB = unitValueCallback;

        if (!title.isEmpty())
            lyt->insertWidget(0, new QLabel(title));

        if (hasLabel) {
            label = new QLabel("0");

            unitLyt->addWidget(label);
        }

        unitLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        unitLyt->addStretch();
        unitLyt->addWidget(unitValue);
        unitLyt->addWidget(unitLabel);
        unitLyt->addSpacing(modeBtnWidth);
        lyt->addLayout(unitLyt);
    }

    void SliderUnitWidget::setUnitLabel(const QString &unit) const {
        unitLabel->setText(unit);
    }

    void SliderUnitWidget::setUnitValue(const int value) const {
        unitValue->setNum(value);
    }

    void SliderUnitWidget::setUnitValue(const double value) const {
        unitValue->setNum(value);
    }

    void SliderUnitWidget::setUnitValue(const QString &text) const {
        unitValue->setText(text);
    }

    void SliderUnitWidget::onSliderChanged(const int v) {
        BaseSliderWidget::onSliderChanged(v);
        unitValueCB(unitValue, v);
    }

    void SliderUnitWidget::onSpinBoxChanged(const int v) {
        BaseSliderWidget::onSliderChanged(v);
        unitValueCB(unitValue, v);
    }
}
