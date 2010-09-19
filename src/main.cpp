#include <signal.h>
#include <QtGui>

#include "qfm.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	Qfm qfm;
	qfm.show();

	return app.exec();
}
