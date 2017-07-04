#include "SettingsWindow.h"

namespace Settings {
	const QString Nickname{"nickname"};
	const QString NicknameColour{"nicknameColor"};
}

/* This object has no parent intentionally */
SettingsWindow::SettingsWindow()
{
	QVBoxLayout *layout = new QVBoxLayout{};
	//TODO make it pretty

	QLabel *nickLabel = new QLabel{"Nick:"};
	layout->addWidget(nickLabel);
	QLineEdit *nickLineEdit = new QLineEdit{_settings.value(Settings::Nickname).toString()};
	layout->addWidget(nickLineEdit);
	QPushButton *nickConfirm = new QPushButton{"Change"};
	layout->addWidget(nickConfirm);

	QPushButton *colourLabel = new QPushButton{"Colour"};
	connect(colourLabel, &QPushButton::clicked, this, &SettingsWindow::nickColourDialog);
	layout->addWidget(colourLabel);

	QPushButton *closeButton = new QPushButton{"Close"};
	connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
	layout->addWidget(closeButton);

	setLayout(layout);
}

void SettingsWindow::setSettings(const QSettings &settings)
{
	for (s : {Settings::Nickname, Settings::NicknameColour})
		if (settings.contains(s))
			_settings.setValue(s, settings.value(s));
	if (!_settings.contains(Settings::NicknameColour))
		_settings.setValue(Settings::NicknameColour, getRandomColour());
	if (!_settings.contains(Settings::Nickname))
		_settings.setValue(Settings::Nickname, readNick());
}

void SettingsWindow::nickColourDialog()
{
	QColor c = QColorDialog::getColor(_settings.value(Settings::NicknameColour).toString(),
		this, tr("Select nick colour"));
	setNickColour(c);
}

QColor SettingsWindow::getRandomColour() const
{
	static const int MOD = ((1 << 8) - 1);
	return QColor{qrand() & MOD, qrand() & MOD, qrand() & MOD};
}

void SettingsWindow::setNickColour(const QColor &c)
{
	_settings.setValue(Settings::NicknameColour, c);
	emit settingsChanged();
}

QString SettingsWindow::readNick() const
{
	static const QString DefaultNick = "Unnamed";
	QString result = _settings.value(Settings::Nickname, QString{}).toString();
	if (!result.isEmpty())
		return result;

	for (const char *env : {"USER", "USERNAME"}) {
		result = qgetenv(env);
		if (!result.isEmpty())
			return result;
	}
	return DefaultNick;
}
