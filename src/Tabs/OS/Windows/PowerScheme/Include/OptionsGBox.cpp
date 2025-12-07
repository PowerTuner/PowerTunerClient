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
#include <QLabel>

#include "OptionsGBox.h"

namespace PWT::UI::WIN {
	OptionsGBox::OptionsGBox(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid): PowerSettingGBox(data.name, data.groupTitle, guid) {
		QHBoxLayout *optionsLyt = new QHBoxLayout();
		QLabel *label = new QLabel(data.description);

		options = new QComboBox();

		options->addItems(data.options);
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		label->setAlignment(Qt::AlignLeft);
		label->setWordWrap(true);

		optionsLyt->addWidget(options);
		optionsLyt->addStretch();
		lyt->addWidget(label);
		lyt->addLayout(optionsLyt);

		QObject::connect(options, &QComboBox::currentIndexChanged, this, &OptionsGBox::onSettingValueChanged);
	}

    void OptionsGBox::setOptions(const QList<QString> &optionList) const {
        options->clear();
        options->addItems(optionList);
    }

	void OptionsGBox::onModeChanged(const int idx) const {
		const QSignalBlocker sblock {options};

		if (idx == 0)
			options->setCurrentIndex(settings.value(currentScheme).ac);
		else
			options->setCurrentIndex(settings.value(currentScheme).dc);
	}
}
