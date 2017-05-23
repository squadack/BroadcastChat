#include "ChatWindow.h"

namespace {
namespace Settings {
	const QString WindowGeometry{"windowGeometry"};
	const QString Nickname{"nickname"};
	const QString NicknameColor{"nicknameColor"};
}
}

ChatWindow::ChatWindow(QWidget *parent)
	:QMainWindow{parent}
{
	QWidget *central = new QWidget{this};
	this->setCentralWidget(central);
	QVBoxLayout *layout = new QVBoxLayout{};

	textarea = new QTextBrowser{};
	textarea->setReadOnly(true);
// 	textarea->setAcceptRichText(false);
// 	textarea->setFocusPolicy(Qt::NoFocus);
	textarea->setOpenExternalLinks(true);
	layout->addWidget(textarea);

	QHBoxLayout *footer = new QHBoxLayout{};

	QLabel *nicklabel = new QLabel{};
	connect(this, &ChatWindow::nickChanged, nicklabel, &QLabel::setText);
	footer->addWidget(nicklabel);
	footer->addWidget(new QLabel{":"});

	lineedit = new QLineEdit{};
	lineedit->setPlaceholderText(tr("Type message..."));
	connect(lineedit, &QLineEdit::returnPressed, this, &ChatWindow::broadcastMessage);
	footer->addWidget(lineedit);

	QPushButton *sendbutton = new QPushButton{};
	sendbutton->setText(tr("Send"));
	sendbutton->setFocusPolicy(Qt::NoFocus);
	connect(sendbutton, &QPushButton::clicked, this, &ChatWindow::broadcastMessage);
	footer->addWidget(sendbutton);

	layout->addLayout(footer);

	QAction *quitAction = new QAction{tr("Quit"), this};
	quitAction->setShortcut(QKeySequence::Quit);
	connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

	QAction *setNickAction = new QAction{tr("Set nickname")};
	connect(setNickAction, &QAction::triggered, this, &ChatWindow::setNickname);

	QAction *setNickColorAction = new QAction{tr("Select nick color")};
	connect(setNickColorAction, &QAction::triggered, this, &ChatWindow::nickColorDialog);

	QMenu *menuMenu = this->menuBar()->addMenu(tr("Menu"));
	menuMenu->addAction(setNickAction);
	menuMenu->addAction(setNickColorAction);
	menuMenu->addAction(quitAction);

	central->setLayout(layout);

	sysicon = new QSystemTrayIcon{this};
// 	sysicon->setContextMenu(menuMenu);
	sysicon->show();

	udpsocket = new QUdpSocket{this};
	if (!udpsocket->bind(PORT_NR, QUdpSocket::ShareAddress))
		qFatal("Can't bind to port number %u", PORT_NR);

	connect(udpsocket, &QUdpSocket::readyRead, this, &ChatWindow::processPendingDatagrams);

	readSettings();

	//TODO placeholder
	_showDate = true;
	_showTime = true;
	_colorNicks = true;
}

void ChatWindow::setNickname()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Set nickname"), tr("Nickname:"), QLineEdit::Normal, nickname, &ok);
	text = text.simplified();
	if (ok && !text.isEmpty() && text != nickname)
	{
		nickname = text;
		QSettings settings;
		settings.setValue(Settings::Nickname, nickname);
		emit nickChanged(QString{"<span style=\"color:%1\">%2</span>"}
				.arg(nickcolor.name())
				.arg(nickname));
	}
}

void ChatWindow::nickColorDialog()
{
	QColor c = QColorDialog::getColor(nickcolor, this, tr("Select nick color"));
	if (c.isValid())
		setNickColor(c);
}

QColor ChatWindow::getRandomColor()
{
	static const int MOD = ((1 << 8) - 1);
	return QColor{qrand() & MOD, qrand() & MOD, qrand() & MOD};
}

void ChatWindow::setNickColor(QColor c)
{
	nickcolor = c;
	QSettings settings;
	settings.setValue(Settings::NicknameColor, nickcolor);
	emit nickChanged(QString{"<span style=\"color:%1\">%2</span>"}
				.arg(nickcolor.name())
				.arg(nickname));
}

void ChatWindow::printMessage(const Message &m)
{
	QString line;
	
	if (_showDate || _showTime)
	{
		QString format;
		if (_showDate)
			format = "yyyy-MM-dd ";
		if (_showTime)
			format += "HH:mm ";
		line += m.getReceivedTime().toString(format);
	}

	if (_colorNicks)
		line += "<span style=\"color:" + m.getSenderColor().name() + ";\">";
	line += m.getSender();
	if (_colorNicks) 
		line += "</span>: ";

	QStringList list = m.getMessage().split(" ");
	QUrl url;
	for (QString& str : list)
	{
		//NOTE czy QUrl wgl potrzebny?
		url.setUrl(str, QUrl::StrictMode);
		if (url.isValid() && (str.startsWith("http://") || str.startsWith("https://")))
			str = "<a href=\"" + str + "\">" + str + "</a>";
	}
	line += list.join(" ");

	textarea->append(line);
	
	if (!isActiveWindow())
	{
		sysicon->showMessage(QCoreApplication::applicationName() + " - " + tr("new message"),
			QString{"%1: %2"}.arg(m.getSender()).arg(m.getMessage()));
	}
}

void ChatWindow::processPendingDatagrams()
{
	while (udpsocket->hasPendingDatagrams())
	{
		//NOTE Qt 5.8 QNetworkDatagram
		QByteArray datagram;
		datagram.resize(udpsocket->pendingDatagramSize());
		udpsocket->readDatagram(datagram.data(), datagram.size());
		Message m;
		if (m.parseMessage(datagram))
		{
			printMessage(m);
		}
	}
}

void ChatWindow::broadcastMessage()
{
	if (!lineedit->text().isEmpty())
	{
		//NOTE Qt 5.8 QNetworkDatagram
		Message m;
		m.setSender(nickname);
		m.setSenderColor(nickcolor);
		m.setMessage(lineedit->text());
		m.setFormat(0);
		QByteArray datagram;
		m.prepareMessage(&datagram);

		udpsocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, PORT_NR);
		lineedit->clear();
	}
}

void ChatWindow::readSettings()
{
	QSettings settings;

	const QByteArray geometry = settings.value(Settings::WindowGeometry, QByteArray{}).toByteArray();
	if (geometry.isEmpty())
	{
		const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
		resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
		move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
	}
	else
	{
		restoreGeometry(geometry);
	}

	nickname = readNick(settings);
	
	QColor c = settings.value(Settings::NicknameColor, QColor{}).value<QColor>();
	if (!c.isValid())
		setNickColor(getRandomColor());
	else
		setNickColor(c);
}

QString ChatWindow::readNick(const QSettings &settings) const
{
	static const QString DefaultNick = "Unnamed";
	QString result = settings.value(Settings::Nickname, QString{}).toString();
	if (!result.isEmpty())
		return result;

	for (const char *env : {"USER", "USERNAME"})
	{
		result = qgetenv(env);
		if (!result.isEmpty())
			return result;
	}

	return DefaultNick;
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
	sysicon->hide();
	QSettings settings;
	settings.setValue(Settings::WindowGeometry, saveGeometry());
	QMainWindow::closeEvent(event);
}
