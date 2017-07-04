#include <QtCore>
#include "ChatWindow.h"


int main(int argc, char **argv)
{
	qsrand(time(nullptr));
	QApplication app{argc, argv};
	QCoreApplication::setOrganizationName("Squadack");
	QCoreApplication::setApplicationName("Broadcast Chat");

	QCommandLineParser parser;
	parser.setApplicationDescription("Broadcast Chat");
	parser.addHelpOption();

	QCommandLineOption optionShowTime{
		QStringList{"t", "time"},
		QObject::tr("Shows time.")
	};
	parser.addOption(optionShowTime);

	QCommandLineOption optionShowColours{
		QStringList{"c", "colours"},
		QObject::tr("Shows colours.")
	};
	parser.addOption(optionShowColours);

	parser.process(app);

	if (parser.isSet(optionShowTime))
		qDebug() << "option show time: SET!";
	if (parser.isSet(optionShowColours))
		qDebug() << "option show colours: SET!";

	//TODO set options above as default in qsettings

	ChatWindow window{nullptr};
	window.show();

	return app.exec();
}
