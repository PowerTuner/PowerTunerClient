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
#include <QFile>
#include <QVBoxLayout>

#include "GPUInfoGBox.h"
#include "LabelInfoWidget.h"
#include "pwtClientCommon/CommonUtils.h"

namespace PWT::UI {
    GPUInfoGBox::GPUInfoGBox(): QGroupBox("GPU") {
        setAlignment(Qt::AlignCenter);
    }

    QString GPUInfoGBox::getGPUVendorString(const PWTS::GPUVendor vendor) const {
        switch (vendor) {
            case PWTS::GPUVendor::AMD:
                return "AMD";
            case PWTS::GPUVendor::Intel:
                return "Intel";
            case PWTS::GPUVendor::NVIDIA:
                return "NVIDIA";
            default:
                return "Unknown";
        }
    }

    void GPUInfoGBox::setData(const QMap<int, PWTS::GpuInfo> &infoMap) {
        QVBoxLayout *lyt = new QVBoxLayout();

        for (const PWTS::GpuInfo &info: infoMap) {
            const QString marketName = getGPUMarketName(info.vendor, info.deviceID.sliced(2), info.revisionID.sliced(2));

            lyt->addWidget(new LabelInfoWidget("Card Index", QString::number(info.index)));
            lyt->addWidget(new LabelInfoWidget("Vendor", getGPUVendorString(info.vendor)));
            lyt->addWidget(new LabelInfoWidget("Device ID", info.deviceID));
            lyt->addWidget(new LabelInfoWidget("Revision ID", info.revisionID));

            if (!info.vbiosVersion.isEmpty())
                lyt->addWidget(new LabelInfoWidget("VBios Version", info.vbiosVersion));

            if (!marketName.isEmpty())
                lyt->addWidget(new LabelInfoWidget("Name", marketName));
        }

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->setSpacing(0);
        setLayout(lyt);
    }
}
