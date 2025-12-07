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
#include "BaseSliderWidget.h"

namespace PWT::UI {
    BaseSliderWidget::BaseSliderWidget() {
        QHBoxLayout *inptLyt = new QHBoxLayout();

        lyt = new QVBoxLayout();
        slider = new QSlider(Qt::Horizontal);
        spinEdit = new QSpinBox();
        modeBtn = new QPushButton("T");

        spinEdit->setVisible(false);
        spinEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        modeBtn->setMaximumWidth(modeBtnWidth);
        modeBtn->setToolTip("Switch between slider and text mode");

        inptLyt->setContentsMargins(0, 0, 0, 0);
        inptLyt->addWidget(slider);
        inptLyt->addWidget(spinEdit);
        inptLyt->addWidget(modeBtn);
        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addLayout(inptLyt);
        setLayout(lyt);

        QObject::connect(modeBtn, &QPushButton::clicked, this, &BaseSliderWidget::onModeBtnClicked);
        QObject::connect(slider, &QSlider::valueChanged, this, &BaseSliderWidget::onSliderChanged);
        QObject::connect(spinEdit, &QSpinBox::valueChanged, this, &BaseSliderWidget::onSpinBoxChanged);
    }

    void BaseSliderWidget::setRange(const int min, const int max) const {
        slider->setRange(min, max);
        spinEdit->setRange(min, max);
    }

    void BaseSliderWidget::setStaticLabel(const QString &text) const {
        staticLabel = true;
        label->setText(text);
    }

    void BaseSliderWidget::setValue(const int value) const {
        if (isSlider)
            slider->setValue(value);
        else
            spinEdit->setValue(value);
    }

    void BaseSliderWidget::onModeBtnClicked() const {
        if (isSlider) {
            const QSignalBlocker sblock {spinEdit};

            slider->setVisible(false);
            spinEdit->setVisible(true);
            spinEdit->setValue(currentValue);
            isSlider = false;

        } else {
            const QSignalBlocker sblock {slider};

            spinEdit->setVisible(false);
            slider->setVisible(true);
            slider->setValue(currentValue);
            isSlider = true;
        }
    }

    void BaseSliderWidget::onSliderChanged(const int v) {
        if (!label.isNull() && !staticLabel)
            label->setNum(v);

        currentValue = v;
        emit valueChanged(v);
    }

    void BaseSliderWidget::onSpinBoxChanged(const int v) {
        if (v < slider->minimum() || v > slider->maximum())
            return;

        if (!label.isNull() && !staticLabel)
            label->setNum(v);

        currentValue = v;
        emit valueChanged(v);
    }
}
