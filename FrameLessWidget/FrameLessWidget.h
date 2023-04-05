#pragma once

#include "framelesswidget_global.h"

#include <QWidget>

#include <qt_windows.h>

#include <dwmapi.h>
#include <windowsx.h>

namespace Ui {
class FrameLessWidgetClass;
} // namespace Ui

class FRAMELESSWIDGET_EXPORT FrameLessWidget : public QWidget {
    Q_OBJECT;

public:
    FrameLessWidget(QWidget* parent = nullptr);

    virtual ~FrameLessWidget();

protected:
    // mousePressEvent
    void mousePressEvent(QMouseEvent* event) override;

    // changeEvent
    void changeEvent(QEvent* event) override;

    // nativeEvent
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    Ui::FrameLessWidgetClass* ui = nullptr;
    /**
     * @brief 识别为拖拉边界的范围
     */
    long _borderWidth = 10;
};
