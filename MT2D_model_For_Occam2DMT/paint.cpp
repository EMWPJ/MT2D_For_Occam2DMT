#include "paint.h"
#include "ui_paint.h"
#include <QDebug>

paint::paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::paint)
{
    ui->setupUi(this);
    boundary = QRectF(-10000,10000,20000,-20000);
    origin = QPointF(0,0);
    pressed = false;
    ctrl = false;
    alt = false;
    offset = QPointF(0,0);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    OriginalMatrix.reset();
    TemporaryMatrix.reset();
    TransformMatrix.reset();
    CurrentMatrix.reset();
}

paint::~paint()
{
    delete ui;
}


void paint::paintEvent(QPaintEvent *)
{
    QPainter draw(this);
    draw.setRenderHint(QPainter::Antialiasing); //告诉绘图引擎应该在可能的情况下进行边的反锯齿绘制
    draw.setRenderHint(QPainter::SmoothPixmapTransform); //使用平滑的pixmap变换算法(双线性插值算法),而不是近邻插值算法
    draw.setRenderHint(QPainter::TextAntialiasing); //尽可能的情况下文字的反锯齿绘制
    CurrentMatrix = OriginalMatrix * TransformMatrix;
    draw.setMatrix(CurrentMatrix);
    qint32 rectCount = rects.count();
    if(rectCount>0)
    {
        for(int i=0;i<rectCount;i++)
        {
            qint32 tempCount = rects.at(i).data.count();
            if(tempCount>0)
            {
                QPen temppen = rects.at(i).pen;
                double tempd = temppen.widthF()/CurrentMatrix.m11()/4;
                draw.setBrush(rects.at(i).brush);
                temppen.setWidthF(tempd);
                draw.setPen(temppen);
                draw.drawRects(rects.at(i).data);
            }
        }
    }
    qint32 lineCount = lines.count();
    if(lineCount>0)
    {
        for(int i=0;i<lineCount;i++)
        {
            qint32 tempCount = lines.at(i).data.count();
            if(tempCount>0)
            {
                QPen temppen = lines.at(i).pen;
                double tempd = temppen.widthF()/CurrentMatrix.m11()/4;
                temppen.setWidthF(tempd);
                draw.setPen(temppen);
                draw.drawLines(lines.at(i).data);
            }
        }
    }
    qint32 pointCount = points.count();
    if(pointCount>0)
    {
        for(int i=0;i<pointCount;i++)
        {
            qint32 tempCount = points.at(i).data.count();
            if(tempCount>0)
            {
                QPen temppen = points.at(i).pen;
                double tempd = temppen.widthF()/CurrentMatrix.m11()/4;
                temppen.setWidthF(tempd);
                draw.setPen(temppen);
                draw.drawPoints(points.at(i).data);
            }
        }
    }
    if(ctrl)
    {
        if(pointA!=QPointF(999999,999999))
        {
            if(!(fabs(pointA.x()-pointB.x())<1.0))
            {
                QPen pen(Qt::blue, 2.0/CurrentMatrix.m11(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                draw.setPen(pen);
                draw.setBrush(Qt::NoBrush);
                draw.drawRect(QRectF(pointA.x(),pointA.y(),pointB.x()-pointA.x(),pointB.y()-pointA.y()));
            }
        }
    }
    //    qDebug()<<"paint"<<alt;
    if(!alt)
    {
        TemporaryMatrix.reset();
        draw.setMatrix(TemporaryMatrix);
        QPointF p = CurrentMatrix.map(currentPoint);
        QPen pen(Qt::red, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        draw.setPen(pen);
        draw.drawLine(QPointF(p.x(),p.y()+50),QPointF(p.x(),p.y()-50));
        draw.drawLine(QPointF(p.x()-50,p.y()),QPointF(p.x()+50,p.y()));
        QFont font("Times New Roman", 12, QFont::Bold);
        draw.setFont(font);
        QString coor = tr("(") + QString::number(currentPoint.x())+tr(",")+QString::number(currentPoint.y())+tr(")");
        draw.drawText(int(p.x())+20,int(p.y())-20,coor);
    }
    return;
}


void paint::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        if(ctrl)
        {
            TemporaryMatrix = CurrentMatrix.inverted();
            pointA = TemporaryMatrix.map(e->pos());
        }
        else
        {
            pressed = true;
            initialPoint = e->pos();
        }
        beforePoint = e->pos();
    }
    else if(e->button()==Qt::RightButton)
    {
        QMenu menu;
        QAction *savePic;
        QAction *copyPic;
        savePic = menu.addAction(QString::fromUtf8("保存图片"));
        copyPic = menu.addAction(QString::fromUtf8("复制图片"));
        QAction *action = menu.exec(QPoint(int(e->screenPos().x()),int(e->screenPos().y())));
        QPixmap pixmap = QWidget::grab(this->rect());
        if(action == savePic)
        {
            QString filename = QFileDialog::getSaveFileName(this, QString::fromUtf8("保存图片"),
                                                            QDir::currentPath(),
                                                            QStringLiteral("png (*.png *jpg)"));
            QFile tempfile(filename);
            if(!tempfile.exists())
            {
                tempfile.open(QIODevice::WriteOnly|QIODevice::Truncate);
                tempfile.close();
            }
            pixmap.save(filename);
        }
        else if(action == copyPic)
        {
            QClipboard *board = QApplication::clipboard();
            board->setPixmap(pixmap);
            board->destroyed();
        }
    }
    e->accept();
}

void paint::mouseReleaseEvent(QMouseEvent *e)
{
    if(ctrl)
    {
        TemporaryMatrix = CurrentMatrix.inverted();
        pointB = TemporaryMatrix.map(e->pos());
        emit sendPoint(pointA,pointB);
        //        qDebug()<<pointA<<pointB;
        pointA = QPointF(0,0);
        pointB = QPointF(0,0);
    }
    else
    {
        pressed = false;
    }
    e->accept();
}

void paint::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()  == Qt::LeftButton)
    {
        if(ctrl)
        {
            TemporaryMatrix = CurrentMatrix.inverted();
            pointB = TemporaryMatrix.map(e->pos());
        }
        else
        {
            TransformMatrix.translate((e->x() - beforePoint.x())/OriginalMatrix.m11() / TransformMatrix.m11(),
                                      (e->y() - beforePoint.y())/OriginalMatrix.m22()/TransformMatrix.m22());
            beforePoint = e->pos();
        }
    }
    TemporaryMatrix = CurrentMatrix.inverted();
    currentPoint = TemporaryMatrix.map(e->pos());
    e->accept();
    repaint();
}

void paint::mouseDoubleClickEvent(QMouseEvent *e)
{
    TemporaryMatrix = CurrentMatrix.inverted();
    pointB = TemporaryMatrix.map(e->pos());
    emit sendPoint(pointB);
    repaint();
}

void paint::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Control)
    {
        ctrl = true;
    }
    else if(e->key()==Qt::Key_Alt)
    {
        if(alt)
        {
            alt = false;
        }
        else
        {
            alt = true;
        }
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_A)
    {
        emit sendPoint(QPointF(-99999999,-99999999),QPointF(99999999,99999999));
    }
    else if (e->key() == Qt::Key_Up)
    {
        double delta = 100;
        TemporaryMatrix = CurrentMatrix.inverted();
        TemporaryMatrix.reset();
        TransformMatrix.translate(currentPoint.x(),currentPoint.y());
        TransformMatrix.scale(pow(10,delta/3600),pow(10,delta/3600));
        TransformMatrix.translate(-currentPoint.x(),-currentPoint.y());
    }
    else if(e->key()==Qt::Key_Down)
    {
        double delta = -100;
        TemporaryMatrix = CurrentMatrix.inverted();
        TemporaryMatrix.reset();
        TransformMatrix.translate(currentPoint.x(),currentPoint.y());
        TransformMatrix.scale(pow(10,delta/3600),pow(10,delta/3600));
        TransformMatrix.translate(-currentPoint.x(),-currentPoint.y());
    }
    repaint();
    return;
}

void paint::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()&Qt::Key_Control)
    {
        ctrl = false;
        pointA = QPointF(999999,999999);
        pointB = QPointF(999999,999999);
    }
    repaint();
    return;
}

void paint::wheelEvent(QWheelEvent *e)
{
    double delta = e->delta();
    TemporaryMatrix = CurrentMatrix.inverted();
    QPointF p = TemporaryMatrix.map(e->posF());
    TemporaryMatrix.reset();
    TransformMatrix.translate(p.x(),p.y());
    TransformMatrix.scale(pow(10,delta/3600),pow(10,delta/3600));
    TransformMatrix.translate(-p.x(),-p.y());
    repaint();
    e->accept();
}


void paint::setBoundary(QRectF _rect)
{
    boundary = _rect;
    TransformMatrix.reset();
    TransformMatrix.reset();
    if(fabs(boundary.width())>fabs(boundary.height()))
    {
        TransformMatrix.scale(this->width()/boundary.width(),-this->width()/boundary.width());
    }
    else
    {
        TransformMatrix.scale(-this->height()/boundary.height(),this->height()/boundary.height());
    }
    TransformMatrix.translate(-boundary.x(),-boundary.y());
    //    qDebug()<<"boundary"<<boundary<<" OriginalMatrix  "<<TransformMatrix;
    repaint();
}

void paint::setdatas(QVector < point > _points,QVector < line > _lines,QVector < rectangle > _rects)
{
    points = _points;
    lines = _lines;
    rects = _rects;
    repaint();
}

void paint::setColorMap(ColorMap _colorMap)
{
    colorMap = _colorMap;
    repaint();
}
