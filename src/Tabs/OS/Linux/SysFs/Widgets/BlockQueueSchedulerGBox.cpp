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
#include "BlockQueueSchedulerGBox.h"

namespace PWT::UI::LNX {
    BlockQueueSchedulerGBox::BlockQueueSchedulerGBox(): QGroupBox("Block device queue scheduler") {
        lyt = new QVBoxLayout();

        setPlaceholder();

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
    }

    void BlockQueueSchedulerGBox::setPlaceholder() const {
        QLabel *emptyLbl = new QLabel(QStringLiteral("No devices found"));

        emptyLbl->setAlignment(Qt::AlignCenter);
        lyt->addWidget(emptyLbl);
    }

    void BlockQueueSchedulerGBox::clearGBox() {
        while (lyt->count()) {
            QLayoutItem *itm = lyt->takeAt(0);

            itm->widget()->deleteLater();
            delete itm;
        }

        blkDevList.clear();
    }

    void BlockQueueSchedulerGBox::setData(const PWTS::DaemonPacket &packet) {
        clearGBox();

        for (const auto &[device, data]: packet.linuxData->blockDevicesQueSched.asKeyValueRange()) {
            BlockQueueSchedulerWidget *dev = new BlockQueueSchedulerWidget(device, data);

            lyt->addWidget(dev);
            blkDevList.append(dev);

            QObject::connect(dev, &BlockQueueSchedulerWidget::deleteDevice, this, &BlockQueueSchedulerGBox::onDeleteBtnClicked);
        }

        if (blkDevList.isEmpty())
            setPlaceholder();
    }

    void BlockQueueSchedulerGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (const BlockQueueSchedulerWidget *devW: blkDevList)
            packet.linuxData->blockDevicesQueSched.insert(devW->getDevice(), {.scheduler = devW->getScheduler()});
    }

    void BlockQueueSchedulerGBox::onDeleteBtnClicked() {
        const BlockQueueSchedulerWidget *blkDev = qobject_cast<BlockQueueSchedulerWidget *>(QObject::sender());

        if (blkDev == nullptr)
            return;

        const int idx = lyt->indexOf(blkDev);

        if (idx == -1)
            return;

        const QLayoutItem *itm = lyt->takeAt(idx);

        itm->widget()->deleteLater();
        blkDevList.remove(idx);
        delete itm;
    }
}
