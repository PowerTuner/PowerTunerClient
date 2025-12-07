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
#pragma once

#include <QComboBox>
#include <QLabel>

#include "pwtShared/Include/OS/Linux/BlockDeviceQueSched.h"

namespace PWT::UI::LNX {
    class BlockQueueSchedulerWidget final: public QWidget {
        Q_OBJECT

    private:
        QLabel *deviceLabel = nullptr;
        QComboBox *scheduler = nullptr;
        QString blockDevice;

    public:
        BlockQueueSchedulerWidget(const QString &device, const PWTS::LNX::BlockDeviceQueSched &data);

        [[nodiscard]] QString getDevice() const;
        [[nodiscard]] QString getScheduler() const;

    private slots:
        void onDeleteBtnClicked();

    signals:
        void deleteDevice();
    };
}
