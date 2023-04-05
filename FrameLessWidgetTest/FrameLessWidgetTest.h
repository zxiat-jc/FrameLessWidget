#pragma once

#include "FrameLessWidget.h"

class FrameLessWidgetTest : public FrameLessWidget
{
	Q_OBJECT;
protected:

	// nativeEvent
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
public:
	FrameLessWidgetTest(QWidget* parent = nullptr);
	~FrameLessWidgetTest();
};
