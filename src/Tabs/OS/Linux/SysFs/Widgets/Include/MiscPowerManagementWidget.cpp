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

#include "MiscPowerManagementWidget.h"

namespace PWT::UI::LNX {
    MiscPowerManagementWidget::MiscPowerManagementWidget(const PWTS::LNX::MiscPMDevice &device) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *controlLyt = new QHBoxLayout();
        QPushButton *deleteBtn = new QPushButton("remove");

        name = new QLabel(device.name);
        devPath = new QLabel(device.control);
        mode = new QComboBox();

        name->setTextInteractionFlags(Qt::TextSelectableByMouse);
        devPath->setTextInteractionFlags(Qt::TextSelectableByMouse);
        mode->addItems({"auto", "on"});
        mode->setCurrentIndex(mode->findText(device.controlValue));
        mode->setToolTip("auto: allow runtime power management\non: prevent runtime power management");
        deleteBtn->setToolTip("remove this device, until next UI refresh, so that it wont be saved to a profile");

        controlLyt->addWidget(devPath);
        controlLyt->addStretch();
        controlLyt->addWidget(mode);
        controlLyt->addWidget(deleteBtn);
        lyt->addWidget(name);
        lyt->addLayout(controlLyt);

        setLayout(lyt);

        QObject::connect(deleteBtn, &QPushButton::clicked, this, &MiscPowerManagementWidget::onDeleteBtnClicked);
    }

    void MiscPowerManagementWidget::onDeleteBtnClicked() {
        emit deleteDevice();
    }
}
