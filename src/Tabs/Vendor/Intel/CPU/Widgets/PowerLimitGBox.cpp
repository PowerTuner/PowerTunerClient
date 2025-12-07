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
#include "PowerLimitGBox.h"
#include "pwtClientCommon/UILogger.h"
#include "../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PowerLimitGBox::PowerLimitGBox(): QGroupBox("Power limits") {
        const auto sliderUnitValCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *lockSettingLyt = new QHBoxLayout();
        QHBoxLayout *pl1Lyt = new QHBoxLayout();
        QHBoxLayout *pl2Lyt = new QHBoxLayout();
        FlowLayout *optLyt = new FlowLayout();

        lock = new QCheckBox("Lock until device reboot");
        pl1 = new SliderUnitWidget("Watts", sliderUnitValCB);
        pl1Time = new SliderUnitWidget("Seconds", sliderUnitValCB);
        pl2 = new SliderUnitWidget("Watts", sliderUnitValCB);
        pl2Time = new SliderUnitWidget("Seconds", sliderUnitValCB);
        pl1Enable = new QCheckBox("Enable PL1 limit");
        pl1Clamp = new QCheckBox("Clamp PL1 limit");
        pl2Enable = new QCheckBox("Enable PL2 limit");
        pl2Clamp = new QCheckBox("Clamp PL2 limit");

        pl1->setStaticLabel("Long term (PL1)");
        pl1->setPageStep(100);
        pl1Time->setStaticLabel("PL1 time window");
        pl1Time->setRange(0, 56000);
        pl1Time->setPageStep(100);
        pl2->setStaticLabel("Short term (PL2)");
        pl2->setPageStep(100);
        pl2Time->setStaticLabel("PL2 time window");
        pl2Time->setRange(0, 56000);
        pl2Time->setPageStep(100);

        lockSettingLyt->addWidget(lock);
        lockSettingLyt->addStretch();
        pl1Lyt->addWidget(pl1);
        pl1Lyt->addSpacing(4);
        pl1Lyt->addWidget(pl1Time);
        pl2Lyt->addWidget(pl2);
        pl2Lyt->addSpacing(4);
        pl2Lyt->addWidget(pl2Time);
        optLyt->addWidget(pl1Enable);
        optLyt->addWidget(pl2Enable);
        optLyt->addWidget(pl1Clamp);
        optLyt->addWidget(pl2Clamp);
        lyt->addLayout(lockSettingLyt);
        lyt->addSpacing(4);
        lyt->addLayout(pl1Lyt);
        lyt->addLayout(pl2Lyt);
        lyt->addSpacing(4);
        lyt->addLayout(optLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(pl1Enable, &QCheckBox::checkStateChanged, this, &PowerLimitGBox::onPl1EnableChanged);
        QObject::connect(pl2Enable, &QCheckBox::checkStateChanged, this, &PowerLimitGBox::onPl2EnableChanged);
        QObject::connect(pl2, &SliderUnitWidget::valueChanged, this, &PowerLimitGBox::onPl2Changed);
    }

    void PowerLimitGBox::setRanges(const PWTS::MinMax &range) const {
        const QSignalBlocker sblock {pl2};

        pl1->setRange(range.min, range.max - 1000);
        pl2->setRange(range.min, range.max);
    }

    void PowerLimitGBox::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::Intel::PkgPowerLimit limits = packet.intelData->pkgPowerLimit.getValue();
        const bool isValid = packet.intelData->pkgPowerLimit.isValid();

        setEnabled(isValid && !limits.lock);

        if (!isValid) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        lock->setChecked(limits.lock);
        pl1->setValue(limits.pl1);
        pl1Time->setValue(limits.pl1Time);
        pl1Enable->setChecked(limits.pl1Enable);
        pl1Clamp->setChecked(limits.pl1Clamp);
        pl2->setValue(limits.pl2);
        pl2Time->setValue(limits.pl2Time);
        pl2Enable->setChecked(limits.pl2Enable);
        pl2Clamp->setChecked(limits.pl2Clamp);
    }

    void PowerLimitGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->pkgPowerLimit.setValue({
            .pl1 = pl1->getValue(),
            .pl2 = pl2->getValue(),
            .pl1Time = pl1Time->getValue(),
            .pl2Time = pl2Time->getValue(),
            .pl1Clamp = pl1Clamp->isChecked(),
            .pl2Clamp = pl2Clamp->isChecked(),
            .pl1Enable = pl1Enable->isChecked(),
            .pl2Enable = pl2Enable->isChecked(),
            .lock = lock->isChecked()
        }, true);
    }

    void PowerLimitGBox::onPl1EnableChanged(const Qt::CheckState state) const {
        pl1->setEnabled(state == Qt::Checked);
    }

    void PowerLimitGBox::onPl2EnableChanged(const Qt::CheckState state) const {
        pl2->setEnabled(state == Qt::Checked);
    }

    void PowerLimitGBox::onPl2Changed(const int v) const {
        pl1->setMaximum(v - 1000);
    }
}
