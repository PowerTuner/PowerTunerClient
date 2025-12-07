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
#include <QVBoxLayout>
#include <QPushButton>

#include "BlockQueueSchedulerWidget.h"

namespace PWT::UI::LNX {
    BlockQueueSchedulerWidget::BlockQueueSchedulerWidget(const QString &device, const PWTS::LNX::BlockDeviceQueSched &data) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *devLyt = new QHBoxLayout();
        QPushButton *deleteBtn = new QPushButton("remove");

        deviceLabel = new QLabel(data.name);
        scheduler = new QComboBox();
        blockDevice = device;

        deviceLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        scheduler->addItems(data.availableQueueSchedulers);
        scheduler->setCurrentIndex(scheduler->findText(data.scheduler));
        deleteBtn->setToolTip("remove this device, until next UI refresh, so that it wont be saved to a profile");

        devLyt->addWidget(deviceLabel);
        devLyt->addWidget(new QLabel(QString("[device: %1]").arg(device)));
        devLyt->addStretch();
        devLyt->addWidget(scheduler);
        devLyt->addWidget(deleteBtn);
        lyt->addLayout(devLyt);

        setLayout(lyt);

        QObject::connect(deleteBtn, &QPushButton::clicked, this, &BlockQueueSchedulerWidget::onDeleteBtnClicked);
    }

    QString BlockQueueSchedulerWidget::getDevice() const {
        return blockDevice;
    }

    QString BlockQueueSchedulerWidget::getScheduler() const {
        return scheduler->currentText();
    }

    void BlockQueueSchedulerWidget::onDeleteBtnClicked() {
        emit deleteDevice();
    }
}
