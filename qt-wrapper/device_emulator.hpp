#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <utility>
#include <QDebug>
#include <QMouseEvent>
#include <queue>
#include "mali-gui/Device.hpp"
#include "mali-gui/Point.hpp"

/**
 * @brief The DeviceEmulator class creates a generic software device that can
 * be used for easier development, as it allows application to run directly on
 * the host machine. It supports output via Gui window and input via mouse
 * clicking. It is implemented as a very thin wrapper arround the Qt's QWidget
 * class.
 */
class DeviceEmulator : public maligui::Device<QColor>, public QWidget
{
public:
    DeviceEmulator(int w, int h) :
        maligui::Device<QColor>(w, h),
        QWidget(nullptr),
        mPixmap(w, h),
        mPainter(&mPixmap)
    {
        setGeometry(0, 0, w, h);
        show();
    }

    virtual void paintEvent(QPaintEvent *event) override
    {
        (void)event;
        QPainter painter(this);
        painter.drawPixmap(0, 0, mPixmap);
    }

    void setXY(int x, int y, QColor color) override
    {
        mPainter.setPen(color);
        mPainter.drawPoint(x, y);
        update();
    }

    QColor get(int offset) const override {
        (void)offset;
        return QColor();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        onPress({event->x(), event->y()});
    }

private:

    std::queue<std::pair<QPoint, QColor>> renderQueue;

    QPixmap mPixmap;

    QPainter mPainter;

};

#endif // DEVICE_H
