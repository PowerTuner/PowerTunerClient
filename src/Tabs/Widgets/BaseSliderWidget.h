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
#pragma once

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPointer>
#include <QPushButton>

namespace PWT::UI {
    class BaseSliderWidget: public QWidget {
        Q_OBJECT

    private:
        QSlider *slider = nullptr;
        QSpinBox *spinEdit = nullptr;
        QPushButton *modeBtn = nullptr;
        mutable bool isSlider = true;
        mutable bool staticLabel = false;
        int currentValue = 0;

    protected:
        static constexpr int modeBtnWidth = 30;
        QVBoxLayout *lyt = nullptr;
        QPointer<QLabel> label;

    public:
        BaseSliderWidget();

        [[nodiscard]] int getValue() const { return currentValue; }
        [[nodiscard]] int getMinumum() const { return slider->minimum(); }
        [[nodiscard]] int getMaximum() const { return slider->maximum(); }
        void setPageStep(const int step) const { slider->setPageStep(step); }
        void setSingleStep(const int step) const { slider->setSingleStep(step); }
        void setMaximum(const int max) const { slider->setMaximum(max); }
        void setLabel(const QString &text) const { label->setText(text); }

        void setRange(int min, int max) const;
        virtual void setStaticLabel(const QString &text) const;
        void setValue(int value) const;

    protected slots:
        void onModeBtnClicked() const;
        virtual void onSliderChanged(int v);
        virtual void onSpinBoxChanged(int v);

    signals:
        void valueChanged(int v);
    };
}
