#include "FrameLessWidget.h"

#include "ui_FrameLessWidget.h"

#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QScreen>

FrameLessWidget::FrameLessWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FrameLessWidgetClass)
{
    ui->setupUi(this);

    // 隐藏标题栏（无边框）
    setWindowFlags(Qt::FramelessWindowHint);
    // 启用样式背景绘制
    setAttribute(Qt::WA_StyledBackground);
    // 背景透明
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover);

    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    QObject::connect(ui->pushButtonClose, &QPushButton::clicked, this, &FrameLessWidget::close);
    QObject::connect(ui->pushButtonMax, &QPushButton::clicked, this, &FrameLessWidget::showMaximized);
    QObject::connect(ui->pushButtonMin, &QPushButton::clicked, this, &FrameLessWidget::showMinimized);
    QObject::connect(ui->pushButtonRestore, &QPushButton::clicked, this, &FrameLessWidget::showNormal);

    QObject::connect(this, &QWidget::windowIconChanged, this, [this](const QIcon& icon) {
        ui->icon->setPixmap(icon.pixmap(icon.actualSize(ui->icon->size())));
    });

    QObject::connect(this, &QWidget::windowTitleChanged, this, [this](const QString& title) {
        ui->title->setText(title);
    });

    ui->pushButtonRestore->hide();
}

FrameLessWidget::~FrameLessWidget()
{
}

void FrameLessWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // 判断是否在titleBarWidget上
        if (ui->titleBarWidget->rect().contains(event->pos())) {
            // 判断是否是双击
            if (event->type() == QEvent::MouseButtonDblClick) {
                // 判断是否是最大化
                if (isMaximized()) {
                    showNormal();
                } else {
                    showMaximized();
                }
            } else {
                if (ReleaseCapture()) {
                    SendMessage(HWND(winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
                    event->ignore();
                }
            }
        }
    }

    return QWidget::mousePressEvent(event);
}

void FrameLessWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (isMaximized()) {
            ui->pushButtonRestore->show();
            ui->pushButtonMax->hide();
        } else {
            ui->pushButtonRestore->hide();
            ui->pushButtonMax->show();
        }
    }
    return QWidget::changeEvent(event);
}

bool FrameLessWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    Q_UNUSED(eventType);

    if (eventType != "windows_generic_MSG") {
        return false;
    }
    MSG* msg = reinterpret_cast<MSG*>(message);

    QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
    if (!widget) {
        return false;
    }
    switch (msg->message) {
    case WM_NCCALCSIZE: {
        *result = 0;
        return true;
    }
    case WM_NCHITTEST: {
        *result = 0;
        const LONG border_width = 5;
        RECT winrect;
        GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        auto resizeWidth = minimumWidth() != maximumWidth();
        auto resizeHeight = minimumHeight() != maximumHeight();

        if (resizeWidth) {
            // 左边
            if (x >= winrect.left && x < winrect.left + border_width) {
                *result = HTLEFT;
            }
            // 右边
            if (x < winrect.right && x >= winrect.right - border_width) {
                *result = HTRIGHT;
            }
        }
        if (resizeHeight) {
            // 底边
            if (y < winrect.bottom && y >= winrect.bottom - border_width) {
                *result = HTBOTTOM;
            }
            // top border
            if (y >= winrect.top && y < winrect.top + border_width) {
                *result = HTTOP;
            }
        }
        if (resizeWidth && resizeHeight) {
            // 左底边
            if (x >= winrect.left && x < winrect.left + border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                *result = HTBOTTOMLEFT;
            }
            // 右底边
            if (x < winrect.right && x >= winrect.right - border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                *result = HTBOTTOMRIGHT;
            }
            // 左上边
            if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
                *result = HTTOPLEFT;
            }
            // 右上边
            if (x < winrect.right && x >= winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width) {
                *result = HTTOPRIGHT;
            }
        }
        if (*result != 0)
            return true;
        break;
    }
    case WM_SIZE: {

        RECT winrect;
        GetClientRect(msg->hwnd, &winrect);

        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(msg->hwnd, &wp);
        if (this) {
            if (wp.showCmd == SW_MAXIMIZE) {
                ::SetWindowPos(reinterpret_cast<HWND>(winId()), Q_NULLPTR, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            }
        }
    }
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void FrameLessWidget::createShadow()
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(13);
    ui->widgetContent->setGraphicsEffect(shadowEffect);
}