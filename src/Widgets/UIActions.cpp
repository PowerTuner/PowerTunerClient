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
#include "UIActions.h"

namespace PWT::UI {
    UIActions::UIActions() {
        lyt = new QHBoxLayout();
        reloadDataFromDaemonBtn = new QPushButton("Reload values from Daemon");
        applyCurrentSettingsBtn = new QPushButton("Apply current settings");

        reloadDataFromDaemonBtn->setMinimumHeight(28);
        applyCurrentSettingsBtn->setMinimumHeight(28);
        enableButtons(false);

        lyt->addWidget(reloadDataFromDaemonBtn);
        lyt->addSpacing(8);
        lyt->addWidget(applyCurrentSettingsBtn);

        QObject::connect(reloadDataFromDaemonBtn, &QPushButton::clicked, this, &UIActions::onReloadFromDaemonClicked);
        QObject::connect(applyCurrentSettingsBtn, &QPushButton::clicked, this, &UIActions::onApplyCurrentSettingsClicked);
    }

    void UIActions::enableButtons(const bool enable) const {
        reloadDataFromDaemonBtn->setEnabled(enable);
        applyCurrentSettingsBtn->setEnabled(enable);
    }

    void UIActions::onReloadFromDaemonClicked() {
        emit reloadFromDaemon();
    }

    void UIActions::onApplyCurrentSettingsClicked() {
        emit applyCurrentSettings();
    }
}
