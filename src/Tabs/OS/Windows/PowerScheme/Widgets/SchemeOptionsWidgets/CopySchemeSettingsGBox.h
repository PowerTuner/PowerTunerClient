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

#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>

namespace PWT::UI::WIN {
    class CopySchemeSettingsGBox final: public QGroupBox {
        Q_OBJECT

    private:
        QComboBox *copyFromScheme = nullptr;
        QComboBox *copyFromMode = nullptr;
        QComboBox *copyToScheme = nullptr;
        QComboBox *copyToMode = nullptr;
        QPushButton *copyBtn = nullptr;

    public:
        CopySchemeSettingsGBox();

        void clearWidget() const;
        void addScheme(const QString &label) const;
        void removeScheme(const QString &label) const;

    private slots:
        void onCopySettingsClicked();

    signals:
        void copySettings(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode);
    };
}
