#include "FrameLessWidgetTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	FrameLessWidgetTest w;
	w.show();
	return a.exec();
}
