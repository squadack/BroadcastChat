#pragma once

#include <QtCore>
#include <QtGui>

class Message {
public:
	void prepareMessage(QByteArray *) const;
	bool parseMessage(const QByteArray &);
	
	const QString & getMessage() const;
	const QString & getSender() const;
	QColor getSenderColor() const;
	const QDateTime & getReceivedTime() const;
	
	void setMessage(const QString &);
	void setSender(const QString &);
	void setSenderColor(QColor);
	void setReceivedTime(const QDateTime &);
private:
	QString message;
	QString sender;
	QColor sender_color;
	QDateTime received_time;
	
};
