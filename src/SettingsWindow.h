#pragma once

#include <QtWidgets>

class SettingsWindow : public QDialog
{
	Q_OBJECT;
public:
	SettingsWindow();
	void setSettings(const QSettings &);
private:
	void nickColourDialog();
	QColor getRandomColour() const;
	void setNickColour(const QColor &);
	QString readNick() const;

	QSettings _settings;
signals:
	void settingsChanged();
};
