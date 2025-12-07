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
#include "SliderLimitGBox.h"

namespace PWT::UI::AMD {
    SliderLimitGBox::SliderLimitGBox(const QString &title, const QString &label, const QString &unit, const std::function<void(QLabel *,int)> &unitVCallback): QGroupBox(title) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new SliderUnitWidget(unit, unitVCallback);

        slider->setStaticLabel(label);
        slider->setPageStep(100);

        lyt->addWidget(slider);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(slider, &SliderUnitWidget::valueChanged, this, &SliderLimitGBox::onSliderValueChanged);
    }

    void SliderLimitGBox::setRange(const PWTS::MinMax &range) const {
        const QSignalBlocker sblock {slider};

        slider->setRange(range.min, range.max);
    }

    void SliderLimitGBox::onSliderValueChanged(const int v) {
        emit sliderValueChanged(v);
    }
}
