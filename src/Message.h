#pragma once

#include <QtCore>
#include <QtGui>

class Message {
public:
	enum FormatFlag {
		BeginWithNewline = 0x0001,
		Unformatted = 0x0002
	};
	Q_DECLARE_FLAGS(Format, FormatFlag)

	void prepareMessage(QByteArray *) const;
	bool parseMessage(const QByteArray &);

	const QString & getMessage() const;
	const QString & getSender() const;
	QColor getSenderColor() const;
	const QDateTime & getReceivedTime() const;
	Message::Format getFormat() const;

	void setMessage(const QString &);
	void setSender(const QString &);
	void setSenderColor(QColor);
	void setReceivedTime(const QDateTime &);
	void setFormat(Format);
private:
	QString message;
	QString sender;
	QColor sender_color;
	QDateTime received_time;
	Message::Format format;
};

// Q_DECLARE_OPERATORS_FOR_FLAGS(Message::FormatFlag);
