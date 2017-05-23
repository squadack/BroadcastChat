#include "Message.h"

void Message::prepareMessage(QByteArray *datagram) const
{
	QDataStream out{datagram, QIODevice::WriteOnly};
	out << sender;
	out << sender_color;
	out << message;
}

bool Message::parseMessage(const QByteArray &datagram)
{
	QDataStream in{datagram};
	in >> sender >> sender_color >> message;
	if (in.status() != QDataStream::Ok)
		return false;
	return true;
}

const QString & Message::getMessage() const
{
	return message;
};

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
