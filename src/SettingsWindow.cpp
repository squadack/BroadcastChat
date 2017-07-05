#include "SettingsWindow.h"

//TODO Move somewhere more global?
namespace Settings {
	const QString Nickname{"nickname"};
	const QString NicknameColour{"nicknameColor"};
	const QString ColorNicks{"colorNicks"};
	const QString ShowDate{"showDate"};
	const QString ShowTime{"showTime"};
}

SettingsWindow::SettingsWindow()
{
	initSettings();

	QVBoxLayout *layout = new QVBoxLayout{};
	//TODO make it pretty

	QLabel *nickLabel = new QLabel{tr("Nick") + ":"};
	layout->addWidget(nickLabel);
	nickLineEdit = new QLineEdit{_settings.value(Settings::Nickname).toString()};
	layout->addWidget(nickLineEdit);

	QPushButton *nickConfirm = new QPushButton{tr("Change")};
	nickConfirm->setFocusPolicy(Qt::NoFocus);
	connect(nickConfirm, &QPushButton::clicked, this, &SettingsWindow::saveNick);
	layout->addWidget(nickConfirm);

	QPushButton *colourButton = new QPushButton{tr("Colour")};
	colourButton->setFocusPolicy(Qt::NoFocus);
	connect(colourButton, &QPushButton::clicked, this, &SettingsWindow::nickColourDialog);
	layout->addWidget(colourButton);

	colourCheckbox = new QCheckBox{tr("Coloured nicks")};
	colourCheckbox->setChecked(_settings.value(Settings::ColorNicks).toBool());
	connect(colourCheckbox, &QCheckBox::stateChanged, this, &SettingsWindow::saveToggled);
	layout->addWidget(colourCheckbox);

	showDateCheckbox = new QCheckBox{tr("Show date")};
	showDateCheckbox->setChecked(_settings.value(Settings::ShowDate).toBool());
	connect(showDateCheckbox, &QCheckBox::stateChanged, this, &SettingsWindow::saveToggled);
	layout->addWidget(showDateCheckbox);

	showTimeCheckbox = new QCheckBox{tr("Show time")};
	showTimeCheckbox->setChecked(_settings.value(Settings::ShowTime).toBool());
	connect(showTimeCheckbox, &QCheckBox::stateChanged, this, &SettingsWindow::saveToggled);
	layout->addWidget(showTimeCheckbox);

	QPushButton *closeButton = new QPushButton{"Close"};
	connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
	layout->addWidget(closeButton);

	setLayout(layout);
}

const QSettings &SettingsWindow::settings() const
{
	return _settings;
}

void SettingsWindow::initSettings()
{
	if (!_settings.contains(Settings::NicknameColour))
		_settings.setValue(Settings::NicknameColour, getRandomColour());
	if (!_settings.contains(Settings::Nickname))
		_settings.setValue(Settings::Nickname, readNick());
}

void SettingsWindow::nickColourDialog()
{
	QColor c = QColorDialog::getColor(_settings.value(Settings::NicknameColour).value<QColor>(),
		this, tr("Select nick colour"));
	if (c.isValid())
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

void SettingsWindow::saveNick()
{
	QString text = nickLineEdit->text().simplified();
	if (!text.isEmpty() && text != _settings.value(Settings::Nickname).toString())
		_settings.setValue(Settings::Nickname, text);
	emit settingsChanged();
}

void SettingsWindow::saveToggled()
{
	_settings.setValue(Settings::ColorNicks, colourCheckbox->isChecked());
	_settings.setValue(Settings::ShowDate, showDateCheckbox->isChecked());
	_settings.setValue(Settings::ShowTime, showTimeCheckbox->isChecked());
	emit settingsChanged();
}
