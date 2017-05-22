#pragma once

#include <QtWidgets>
#include <QtNetwork>

class ChatWindow : public QMainWindow
{
	Q_OBJECT;
public:
	ChatWindow(QWidget *parent = nullptr);
	ChatWindow(const ChatWindow &) = delete;
	~ChatWindow() = default;
	
	ChatWindow & operator=(const ChatWindow &) = delete;

private:
	void setNickname();
	QString readNick(const QSettings &) const;
	void setNickColor(QColor);
	void nickColorDialog();
	static QColor getRandomColor();
	void processPendingDatagrams();
	//TODO message class
	bool parseDatagram(const QByteArray &, QString *, QColor *, QString *);
	void printMessage(const QString &, const QString &, QColor);
	
	void broadcastMessage();
	void readSettings();
	void closeEvent(QCloseEvent *);

	static const quint16 PORT_NR = 41829;

	QTextBrowser *textarea;
	QLineEdit *lineedit;
	QUdpSocket *udpsocket;
	QSystemTrayIcon *sysicon;
	QString nickname;
	QColor nickcolor;
	
	bool _showDate;
	bool _showTime;
	bool _colorNicks;
signals:
	void nickChanged(const QString &);
};
