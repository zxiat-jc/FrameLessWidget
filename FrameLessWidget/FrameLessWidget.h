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

    // setCenterWidget
    void setCenterWidget(QWidget* widget);

    // centerWidget
    QWidget* centerWidget() const;

    // widgetContent
    QWidget* widgetContent() const;

    /**
     * @brief 设置菜单栏
     * @param menu QWidget*
     */
    void setMenuBar(QWidget* menu);

    /**
     * @brief 隐藏菜单栏
     */
    void hideMenuBar();

    /**
     * @brief 获取菜单栏
     * @return QWidget*
     */
    QWidget* menuBar();

    /**
     * @brief 工具栏
     * @return QWidget*
     */
    QWidget* toolBar();

    /**
     * @brief 工具栏显示
     * @param visible 是否显示
     */
    void setToolBarVisible(bool visible);

protected:
    // mousePressEvent
    void mousePressEvent(QMouseEvent* event) override;

    // changeEvent
    void changeEvent(QEvent* event) override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif

private:
    Ui::FrameLessWidgetClass* ui = nullptr;

    QWidget* _menuBar = nullptr;

    /**
     * @brief 识别为拖拉边界的范围
     */
    long _borderWidth = 10;
};
