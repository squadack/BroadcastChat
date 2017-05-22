#include <QtCore>
#include "ChatWindow.h"

int main(int argc, char **argv)
{
	qsrand(time(nullptr));
	QApplication app{argc, argv};
	QCoreApplication::setOrganizationName("Squadack");
	QCoreApplication::setApplicationName("Broadcast Chat");
	ChatWindow window{nullptr};
	window.show();
	
	return app.exec();
}
