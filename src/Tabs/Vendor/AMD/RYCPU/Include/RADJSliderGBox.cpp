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
#include "RADJSliderGBox.h"

namespace PWT::UI::AMD {
    RADJSliderGBox::RADJSliderGBox(const QString &title, const QString &label, const QString &unit, const std::function<void(QLabel *,int)> &unitVCallback, const bool hasReadFeature): QGroupBox(title) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new SliderUnitWidget(unit, unitVCallback);

        slider->setStaticLabel(label);

        if (!hasReadFeature) { // cannot read its value from table, make this an optional setting
            enableChk = new QCheckBox("Enable setting");

            enableChk->setToolTip("If unchecked, this setting is ignored and wont be applied");
            lyt->addWidget(enableChk);
            lyt->addSpacing(4);

            QObject::connect(enableChk, &QCheckBox::checkStateChanged, this, &RADJSliderGBox::onEnableStateChanged);
        }

        lyt->addWidget(slider);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(slider, &SliderUnitWidget::valueChanged, this, &RADJSliderGBox::onSliderValueChanged);
    }

    void RADJSliderGBox::setRange(const PWTS::MinMax &range) const {
        const QSignalBlocker sblock {slider};

        slider->setRange(range.min, range.max);
    }

    void RADJSliderGBox::onSliderValueChanged(const int v) {
        emit sliderValueChanged(v);
    }

    void RADJSliderGBox::onEnableStateChanged(const Qt::CheckState state) {
        enableChecked = state == Qt::Checked;
    }
}
