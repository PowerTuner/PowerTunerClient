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
#include "PowerCtlNHLMGBox.h"
#include "pwtClientCommon/UILogger.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PowerCtlNHLMGBox::PowerCtlNHLMGBox() {
        FlowLayout *lyt = new FlowLayout();

        c1e = new QCheckBox("C1 Enhanced (C1E)");

        lyt->addWidget(c1e);

        setLayout(lyt);
    }

    void PowerCtlNHLMGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->powerCtl.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::PowerCtl data = packet.intelData->powerCtl.getValue();

        c1e->setChecked(data.c1eEnable);
    }

    void PowerCtlNHLMGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->powerCtl.setValue({
            .c1eEnable = c1e->isChecked(),
        }, true);
    }
}
