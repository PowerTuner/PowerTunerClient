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
#include "VRCurrentConfigGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    VRCurrentConfigGBox::VRCurrentConfigGBox(): QGroupBox("VR Current Config") {
        const auto sliderUnitValCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QVBoxLayout *lyt = new QVBoxLayout();

        lock = new QCheckBox("Lock until device reboot");
        pl4 = new SliderUnitWidget("Watts", sliderUnitValCB);

        pl4->setStaticLabel("Power limit 4");
        pl4->setPageStep(100);

        lyt->addWidget(lock);
        lyt->addSpacing(4);
        lyt->addWidget(pl4);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void VRCurrentConfigGBox::setRange(const PWTS::MinMax &range) const {
        pl4->setRange(range.min, range.max);
    }

    void VRCurrentConfigGBox::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::Intel::VRCurrentConfig data = packet.intelData->vrCurrentCfg.getValue();
        const bool isValid = packet.intelData->vrCurrentCfg.isValid();

        setEnabled(isValid && !data.lock);

        if (!isValid) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        lock->setChecked(data.lock);
        pl4->setValue(data.pl4);
    }

    void VRCurrentConfigGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->vrCurrentCfg.setValue({
            .pl4 = pl4->getValue(),
            .lock = lock->isChecked()
        }, true);
    }
}
