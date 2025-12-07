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

#include "PowerSettingGBox.h"
#include "pwtShared/Include/OS/Windows/PowerSchemeSettingData.h"

namespace PWT::UI::WIN {
	class OptionsGBox final: public PowerSettingGBox {
		Q_OBJECT

	private:
		QComboBox *options = nullptr;

	protected:
		void onModeChanged(int idx) const override;

	public:
		OptionsGBox(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid);

        void setOptions(const QList<QString> &optionList) const;
	};
}
