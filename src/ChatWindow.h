#pragma once

#include <QtWidgets>
#include <QtNetwork>
#include "Message.h"
#include "SettingsWindow.h"

class ChatWindow : public QMainWindow
{
	Q_OBJECT;
public:
	ChatWindow(QWidget *parent = nullptr);
	ChatWindow(const ChatWindow &) = delete;
	~ChatWindow() = default;

	ChatWindow & operator=(const ChatWindow &) = delete;

private:
	void onSettingsChanged();
	void processPendingDatagrams();
	//TODO message class
	void onNewMessage(const Message &);
	void printMessage(const Message &);

	void broadcastMessage();
	void readSettings();
	void closeEvent(QCloseEvent *);

	static const quint16 PORT_NR = 41829;

	QTextBrowser *textarea;
	QLineEdit *lineedit;
	QUdpSocket *udpsocket;
	QSystemTrayIcon *sysicon;
	SettingsWindow settingsWindow;
	QString nickname;
	QColor nickcolor;

	bool _showDate;
	bool _showTime;
	bool _colorNicks;

	//TODO store it somewhere, load on start, save on quit
	QVector<Message> messages;

signals:
	void nickChanged(const QString &);
};
