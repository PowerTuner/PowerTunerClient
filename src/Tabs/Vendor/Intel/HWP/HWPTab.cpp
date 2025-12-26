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
#include "HWPTab.h"
#include "../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    HWPTab::HWPTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;
        FlowLayout *optionsLyt = new FlowLayout();

        enableHWP = new QCheckBox("HWP Enable");
        hwpRequestGBox = new HWPRequestGBox(features, packet.cpuInfo.numLogicalCpus);

        if (features.contains(PWTS::Feature::PWR_SCHEME_GROUP)) {
            preferOSSettng = new QCheckBox("Prefer OS setting");

            preferOSSettng->setChecked(true);
            preferOSSettng->setToolTip("If checked, this option is ignored and the OS setting will be applied instead. (SysFs in Linux, power scheme in Windows, etc..)");
            optionsLyt->addWidget(preferOSSettng);

            QObject::connect(preferOSSettng, &QCheckBox::checkStateChanged, this, &HWPTab::onPreferOSStateChanged);
        }

        optionsLyt->addWidget(enableHWP);

        if (features.contains(PWTS::Feature::INTEL_HWP_CTL)) {
            hwpPkgCtlPolarity = new QCheckBox("HWP Pkg Ctl Polarity enable");
            optionsLyt->addWidget(hwpPkgCtlPolarity);
        }

        scrollWidgLyt->insertLayout(nextInsertIdx(), optionsLyt);
        scrollWidgLyt->insertSpacing(nextInsertIdx(), 6);
        scrollWidgLyt->insertWidget(nextInsertIdx(), hwpRequestGBox);

        if (features.contains(PWTS::Feature::INTEL_HWP_REQ_PKG)) {
            hwpRequestPkgGBox = new HWPRequestPkgGBox(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), hwpRequestPkgGBox);
        }
    }

    void HWPTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const int hwpEnable = packet.intelData->hwpEnable.getValue();
        const bool hwpEnableValid = packet.intelData->hwpEnable.isValid();

        enableHWP->setEnabled(hwpEnableValid);
        enableHWP->setChecked(hwpEnableValid && hwpEnable == 1);
        hwpRequestGBox->setData(packet);

        if (!hwpRequestPkgGBox.isNull())
            hwpRequestPkgGBox->setData(packet);

        if (!hwpPkgCtlPolarity.isNull()) {
            hwpPkgCtlPolarity->setEnabled(packet.intelData->hwpPkgCtlPolarity.isValid());

            if (hwpPkgCtlPolarity->isEnabled())
                hwpPkgCtlPolarity->setChecked(packet.intelData->hwpPkgCtlPolarity.getValue());
        }
    }

    void HWPTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        const bool isIgnored = !preferOSSettng.isNull() && preferOSSettng->isChecked();

        packet.intelData->hwpEnable.setValue(enableHWP->isChecked(), enableHWP->isEnabled(), isIgnored);
        hwpRequestGBox->setDataForPacket(packet, isIgnored);

        if (!hwpRequestPkgGBox.isNull())
            hwpRequestPkgGBox->setDataForPacket(packet, isIgnored);

        if (!hwpPkgCtlPolarity.isNull())
            packet.intelData->hwpPkgCtlPolarity.setValue(hwpPkgCtlPolarity->isChecked(), hwpPkgCtlPolarity->isEnabled(), isIgnored);
    }

    void HWPTab::onPreferOSStateChanged(const Qt::CheckState state) {
        preferOSChecked = state == Qt::Checked;
    }
}
