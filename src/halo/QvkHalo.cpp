/* vokoscreenNG
 * Copyright (C) 2017-2022 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkHalo.h"
#include "QvkPushButton.h"
#include "global.h"

QvkHalo::QvkHalo()
{
}


QvkHalo::~QvkHalo()
{
}


void QvkHalo::init( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    createHaloPreviewWidget();
    vkHaloWindow = new QvkHaloWindow( this );
    createSpezialSlider();
    createColorButtons();
    createSpezialCheckBox();

    setToolButtonDefaultValues();

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 40 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mytimer() ) );
}


void QvkHalo::createHaloPreviewWidget()
{
    vkHaloPreviewWidget = new QvkHaloPreviewWidget( );
    ui->horizontalLayout_61->insertWidget( 0, vkHaloPreviewWidget );
    vkHaloPreviewWidget->setObjectName( "widgetHaloPreview" );
    vkHaloPreviewWidget->show();
}


void QvkHalo::createSpezialSlider()
{
    vkSpezialSliderHole = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutHaloHole->insertWidget( 0, vkSpezialSliderHole );
    vkSpezialSliderHole->setObjectName( "sliderhaloHole" );
    vkSpezialSliderHole->setTracking( true );
    vkSpezialSliderHole->setMinimum( 20 );
    vkSpezialSliderHole->setMaximum( 40 );
    vkSpezialSliderHole->setValue( 30 ); // This value must be greater than the holeDefault in order to be changed after the connect
    vkSpezialSliderHole->setShowValue( false );
    vkSpezialSliderHole->show();
    vkSpezialSliderHole->setBigHandel( false);
    connect( vkSpezialSliderHole, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Hole( int ) ) );
    vkSpezialSliderHole->setValue( holeDefault );

    vkSpezialSliderDiameter = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutDiameter->insertWidget( 0, vkSpezialSliderDiameter );
    vkSpezialSliderDiameter->setObjectName( "sliderhaloDiameter" );
    vkSpezialSliderDiameter->setTracking( true );
    vkSpezialSliderDiameter->setMinimum( 50 );
    vkSpezialSliderDiameter->setMaximum( 90 );
    vkSpezialSliderDiameter->setValue( 0 );
    vkSpezialSliderDiameter->setShowValue( false );
    vkSpezialSliderDiameter->show();
    vkSpezialSliderDiameter->setBigHandel( false );
    connect( vkSpezialSliderDiameter, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Diameter( int ) ) );
    vkSpezialSliderDiameter->setValue( diameterDefault );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutHaloOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderhaloOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 0 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    vkSpezialSliderOpacity->setBigHandel( false );
    connect( vkSpezialSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Opacity( int ) ) );
    vkSpezialSliderOpacity->setValue( opacityDefault );
}


void QvkHalo::slot_valueChanged_SpezialSlider_Diameter( int value )
{
    vkHaloPreviewWidget->setDiameter( value );
    vkHaloWindow->setDiameter( value );

    vkSpezialSliderHole->setMaximum( vkSpezialSliderDiameter->value()/2  );
}


void QvkHalo::slot_valueChanged_SpezialSlider_Hole( int value )
{
    vkHaloWindow->setHoleRadius( value );
    vkHaloWindow->repaint();

    vkHaloPreviewWidget->setHole( value );
    vkHaloPreviewWidget->repaint();
}


void QvkHalo::slot_valueChanged_SpezialSlider_Opacity( int value )
{
    vkHaloPreviewWidget->setOpacity( (qreal)value / 100 );
    vkHaloWindow->setOpacity( (qreal)value / 100 );
}


void QvkHalo::createColorButtons()
{
    QList<QHBoxLayout *> listLayaout = ui->gridLayout_halo_color_pushButton->findChildren<QHBoxLayout *>();
    for ( int i = 0; i < listLayaout.count(); i++ )
    {
        for ( int x = 0; x < 20; x++ )
        {
            if ( listLayaout.at(i)->objectName().section( "_", 2, 2 ) == QVariant::fromValue( Qt::GlobalColor(x) ).toString() )
            {
                QvkPushButton *vkPushButton = new QvkPushButton( Qt::GlobalColor(x) );
                vkPushButton->setObjectName( "PushButton_Halo_color_" + QVariant::fromValue( Qt::GlobalColor(x) ).toString() );
                vkPushButton->setMaximumHeight( 23 );
                listLayaout.at(i)->addWidget( vkPushButton );
                connect( vkPushButton, &QPushButton::clicked, this, [=]() { vkHaloPreviewWidget->setColor( Qt::GlobalColor(x) );
                                                                            vkHaloWindow->setColor( Qt::GlobalColor(x) );
                                                                           } );
                break;
            }
        }
    }
}


void QvkHalo::createSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
    vkSpezialCheckbox->setObjectName( "spezialCheckboxHalo" );
    ui->horizontalLayout_48->insertWidget( 1, vkSpezialCheckbox );
    connect( vkSpezialCheckbox, SIGNAL( signal_clicked( bool ) ), this, SLOT( slot_haloOnOff( bool ) ) );
}


void QvkHalo::slot_haloOnOff( bool value )
{
    if ( value == true )
    {
        timer->start();
        vkHaloWindow->show();
    }

    if ( value == false )
    {
        timer->stop();
        vkHaloWindow->hide();
    }
}


void QvkHalo::slot_mytimer()
{
    if ( global::showclickCounter == 0 )
    {
        vkHaloWindow->raise();
    }

    if ( QCursor::pos() != oldPos )
    {
        vkHaloWindow->repaint();

        vkHaloWindow->resize( vkHaloWindow->screen->size().width(),
                              vkHaloWindow->screen->size().height() );

        vkHaloWindow->move( vkHaloWindow->screen->geometry().x(),
                            vkHaloWindow->screen->geometry().y() );

        oldPos = QCursor::pos();
    }
}


void QvkHalo::setToolButtonDefaultValues()
{
    connect( ui->toolButtonHaloColorDefault,    &QToolButton::clicked, this, [=]() { vkHaloPreviewWidget->setColor( colorDefault );
                                                                                     vkHaloWindow->setColor( colorDefault );
                                                                                   } );
    connect( ui->toolButtonHaloDiameterDefault, &QToolButton::clicked, this, [=]() { vkSpezialSliderDiameter->setValue( diameterDefault ); } );
    connect( ui->toolButtonHaloOpacityDefault,  &QToolButton::clicked, this, [=]() { vkSpezialSliderOpacity->setValue( opacityDefault ); } );
    connect( ui->toolButtonHaloHoleDefault,     &QToolButton::clicked, this, [=]() { vkSpezialSliderHole->setValue( holeDefault); } );

}
