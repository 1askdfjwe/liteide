/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011-2019 LiteIDE. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: tooldockwidget.cpp
// Creator: visualfc <visualfc@gmail.com>

#include "tooldockwidget.h"
#include "liteapi/liteapi.h"
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QHBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include <QMenu>
#include <QToolButton>
#include <QDebug>
//lite_memory_check_begin
#if defined(WIN32) && defined(_MSC_VER) &&  defined(_DEBUG)
     #define _CRTDBG_MAP_ALLOC
     #include <stdlib.h>
     #include <crtdbg.h>
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
#endif
//lite_memory_check_end

BaseDockWidget::BaseDockWidget(QSize iconSize, QWidget *parent) :
    QDockWidget(parent), current(0)
{
    m_mainWidget = new QWidget;
    m_widget = 0;
    QDockWidget::setWidget(m_mainWidget);
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(1);
    m_mainWidget->setLayout(m_mainLayout);

    m_comboBox = new QComboBox;
    m_comboBox->setMinimumContentsLength(4);
    //m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //m_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    m_toolBar = new QToolBar(this);
    m_toolBar->setContentsMargins(0, 0, 0, 0);
    m_toolBar->setIconSize(iconSize);
    //m_toolBar->setFixedHeight(24);
//    m_titleLabel = new QLabel;
//    m_titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_comboBoxAct = m_toolBar->addWidget(m_comboBox);
//    m_titleLabelAct = m_toolBar->addWidget(m_titleLabel);
//    m_titleLabelAct->setChecked(false);

    m_spacer = new QWidget;
    m_spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_toolBar->addSeparator();
    m_spacerAct = m_toolBar->addWidget(m_spacer);

    m_closeAct = new QAction(tr("Hide"), m_toolBar);
    m_closeAct->setToolTip(tr("Hide Tool Window"));
    m_closeAct->setIcon(QIcon("icon:images/closetool.png"));
    m_toolBar->addAction(m_closeAct);

    connect(m_closeAct,SIGNAL(triggered()),this,SLOT(close()));
    connect(m_comboBox,SIGNAL(activated(int)),this,SLOT(activeComboBoxIndex(int)));

    this->setTitleBarWidget(m_toolBar);

    m_floatAct = new QAction(tr("Floating Window"),this);
    m_floatAct->setCheckable(true);
    connect(m_floatAct,SIGNAL(triggered(bool)),this,SLOT(setFloatingWindow(bool)));
    connect(this,SIGNAL(topLevelChanged(bool)),this,SLOT(topLevelChanged(bool)));
    /*
    m_toolBar->setStyleSheet("QToolBar {border: 1px ; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #eeeeee, stop: 1 #ababab); }"\
                             "QToolBar QToolButton { border:1px ; border-radius: 1px; }"\
                             "QToolBar QToolButton::hover { background-color: #ababab;}"\
                             "QToolBar::separator {width:2px; margin-left:2px; margin-right:2px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dedede, stop: 1 #a0a0a0);}");
*/
}

void BaseDockWidget::setWindowTitle(const QString &text)
{
    QDockWidget::setWindowTitle(text);
}

QAction *BaseDockWidget::addWidget(QWidget *widget)
{
   return m_toolBar->insertWidget(m_closeAct,widget);
}

void BaseDockWidget::setToolMenu(QMenu *menu)
{
    QToolButton *btn = new QToolButton;
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setMenu(menu);
    btn->setStyleSheet("QToolButton::menu-indicator{image:none;}");

    m_toolBar->insertWidget(m_closeAct,btn);
}

void BaseDockWidget::setWidgetActions(QList<QAction*> actions)
{
    foreach(QAction *action, m_widgetActions) {
        m_toolBar->removeAction(action);
    }
    m_widgetActions = actions;
    //m_spacerAct->setVisible(!m_widgetActions.isEmpty());
    foreach(QAction *action, m_widgetActions) {
        m_toolBar->insertAction(m_spacerAct,action);
        if (action->menu()) {
            QWidget *w = m_toolBar->widgetForAction(action);
            QToolButton *btn = qobject_cast<QToolButton*>(w);
            if (btn) {
                btn->setPopupMode(QToolButton::InstantPopup);
                btn->setStyleSheet("QToolButton::menu-indicator{image:none;}");
            }
        }
    }
}

void BaseDockWidget::setWidgetList(QList<QWidget *> widgets)
{
    foreach(QAction *action, m_widgetActions) {
        m_toolBar->removeAction(action);
    }
    m_widgetActions.clear();
    m_widgetList = widgets;
    //m_spacerAct->setVisible(!m_widgetActions.isEmpty());
    foreach(QWidget *widget, m_widgetList) {
        QAction *action = m_toolBar->insertWidget(m_spacerAct,widget);
        m_widgetActions.push_back(action);
        if (action->menu()) {
            QWidget *w = m_toolBar->widgetForAction(action);
            QToolButton *btn = qobject_cast<QToolButton*>(w);
            if (btn) {
                btn->setPopupMode(QToolButton::InstantPopup);
                btn->setStyleSheet("QToolButton::menu-indicator{image:none;}");
            }
        }
    }
}

QList<QAction *> BaseDockWidget::actions() const
{
    return m_actions;
}

void BaseDockWidget::removeAction(QAction *action)
{
    if (m_actions.removeAll(action)) {
        if (action == current)
            current = 0;
        int index = m_comboBox->findData(action->objectName());
        if (index >= 0) {
            m_comboBox->removeItem(index);
        }
        QObject::disconnect(action, SIGNAL(changed()), this, SLOT(actionChanged()));
    }
}

void BaseDockWidget::setWidget(QWidget *widget)
{
    if (m_widget) {
        m_mainLayout->removeWidget(m_widget);
    }
    m_widget = widget;
    if (m_widget) {
        m_mainLayout->addWidget(m_widget);
    }
}

QWidget *BaseDockWidget::widget() const
{
    return m_widget;
}

QAction * BaseDockWidget::checkedAction () const
{
    return current;
}

void BaseDockWidget::actionChanged()
{
    QAction *action = qobject_cast<QAction*>(sender());
    Q_ASSERT_X(action != 0, "ActionGroup::actionChanged", "internal error");
    if (action->isChecked()) {
        if (action != current) {
            if(current)
                current->setChecked(false);
            current = action;
            int index = m_comboBox->findData(action->objectName());
            if (index >= 0) {
                m_comboBox->setCurrentIndex(index);
                //m_titleLabel->setText(m_comboBox->currentText());
            }
        }
    } else if (action == current) {
        current = 0;
    }
}

void BaseDockWidget::activeComboBoxIndex(int index)
{
    if (index < 0 || index >= m_comboBox->count()) {
        return;
    }
    QString objName = m_comboBox->itemData(index).toString();
    foreach(QAction *act, m_actions) {
        if (act->objectName() == objName) {
            if (!act->isChecked()) {
                act->setChecked(true);
            }
            break;
        }
    }
}

void BaseDockWidget::topLevelChanged(bool b)
{
   // m_comboBoxAct->setVisible(!b);
//    m_titleLabel->setText(m_comboBox->currentText());
//    m_titleLabelAct->setVisible(b);
    m_closeAct->setVisible(!b);
    DockWidgetFeatures flags = this->features();
    if (b) {
        this->setTitleBarWidget(0);
        m_mainLayout->insertWidget(0,m_toolBar);
        m_mainLayout->setMargin(2);
        m_toolBar->setVisible(true);
        flags |= QDockWidget::DockWidgetFloatable;
    } else {
        m_mainLayout->setMargin(0);
        m_toolBar->setVisible(false);
        m_mainLayout->removeWidget(m_toolBar);
        this->setTitleBarWidget(m_toolBar);
        flags &= (~QDockWidget::DockWidgetFloatable);
    }
    this->setFeatures(flags);
    m_floatAct->setChecked(b);
}

void BaseDockWidget::setFloatingWindow(bool b)
{
    if (this->isFloating() != b) {
        DockWidgetFeatures flags = this->features();
        if (b) {
            flags |= QDockWidget::DockWidgetFloatable;
        } else {
            flags &= (~QDockWidget::DockWidgetFloatable);
        }
        this->setFeatures(flags);
        this->setFloating(b);
    }
    if (m_floatAct->isChecked() != b) {
        m_floatAct->setChecked(b);
    }
}

void BaseDockWidget::addAction(QAction *action, const QString &title)
{
    if(!m_actions.contains(action)) {
        m_actions.append(action);
        m_comboBox->addItem(title,action->objectName());
        QObject::connect(action, SIGNAL(changed()), this, SLOT(actionChanged()));
    }
    if (current && current->isChecked()) {
        current->setChecked(false);
    }
    if (action->isChecked()) {
        current = action;
    }
}

SplitDockWidget::SplitDockWidget(QSize iconSize, QWidget *parent) :
    BaseDockWidget(iconSize,parent)
{
}

void SplitDockWidget::createMenu(Qt::DockWidgetArea area, bool split)
{
    m_area = area;
    QMenu *moveMenu = new QMenu(tr("Move To"),this);
    if (area != Qt::TopDockWidgetArea) {
        QAction *act = new QAction(tr("Top"),this);
        act->setData(Qt::TopDockWidgetArea);
        moveMenu->addAction(act);
        connect(act,SIGNAL(triggered()),this,SLOT(moveAction()));
        QAction *act1 = new QAction(tr("Top (Split)"),this);
        act1->setData(Qt::TopDockWidgetArea);
        moveMenu->addAction(act1);
        connect(act1,SIGNAL(triggered()),this,SLOT(moveActionSplit()));
    }
    if (area != Qt::BottomDockWidgetArea) {
        QAction *act = new QAction(tr("Bottom"),this);
        act->setData(Qt::BottomDockWidgetArea);
        moveMenu->addAction(act);
        connect(act,SIGNAL(triggered()),this,SLOT(moveAction()));
        QAction *act1 = new QAction(tr("Bottom (Split)"),this);
        act1->setData(Qt::BottomDockWidgetArea);
        moveMenu->addAction(act1);
        connect(act1,SIGNAL(triggered()),this,SLOT(moveActionSplit()));
    }
    if (area != Qt::LeftDockWidgetArea) {
        QAction *act = new QAction(tr("Left"),this);
        act->setData(Qt::LeftDockWidgetArea);
        moveMenu->addAction(act);
        connect(act,SIGNAL(triggered()),this,SLOT(moveAction()));
        QAction *act1 = new QAction(tr("Left (Split)"),this);
        act1->setData(Qt::LeftDockWidgetArea);
        moveMenu->addAction(act1);
        connect(act1,SIGNAL(triggered()),this,SLOT(moveActionSplit()));
    }
    if (area != Qt::RightDockWidgetArea) {
        QAction *act = new QAction(tr("Right"),this);
        act->setData(Qt::RightDockWidgetArea);
        moveMenu->addAction(act);
        connect(act,SIGNAL(triggered()),this,SLOT(moveAction()));
        QAction *act1 = new QAction(tr("Right (Split)"),this);
        act1->setData(Qt::RightDockWidgetArea);
        moveMenu->addAction(act1);
        connect(act1,SIGNAL(triggered()),this,SLOT(moveActionSplit()));
    }

    if (area == Qt::TopDockWidgetArea) {
        m_areaInfo = split ? tr("TopDockWidget (Split)") : tr("TopDockWidget");
    } else if (area == Qt::BottomDockWidgetArea) {
        m_areaInfo = split ? tr("BottomDockWidget (Split)") : tr("BottomDockWidget");
    } else if (area == Qt::LeftDockWidgetArea) {
        m_areaInfo = split ? tr("LeftDockWidget (Split)") : tr("LeftDockWidget");
    } else if (area == Qt::RightDockWidgetArea) {
        m_areaInfo = split ? tr("RightDockWidget (Split)") : tr("RightDockWidget");
    }

    QMenu *menu = new QMenu(this);
    menu->addAction(m_floatAct);
    if (split) {
        QAction *unsplitAct = new QAction(tr("Unsplit"),this);
        unsplitAct->setData(area);
        connect(unsplitAct,SIGNAL(triggered()),this,SLOT(unsplitAction()));
        menu->addAction(unsplitAct);
    } else {
        QAction *splitAct = new QAction(tr("Split"),this);
        splitAct->setData(area);
        connect(splitAct,SIGNAL(triggered()),this,SLOT(splitAction()));
        menu->addAction(splitAct);
    }
    menu->addAction(moveMenu->menuAction());

    if (area == Qt::BottomDockWidgetArea || area == Qt::TopDockWidgetArea) {
        m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        m_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        QWidget *spacer = new QWidget;
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_toolBar->insertWidget(m_closeAct,spacer);
    }

    QToolButton *btn = new QToolButton(m_toolBar);
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setIcon(QIcon("icon:images/movemenu.png"));
    btn->setMenu(menu);
    btn->setText(tr("Move To"));
    btn->setToolTip(tr("Move To"));
    btn->setStyleSheet("QToolButton::menu-indicator {image: none;}");
    m_toolBar->insertWidget(m_closeAct,btn);
}

void SplitDockWidget::setWindowTitle(const QString &text)
{
    BaseDockWidget::setWindowTitle(m_areaInfo+"  -  "+text);
}

void SplitDockWidget::moveAction()
{
    QAction *action = static_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    Qt::DockWidgetArea area = (Qt::DockWidgetArea)action->data().toInt();
    emit moveActionTo(m_area,area,current,false);
}

void SplitDockWidget::moveActionSplit()
{
    QAction *action = static_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    Qt::DockWidgetArea area = (Qt::DockWidgetArea)action->data().toInt();
    emit moveActionTo(m_area,area,current,true);
}

void SplitDockWidget::splitAction()
{
    QAction *action = static_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    Qt::DockWidgetArea area = (Qt::DockWidgetArea)action->data().toInt();
    emit moveActionTo(m_area,area,current,true);
}

void SplitDockWidget::unsplitAction()
{
    QAction *action = static_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    Qt::DockWidgetArea area = (Qt::DockWidgetArea)action->data().toInt();
    emit moveActionTo(m_area,area,current,false);
}


OutputDockWidget::OutputDockWidget(QSize iconSize, QWidget *parent) :
    BaseDockWidget(iconSize,parent)
{
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_toolBar->insertWidget(m_closeAct,spacer);
}

void OutputDockWidget::createMenu(Qt::DockWidgetArea area)
{    
    this->setAllowedAreas(area);
    QMenu *moveMenu = new QMenu(tr("Move To"),this);

    QAction *leftAct = new QAction(tr("LeftSideBar"),this);
    QAction *rightAct = new QAction(tr("RightSideBar"),this);

    leftAct->setData(Qt::LeftDockWidgetArea);
    rightAct->setData(Qt::RightDockWidgetArea);

    moveMenu->addAction(leftAct);
    moveMenu->addAction(rightAct);

    connect(leftAct,SIGNAL(triggered()),this,SLOT(moveAction()));
    connect(rightAct,SIGNAL(triggered()),this,SLOT(moveAction()));

    QMenu *menu = new QMenu(this);
    menu->addAction(m_floatAct);
    menu->addAction(moveMenu->menuAction());

    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_toolBar->insertWidget(m_closeAct,spacer);

    QToolButton *btn = new QToolButton(m_toolBar);
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setIcon(QIcon("icon:images/movemenu.png"));
    btn->setMenu(menu);
    btn->setText(tr("Move To"));
    btn->setToolTip(tr("Move To"));
    btn->setStyleSheet("QToolButton::menu-indicator {image: none;}");
    m_toolBar->insertWidget(m_closeAct,btn);
}

void OutputDockWidget::setWindowTitle(const QString &text)
{
    BaseDockWidget::setWindowTitle(QString(tr("BottomDockWidget"))+"  -  "+text);
}

void OutputDockWidget::setWidget(QWidget *widget)
{
    if (m_widget) {
        m_mainLayout->removeWidget(m_widget);
        m_widget->setVisible(false);
    }
    m_widget = widget;
    if (m_widget) {
        m_mainLayout->addWidget(m_widget);
        m_widget->setVisible(true);
    }
}

void OutputDockWidget::moveAction()
{
    QAction *action = static_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    this->setFloatingWindow(false);
    Qt::DockWidgetArea area = (Qt::DockWidgetArea)action->data().toInt();
    emit moveActionTo(Qt::BottomDockWidgetArea,area,current);
}
