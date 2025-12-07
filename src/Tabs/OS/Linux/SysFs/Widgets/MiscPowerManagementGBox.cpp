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
#include "MiscPowerManagementGBox.h"

namespace PWT::UI::LNX {
    MiscPowerManagementGBox::MiscPowerManagementGBox(): QGroupBox("Misc. Power Management") {
        lyt = new QVBoxLayout();

        setPlaceholder();

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
    }

    void MiscPowerManagementGBox::setPlaceholder() const {
        QLabel *emptyLbl = new QLabel(QStringLiteral("No devices found"));

        emptyLbl->setAlignment(Qt::AlignCenter);
        lyt->addWidget(emptyLbl);
    }

    void MiscPowerManagementGBox::clearGBox() {
        while (lyt->count()) {
            QLayoutItem *itm = lyt->takeAt(0);

            itm->widget()->deleteLater();
            delete itm;
        }

        pmDevList.clear();
    }

    void MiscPowerManagementGBox::setData(const PWTS::DaemonPacket &packet) {
        clearGBox();

        for (const PWTS::LNX::MiscPMDevice &miscPmDev: packet.linuxData->miscPMDevices) {
            MiscPowerManagementWidget *pmDev = new MiscPowerManagementWidget(miscPmDev);

            lyt->addWidget(pmDev);
            pmDevList.append(pmDev);

            QObject::connect(pmDev, &MiscPowerManagementWidget::deleteDevice, this, &MiscPowerManagementGBox::onDeleteBtnClicked);
        }

        if (pmDevList.isEmpty())
            setPlaceholder();
    }

    void MiscPowerManagementGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (const MiscPowerManagementWidget *devW: pmDevList) {
            packet.linuxData->miscPMDevices.append({
                .name = devW->getName(),
                .control = devW->getPath(),
                .controlValue = devW->getMode()
            });
        }
    }

    void MiscPowerManagementGBox::onDeleteBtnClicked() {
        const MiscPowerManagementWidget *pmDev = qobject_cast<MiscPowerManagementWidget *>(QObject::sender());

        if (pmDev == nullptr)
            return;

        const int idx = lyt->indexOf(pmDev);

        if (idx == -1)
            return;

        const QLayoutItem *itm = lyt->takeAt(idx);

        itm->widget()->deleteLater();
        pmDevList.remove(idx);
        delete itm;
    }
}
