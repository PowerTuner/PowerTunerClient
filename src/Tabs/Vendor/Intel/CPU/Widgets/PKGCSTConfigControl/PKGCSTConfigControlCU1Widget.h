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
#include <QCheckBox>

#include "PKGCSTConfigControlWidget.h"

namespace PWT::UI::INTEL {
    class PKGCSTConfigControlCU1Widget final: public PKGCSTConfigControlWidget {
        Q_OBJECT

    private:
        QComboBox *pkgCStateLimit = nullptr;
        QComboBox *maxCoreCState = nullptr;
        QCheckBox *ioMwaitRedirectionEnable = nullptr;
        QCheckBox *cfgLock = nullptr;
        QCheckBox *c3StateAutodemotionEnable = nullptr;
        QCheckBox *c1StateAutodemotionEnable = nullptr;
        QCheckBox *c3UndemotionEnable = nullptr;
        QCheckBox *c1UndemotionEnable = nullptr;
        QCheckBox *pkgcAutodemotionEnable = nullptr;
        QCheckBox *pkgcUndemotionEnable = nullptr;
        QCheckBox *timedMwaitEnable = nullptr;

        void applyCFGLock(bool biosLock) const;

    public:
        PKGCSTConfigControlCU1Widget();

        void setData(const PWTS::Intel::PkgCstConfigControl &data, bool isBiosLocked) const override;
        PWTS::Intel::PkgCstConfigControl getData() const override;

    private slots:
        void onCfgLockStateChanged(Qt::CheckState state) const;
    };
}
