#include "Message.h"

void Message::prepareMessage(QByteArray *datagram) const
{
	QDataStream out{datagram, QIODevice::WriteOnly};
	out << sender;
	out << sender_color;
// 	Message::Format::Int flags = format;
	out << format;
	out << message;
}

bool Message::parseMessage(const QByteArray &datagram)
{
	QDataStream in{datagram};
	Message::Format::Int flags;
	in >> sender >> sender_color >> flags >> message;
	format = Message::Format(flags);
	received_time = QDateTime::currentDateTime();
	if (in.status() != QDataStream::Ok)
		return false;
	return true;
}

const QString & Message::getMessage() const
{
	return message;
}

const QString & Message::getSender() const
{
	return sender;
}

QColor Message::getSenderColor() const
{
	return sender_color;
}

const QDateTime & Message::getReceivedTime() const
{
	return received_time;
}

Message::Format Message::getFormat() const 
{
	return format;
}

void Message::setMessage(const QString &msg)
{
	message = msg;
}

void Message::setSender(const QString &s)
{
	sender = s;
}

void Message::setSenderColor(QColor c)
{
	sender_color = c;
}

void Message::setReceivedTime(const QDateTime &rt)
{
	received_time = rt;
}

void Message::setFormat(Message::Format f)
{
	format = f;
}
