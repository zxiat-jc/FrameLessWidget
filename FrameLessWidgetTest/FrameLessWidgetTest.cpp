#include "FrameLessWidgetTest.h"

bool FrameLessWidgetTest::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	return FrameLessWidget::nativeEvent(eventType, message, result);
}

FrameLessWidgetTest::FrameLessWidgetTest(QWidget* parent)
	: FrameLessWidget(parent)
{
	this->setWindowTitle("标题");
	this->setWindowIcon(QIcon(":/res/img/4-squares.svg"));
}

FrameLessWidgetTest::~FrameLessWidgetTest()
{}
