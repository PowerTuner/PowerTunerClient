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

#include <QCheckBox>

#include "../../../../Widgets/SliderUnitWidget.h"
#include "../../../../Widgets/SliderLevelsWidget.h"
#include "pwtShared/Include/CPU/Intel/HWPRequest.h"
#include "pwtShared/Include/CPU/Intel/HWPRequestPkg.h"
#include "pwtShared/Include/CPU/Intel/HWPCapabilities.h"

namespace PWT::UI::INTEL {
    class HWPRequestWidget final: public QWidget {
        Q_OBJECT

    private:
        SliderUnitWidget *minPerf = nullptr;
        SliderUnitWidget *maxPerf = nullptr;
        SliderUnitWidget *desiredPerf = nullptr;
        QPointer<SliderLevelsWidget> epp;
        QPointer<SliderUnitWidget> acw;
        QPointer<QCheckBox> pkgControl;
        QPointer<QCheckBox> activeWindowValid;
        QPointer<QCheckBox> eppValid;
        QPointer<QCheckBox> desiredValid;
        QPointer<QCheckBox> maxValid;
        QPointer<QCheckBox> minValid;

    public:
        HWPRequestWidget(bool hasEPP, bool hasActivityWindow, bool hasPackageControl, bool hasValidBits);

        void setRequestPkgData(const PWTS::Intel::HWPRequestPkg &data) const;
        void setRequestData(const PWTS::Intel::HWPRequest &data) const;
        PWTS::Intel::HWPRequestPkg getRequestPkgData() const;
        PWTS::Intel::HWPRequest getRequestData() const;
        void setRanges(const PWTS::Intel::HWPCapabilities &caps) const;

    private slots:
        void onDesiredPerfChanged(int v) const;
        void onEppChanged(int v) const;
        void onAcwChanged(int v) const;
    };
}
