#include "Console.h"
#include "ui_Console.h"
#include <QDebug>

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    protocol = Protocol::instance();
    //connect(protocol, SIGNAL(serialOpened()), this, SLOT(serialOpened()));
}

Console::~Console()
{
    delete ui;
}

void Console::serialOpened()
{
    connect(protocol, SIGNAL(datasReceive(QByteArray*)), this, SLOT(datasReceive(QByteArray*)));
    //connect(protocol, SIGNAL(receiveByte(unsigned char)), this, SLOT(receiveByte(unsigned char)));
}

void Console::datasReceive(QByteArray *datas)
{
    QString str;

    for (int i=0; i<datas->length(); i++) {

        switch(i) {
            case 0:
                str.append( "<b>CMD:</b> <span style=\"color:#F00\">" );
                str.append( "0x" );
                str.append( QString::number((unsigned char)datas->at(i),16) );
                str.append( "</span> | " );
                break;
            case 1:
                str.append( "<b>LENGTH:</b> <span style=\"color:#0F0\">" );
                str.append( "0x" );
                str.append( QString::number((unsigned char)datas->at(i),16) );
                str.append( "</span> | <b style=\"color:#00F\">DATAS</b> : " );
                break;
            default:
                str.append( "0x" );
                str.append( QString::number((unsigned char)datas->at(i),16) );
                str.append( ", " );
                break;
        }


    }

    ui->consoleText->appendHtml( str );
    ui->consoleText->setMaximumBlockCount(50);


}

void Console::receiveByte(const unsigned char byte)
{
    ui->consoleText->appendPlainText( QString::number(byte,10) );
}

void Console::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    connect(protocol, SIGNAL(datasReceive(QByteArray*)), this, SLOT(datasReceive(QByteArray*)));
}

void Console::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);

    disconnect(protocol, SIGNAL(datasReceive(QByteArray*)), this, SLOT(datasReceive(QByteArray*)));
}
