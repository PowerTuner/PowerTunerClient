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
#include "CPUParkGBox.h"

namespace PWT::UI::LNX {
    CPUParkGBox::CPUParkGBox(const int threadCount): QGroupBox("CPU Parking") {
        QVBoxLayout *lyt = new QVBoxLayout();

        cpuParkWidget = new CPUParkWidget(threadCount);
        percentSlider = new SliderUnitWidget("%", [](QLabel *unitV, const int v) { unitV->setNum(v); }, false);

        percentSlider->setRange(0, 100);
        percentSlider->setPageStep(10);

        lyt->addWidget(percentSlider);
        lyt->addWidget(cpuParkWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(percentSlider, &SliderUnitWidget::valueChanged, this, &CPUParkGBox::onPercentSliderChanged);
        QObject::connect(cpuParkWidget, &CPUParkWidget::statusChanged, this, &CPUParkGBox::onCPUParkWidgetStatusChanged);
    }

    void CPUParkGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(true);
        cpuParkWidget->setData(packet);
        onCPUParkWidgetStatusChanged();
    }

    void CPUParkGBox::onPercentSliderChanged(const int v) const {
        const int onlineC = qFloor(static_cast<float>(v * cpuParkWidget->getCPUCount()) / 100.f);

        cpuParkWidget->updateOnlineCPUs(onlineC);
    }

    void CPUParkGBox::onCPUParkWidgetStatusChanged() const {
        const int perc = qFloor(static_cast<float>(cpuParkWidget->getCPUOnlineCount()) / static_cast<float>(cpuParkWidget->getCPUCount()) * 100);
        const QSignalBlocker sblock {percentSlider};

        percentSlider->setValue(perc);
    }
}
