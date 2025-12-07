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
#include "SliderClockGBox.h"

namespace PWT::UI::AMD {
    SliderClockGBox::SliderClockGBox(const QString &title, const QString &label, const bool enableDontSet): QGroupBox(title) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new SliderUnitWidget("MHz", [](QLabel *unitV, const int v) { unitV->setNum(v); });

        slider->setStaticLabel(label);
        slider->setPageStep(100);

        if (enableDontSet) {
            QHBoxLayout *dontSetLyt = new QHBoxLayout();

            dontSet = new QCheckBox("Don't apply this setting");

            dontSetLyt->setAlignment(Qt::AlignRight);
            dontSetLyt->addWidget(dontSet);
            lyt->addLayout(dontSetLyt);
            lyt->addSpacing(4);

            QObject::connect(dontSet, &QCheckBox::checkStateChanged, this, &SliderClockGBox::onDontSetChanged);
        }

        lyt->addWidget(slider);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void SliderClockGBox::setRange(const PWTS::MinMax &range) const {
        slider->setRange(range.min, range.max);
    }

    void SliderClockGBox::onDontSetChanged(const Qt::CheckState state) const {
        slider->setEnabled(state == Qt::Unchecked);
    }
}
