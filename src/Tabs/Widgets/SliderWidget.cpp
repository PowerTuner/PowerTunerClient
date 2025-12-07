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
#include "SliderWidget.h"

namespace PWT::UI {
    SliderWidget::SliderWidget(const QString &title) {
        label = new QLabel("0");

        if (!title.isEmpty()) {
            QHBoxLayout *footLyt = new QHBoxLayout();

            footLyt->addWidget(new QLabel(title));
            footLyt->addStretch();
            footLyt->addWidget(label);
            footLyt->addSpacing(modeBtnWidth);
            lyt->addLayout(footLyt);

        } else {
            label->setAlignment(Qt::AlignRight);
            lyt->addWidget(label);
        }
    }
}
