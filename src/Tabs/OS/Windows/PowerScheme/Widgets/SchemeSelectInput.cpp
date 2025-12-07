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
#include <QLabel>

#include "SchemeSelectInput.h"

namespace PWT::UI::WIN {
	SchemeSelectInput::SchemeSelectInput() {
		QHBoxLayout *lyt = new QHBoxLayout();
		QLabel *label = new QLabel("Scheme:");

		schemeSelect = new QComboBox();

		schemeSelect->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		schemeSelect->setMinimumWidth(150);
		label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
		lyt->setAlignment(Qt::AlignRight);
		lyt->addWidget(label);
		lyt->addWidget(schemeSelect);

		setLayout(lyt);

		QObject::connect(schemeSelect, &QComboBox::currentTextChanged, this, &SchemeSelectInput::onSchemeChanged);
	}

	void SchemeSelectInput::clearInput() const {
		const QSignalBlocker sblock {schemeSelect};

		schemeSelect->clear();
	}

	void SchemeSelectInput::addScheme(const QString &label) const {
		const QSignalBlocker sblock {schemeSelect};

		schemeSelect->addItem(label);
	}

	void SchemeSelectInput::restoreSelectedScheme() {
		const QSignalBlocker sblock {schemeSelect};
		const int prevIdx = schemeSelect->findText(prevSelected);

		schemeSelect->setCurrentIndex(prevIdx == -1 ? 0 : prevIdx);
		onSchemeChanged(schemeSelect->currentText());
	}

	void SchemeSelectInput::onSchemeChanged(const QString &scheme) {
		emit schemeSelected(scheme);
		prevSelected = scheme;
	}
}
