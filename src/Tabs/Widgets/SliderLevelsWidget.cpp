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
#include "SliderLevelsWidget.h"

namespace PWT::UI {
    SliderLevelsWidget::SliderLevelsWidget(const QString &leftLbl, const QString &rightLbl, const QString &title) {
        QHBoxLayout *lvlLyt = new QHBoxLayout();
        QHBoxLayout *headLyt = new QHBoxLayout();
        QLabel *left = new QLabel(leftLbl);
        QLabel *right = new QLabel(rightLbl);

        label = new QLabel("0");

        if (!title.isEmpty())
            headLyt->addWidget(new QLabel(title));

        headLyt->addStretch();
        headLyt->addWidget(label);
        lvlLyt->addWidget(left);
        lvlLyt->addStretch();
        lvlLyt->addWidget(right);
        lvlLyt->addSpacing(modeBtnWidth);
        lyt->insertLayout(0, headLyt);
        lyt->addLayout(lvlLyt);
    }
}
