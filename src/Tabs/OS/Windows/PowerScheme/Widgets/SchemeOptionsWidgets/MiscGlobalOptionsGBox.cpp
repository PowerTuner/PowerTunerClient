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
#include <QHBoxLayout>

#include "MiscGlobalOptionsGBox.h"

namespace PWT::UI::WIN {
    MiscGlobalOptionsGBox::MiscGlobalOptionsGBox(): QGroupBox("Misc. Options") {
        QHBoxLayout *lyt = new QHBoxLayout();

        resetSchemes = new QCheckBox("Restore default power schemes before apply");
        replaceDefaults = new QCheckBox("Replace default power schemes");

        resetSchemes->setToolTip("Reset power schemes to Windows defaults before applying settings");
        replaceDefaults->setToolTip("Replace all Windows default power schemes with current schemes, so that reset to defaults will reset to custom scheme settings");

        lyt->addWidget(resetSchemes);
        lyt->addWidget(replaceDefaults);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
    }

    void MiscGlobalOptionsGBox::refreshWidget(const PWTS::DaemonPacket &packet) const {
        resetSchemes->setChecked(packet.windowsData->resetSchemesDefault);
        replaceDefaults->setChecked(packet.windowsData->replaceDefaultSchemes);
    }
}
