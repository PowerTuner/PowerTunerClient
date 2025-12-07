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

#include <QPushButton>
#include <QHBoxLayout>

namespace PWT::UI {
    class UIActions final: public QObject {
        Q_OBJECT

    private:
        QHBoxLayout *lyt = nullptr;
        QPushButton *reloadDataFromDaemonBtn = nullptr;
        QPushButton *applyCurrentSettingsBtn = nullptr;

    public:
        UIActions();

        [[nodiscard]] QHBoxLayout *getLayout() const { return lyt; }

        void enableButtons(bool enable) const;

    private slots:
        void onReloadFromDaemonClicked();
        void onApplyCurrentSettingsClicked();

    signals:
        void reloadFromDaemon();
        void applyCurrentSettings();
    };
}
