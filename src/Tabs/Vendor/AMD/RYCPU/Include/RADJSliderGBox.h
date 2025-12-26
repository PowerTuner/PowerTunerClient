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

#include <QGroupBox>
#include <QCheckBox>

#include "../../../../Widgets/SliderUnitWidget.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::AMD {
    class RADJSliderGBox: public QGroupBox {
        Q_OBJECT

    protected:
        SliderUnitWidget *slider = nullptr;
        QPointer<QCheckBox> enableChk;
        bool enableChecked = false;

    public:
        RADJSliderGBox(const QString &title, const QString &label, const QString &unit, const std::function<void(QLabel *,int)> &unitVCallback, bool hasReadFeature);

        void setMaximum(const int max) const { slider->setMaximum(max); }

        virtual void setDataForPacket(const PWTS::ClientPacket &packet) const = 0;
        virtual void setData(const PWTS::DaemonPacket &packet) = 0;

        void setRange(const PWTS::MinMax &range) const;

    private slots:
        void onSliderValueChanged(int v);
        void onEnableStateChanged(Qt::CheckState state);

    signals:
        void sliderValueChanged(int v);
    };
}
