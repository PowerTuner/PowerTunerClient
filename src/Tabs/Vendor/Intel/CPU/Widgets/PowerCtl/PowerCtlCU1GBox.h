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

#include "PowerCtlGBox.h"

namespace PWT::UI::INTEL {
    class PowerCtlCU1GBox final: public PowerCtlGBox {
    private:
        QCheckBox *prochot = nullptr;
        QCheckBox *c1e = nullptr;
        QCheckBox *sapmImcC2Policy = nullptr;
        QCheckBox *fastBrkSnpEn = nullptr;
        QCheckBox *powerPerformancePlatformOverride = nullptr;
        QCheckBox *disableEfficiencyOptimization = nullptr;
        QCheckBox *disableRaceToHalt = nullptr;
        QCheckBox *prochotOutputDisable = nullptr;
        QCheckBox *prochotConfigurableResponseEnable = nullptr;
        QCheckBox *vrThermAlertDisableLock = nullptr;
        QCheckBox *vrThermAlertDisable = nullptr;
        QCheckBox *ringEEDisable = nullptr;
        QCheckBox *saOptimizationDisable = nullptr;
        QCheckBox *ookDisable = nullptr;
        QCheckBox *hwpAutonomousDisable = nullptr;
        QCheckBox *cstatePrewakeDisable = nullptr;

    public:
        PowerCtlCU1GBox();

        void setData(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(const PWTS::ClientPacket &packet) const override;
    };
}
