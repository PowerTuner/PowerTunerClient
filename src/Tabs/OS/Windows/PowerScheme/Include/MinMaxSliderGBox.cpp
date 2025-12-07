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
#include "MinMaxSliderGBox.h"

namespace PWT::UI::WIN {
	MinMaxSliderGBox::MinMaxSliderGBox(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid): PowerSettingGBox(data.name, data.groupTitle, guid) {
		QLabel *descr = new QLabel(data.description);

		slider = new SliderUnitWidget(data.valueUnit, [](QLabel *unitV, const int v) { unitV->setNum(v); }, false);

		descr->setWordWrap(true);
		setLimits(data);

		lyt->addWidget(descr);
		lyt->addWidget(slider);

		QObject::connect(slider, &SliderUnitWidget::valueChanged, this, &MinMaxSliderGBox::onSliderChanged);
	}

	int MinMaxSliderGBox::getPageStep(const int valueMax) const {
		if (valueMax >= 1000)
			return 100;
		else if (valueMax >= 100)
			return 10;

		return 5;
	}

    void MinMaxSliderGBox::setLimits(const PWTS::WIN::PowerSchemeSettingData &data) const {
        slider->setRange(data.range.min, data.range.max);
        slider->setSingleStep(data.valueIncrement);
        slider->setPageStep(getPageStep(data.range.max));
    }

	void MinMaxSliderGBox::onSliderChanged(const int v) {
		onSettingValueChanged(v);
	}

	void MinMaxSliderGBox::onModeChanged(const int idx) const {
		const QSignalBlocker sblock {slider};

		if (idx == 0)
			slider->setValue(settings.value(currentScheme).ac);
		else
			slider->setValue(settings.value(currentScheme).dc);
	}
}
