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
#include "TurboPowerCurrentGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    TurboPowerCurrentGBox::TurboPowerCurrentGBox(): QGroupBox("Turbo Power Current Limit") {
        const auto sliderUnitVCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QHBoxLayout *lyt = new QHBoxLayout();
        QVBoxLayout *tdpLyt = new QVBoxLayout();
        QVBoxLayout *tdcLyt = new QVBoxLayout();

        tdp = new SliderUnitWidget("Watts", sliderUnitVCB);
        tdpEnable = new QCheckBox("Enable TDP limit override");
        tdc = new SliderUnitWidget("Amps", sliderUnitVCB);
        tdcEnable = new QCheckBox("Enable TDC limit overrride");

        tdp->setStaticLabel("TDP Limit");
        tdp->setPageStep(100);
        tdc->setStaticLabel("TDC Limit");
        tdc->setPageStep(100);

        tdpLyt->addWidget(tdp);
        tdpLyt->addWidget(tdpEnable);
        tdcLyt->addWidget(tdc);
        tdcLyt->addWidget(tdcEnable);
        lyt->addLayout(tdpLyt);
        lyt->addSpacing(4);
        lyt->addLayout(tdcLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(tdpEnable, &QCheckBox::checkStateChanged, this, &TurboPowerCurrentGBox::onTDPEnableChanged);
        QObject::connect(tdcEnable, &QCheckBox::checkStateChanged, this, &TurboPowerCurrentGBox::onTDCEnableChanged);
    }

    void TurboPowerCurrentGBox::setRanges(const PWTS::MinMax &tdpRange, const PWTS::MinMax &tdcRange) const {
        tdp->setRange(tdpRange.min, tdpRange.max);
        tdc->setRange(tdcRange.min, tdcRange.max);
    }

    void TurboPowerCurrentGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->turboPowerCurrentLimit.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::TurboPowerCurrentLimit data = packet.intelData->turboPowerCurrentLimit.getValue();

        tdp->setValue(data.tdpLimit);
        tdpEnable->setChecked(data.tdpLimitOverride);
        tdc->setValue(data.tdcLimit);
        tdcEnable->setChecked(data.tdcLimitOverride);
    }

    void TurboPowerCurrentGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->turboPowerCurrentLimit.setValue({
            .tdpLimit = tdp->getValue(),
            .tdpLimitOverride = tdpEnable->isChecked(),
            .tdcLimit = tdc->getValue(),
            .tdcLimitOverride = tdcEnable->isChecked(),
        }, true);
    }

    void TurboPowerCurrentGBox::onTDPEnableChanged(const Qt::CheckState state) const {
        tdp->setEnabled(state == Qt::Checked);
    }

    void TurboPowerCurrentGBox::onTDCEnableChanged(const Qt::CheckState state) const {
        tdc->setEnabled(state == Qt::Checked);
    }
}
