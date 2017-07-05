#pragma once

#include <QtWidgets>

class SettingsWindow : public QDialog
{
	Q_OBJECT;
public:
	SettingsWindow();
	void setSettings(const QSettings &);
	const QSettings &settings() const;
private:
	void initSettings();

	void nickColourDialog();
	QColor getRandomColour() const;
	void setNickColour(const QColor &);

	QString readNick() const;
	void saveNick();

	void saveToggled();

	QSettings _settings; //TODO remove?
	QLineEdit *nickLineEdit;
	QCheckBox *colourCheckbox;
	QCheckBox *showDateCheckbox;
	QCheckBox *showTimeCheckbox;
signals:
	void settingsChanged();
};
