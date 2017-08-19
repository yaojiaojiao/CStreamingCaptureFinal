#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ADQAPI.h"
#include <QDebug>
#include <QFont>
#include <qbuttongroup.h>
#include <memory.h>
#include <stdio.h>
#include <QDataStream>

//#define SUCCESS(f) = {if(!(f)){QMessageBox::critical(this, QString::fromStdString("提示"), QString::fromStdString("Error"));}}
bool success = true;
const unsigned int adq_num = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupadq.num_sample_skip = 128;        //设为128，省去sample_decimation

    ButtonClassify();                      //Group里RadioButton分类
    on_radioButton_default_clicked();
    Create_statusbar();

    num_of_devices = 0;
    num_of_failed = 0;
    num_of_ADQ214 = 0;
    adq_cu = CreateADQControlUnit();
    qDebug() << "adq_cu = " << adq_cu;
    connectADQDevice();

    setupadq.apirev = ADQAPI_GetRevision();
    qDebug() << IS_VALID_DLL_REVISION(setupadq.apirev);
    qDebug() << "ADQAPI Example";
    qDebug() << "API Revision:" << setupadq.apirev;
    drawLayoutCHA=ui->verticalLayout_CHA;
    drawLayoutCHB=ui->verticalLayout_CHB;

    onRadioChannels();
    onRadioTrigger();
    on_lineEdit_BufferNum_returnPressed();
    on_lineEdit_BufferSize_returnPressed();
    on_methodBox_activated(1);
    psd_res = nullptr;
}

void MainWindow::Create_statusbar()                   //创建状态栏
{
    bar = ui->statusBar;
    QFont font("Microsoft YaHei UI",10);
    bar->setFont(font);

    ADQ_state = new QLabel;
    ADQ_state->setMinimumSize(115,22);
    ADQ_state->setAlignment(Qt::AlignLeft);
    bar->addWidget(ADQ_state);
}

void MainWindow::on_actionSearch_triggered()          //search
{
    connectADQDevice();
}

void MainWindow::connectADQDevice()
{
    num_of_devices = ADQControlUnit_FindDevices(adq_cu);			//找到所有与电脑连接的ADQ，并创建一个指针列表，返回找到设备的总数
    num_of_failed = ADQControlUnit_GetFailedDeviceCount(adq_cu);
    num_of_ADQ214 = ADQControlUnit_NofADQ214(adq_cu);				//返回找到ADQ214设备的数量
    if((num_of_failed > 0)||(num_of_devices == 0))
    {
        ADQ_state->setText(QString::fromLocal8Bit("采集卡未连接"));
        isADQ214Connected = false;
    }
    else if (num_of_ADQ214 != 0)
    {
        ADQ_state->setText(QString::fromLocal8Bit("采集卡已连接"));
        isADQ214Connected = true;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (psd_res != nullptr)
        delete psd_res;
}

//默认buffers
void MainWindow::on_radioButton_default_clicked()
{
    setupadq.num_buffers = 8;
    setupadq.size_buffers = 1024;
    ui->lineEdit_BufferNum->setText(QString::number(setupadq.num_buffers));
    ui->lineEdit_BufferSize->setText(QString::number(setupadq.size_buffers/512));
}

//自定义buffers
void MainWindow::on_radioButton_customize_clicked()
{
    setupadq.num_buffers = ui->lineEdit_BufferNum->text().toInt();
    setupadq.size_buffers = ui->lineEdit_BufferSize->text().toInt()*512;
}


//进制自动转换
void MainWindow::on_lineEdit_toFPGA_0_textChanged(const QString &arg0)             //30
{
    QString lineEdit_toFPGA0x = QString::number(arg0.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_0x->setText(lineEdit_toFPGA0x);
}

void MainWindow::on_lineEdit_toFPGA_0x_textChanged(const QString &arg0x)
{
    int lineEdit_toFPGA0 = arg0x.toInt(0,16);
    ui->lineEdit_toFPGA_0->setText(QString::number(lineEdit_toFPGA0));
}

void MainWindow::on_lineEdit_toFPGA_1_textChanged(const QString &arg1)            //31
{
    QString lineEdit_toFPGA1x = QString::number(arg1.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_1x->setText(lineEdit_toFPGA1x);
}

void MainWindow::on_lineEdit_toFPGA_1x_textChanged(const QString &arg1x)
{
    int lineEdit_toFPGA1 = arg1x.toInt(0,16);
    ui->lineEdit_toFPGA_1->setText(QString::number(lineEdit_toFPGA1));
}

void MainWindow::on_lineEdit_toFPGA_2_textChanged(const QString &arg2)            //32
{
    QString lineEdit_toFPGA2x = QString::number(arg2.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_2x->setText(lineEdit_toFPGA2x);
}

void MainWindow::on_lineEdit_toFPGA_2x_textChanged(const QString &arg2x)
{
    int lineEdit_toFPGA2 = arg2x.toInt(0,16);
    ui->lineEdit_toFPGA_2->setText(QString::number(lineEdit_toFPGA2));
}

void MainWindow::on_lineEdit_toFPGA_3_textChanged(const QString &arg3)            //33
{
    QString lineEdit_toFPGA3x = QString::number(arg3.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_3x->setText(lineEdit_toFPGA3x);
}

void MainWindow::on_lineEdit_toFPGA_3x_textChanged(const QString &arg3x)
{
    int lineEdit_toFPGA3 = arg3x.toInt(0,16);
    ui->lineEdit_toFPGA_3->setText(QString::number(lineEdit_toFPGA3));
}

void MainWindow::on_lineEdit_toFPGA_4_textChanged(const QString &arg4)             //34
{
    QString lineEdit_toFPGA4x = QString::number(arg4.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_4x->setText(lineEdit_toFPGA4x);
}

void MainWindow::on_lineEdit_toFPGA_4x_textChanged(const QString &arg4x)
{
    int lineEdit_toFPGA4 = arg4x.toInt(0,16);
    ui->lineEdit_toFPGA_4->setText(QString::number(lineEdit_toFPGA4));
}

void MainWindow::on_lineEdit_toFPGA_5_textChanged(const QString &arg5)             //35
{
    QString lineEdit_toFPGA5x = QString::number(arg5.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_5x->setText(lineEdit_toFPGA5x);
}

void MainWindow::on_lineEdit_toFPGA_5x_textChanged(const QString &arg5x)
{
    int lineEdit_toFPGA5 = arg5x.toInt(0,16);
    ui->lineEdit_toFPGA_5->setText(QString::number(lineEdit_toFPGA5));
}

void MainWindow::on_lineEdit_toFPGA_6_textChanged(const QString &arg6)            //36
{
    QString lineEdit_toFPGA6x = QString::number(arg6.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_6x->setText(lineEdit_toFPGA6x);
}

void MainWindow::on_lineEdit_toFPGA_6x_textChanged(const QString &arg6x)
{
    int lineEdit_toFPGA6 = arg6x.toInt(0,16);
    ui->lineEdit_toFPGA_6->setText(QString::number(lineEdit_toFPGA6));
}

void MainWindow::on_lineEdit_toFPGA_7_textChanged(const QString &arg7)             //37
{
    QString lineEdit_toFPGA7x = QString::number(arg7.toInt(), 16).toUpper();
    ui->lineEdit_toFPGA_7x->setText(lineEdit_toFPGA7x);
}

void MainWindow::on_lineEdit_toFPGA_7x_textChanged(const QString &arg7x)
{
    int lineEdit_toFPGA7 = arg7x.toInt(0,16);
    ui->lineEdit_toFPGA_7->setText(QString::number(lineEdit_toFPGA7));
}


void MainWindow::on_pushButton_input_clicked()
{
    if(num_of_ADQ214 != 0)
    {
        read_datay0 = ADQ214_ReadAlgoRegister(adq_cu,1,0x20);    //return the read data
        qDebug() << "read_datay0 = " << read_datay0;
        ui->lineEdit_fromFPGA_0->setText(QString("%1").arg(read_datay0,0,10));
        ui->lineEdit_fromFPGA_0x->setText(QString("0x%1").arg(read_datay0,0,16).toUpper());

        read_datay1 = ADQ214_ReadAlgoRegister(adq_cu,1,0x21);
        qDebug() << "read_datay1 = " << read_datay1;
        ui->lineEdit_fromFPGA_1->setText(QString("%1").arg(read_datay1,0,10));
        ui->lineEdit_fromFPGA_1x->setText(QString("0x%1").arg(read_datay1,0,16).toUpper());

        read_datay2 = ADQ214_ReadAlgoRegister(adq_cu,1,0x22);
        qDebug() << "read_datay2 = " << read_datay2;
        ui->lineEdit_fromFPGA_2->setText(QString("%1").arg(read_datay2,0,10));
        ui->lineEdit_fromFPGA_2x->setText(QString("0x%1").arg(read_datay2,0,16).toUpper());

        read_datay3 = ADQ214_ReadAlgoRegister(adq_cu,1,0x23);
        qDebug() << "read_datay3 = " << read_datay3;
        ui->lineEdit_fromFPGA_3->setText(QString("%1").arg(read_datay3,0,10));
        ui->lineEdit_fromFPGA_3x->setText(QString("0x%1").arg(read_datay3,0,16).toUpper());

    }
    else
        qDebug() << "ADQ214 device unconnected";
}

void MainWindow::on_pushButton_output_clicked()
{
    if(num_of_ADQ214 != 0)
    {
        write_data0 = ui->lineEdit_toFPGA_0->text().toInt();
        int x0 = ADQ214_WriteAlgoRegister(adq_cu,1,0x30,0,write_data0);      //adq_cu：返回控制单元的指针
        qDebug() << "x0 = " << x0;

        write_data1 = ui->lineEdit_toFPGA_1->text().toInt();
        int x1 = ADQ214_WriteAlgoRegister(adq_cu,1,0x31,0,write_data1);      //adq_cu：返回控制单元的指针
        qDebug() << "x1 = " << x1;

        write_data2 = ui->lineEdit_toFPGA_2->text().toInt();
        int x2 = ADQ214_WriteAlgoRegister(adq_cu,1,0x32,0,write_data2);      //adq_cu：返回控制单元的指针
        qDebug() << "x2 = " << x2;

        write_data3 = ui->lineEdit_toFPGA_3->text().toInt();
        int x3 = ADQ214_WriteAlgoRegister(adq_cu,1,0x33,0,write_data3);      //adq_cu：返回控制单元的指针
        qDebug() << "x3 = " << x3;

        write_data4 = ui->lineEdit_toFPGA_4->text().toInt();
        int x4 = ADQ214_WriteAlgoRegister(adq_cu,1,0x34,0,write_data4);      //adq_cu：返回控制单元的指针
        qDebug() << "x4 = " << x4;

        write_data5 = ui->lineEdit_toFPGA_5->text().toInt();
        int x5 = ADQ214_WriteAlgoRegister(adq_cu,1,0x35,0,write_data5);      //adq_cu：返回控制单元的指针
        qDebug() << "x5 = " << x5;

        write_data6 = ui->lineEdit_toFPGA_6->text().toInt();
        int x6 = ADQ214_WriteAlgoRegister(adq_cu,1,0x36,0,write_data6);      //adq_cu：返回控制单元的指针
        qDebug() << "x6 = " << x6;

        write_data7 = ui->lineEdit_toFPGA_7->text().toInt();
        int x7 = ADQ214_WriteAlgoRegister(adq_cu,1,0x37,0,write_data7);      //adq_cu：返回控制单元的指针
        qDebug() << "x7 = " << x7;

    }
    else
        qDebug() << "ADQ214 device unconnected";
}

//Group内RadioButton分组互斥分配ID
void MainWindow::ButtonClassify()
{

    ButtonChannel = new QButtonGroup(this);
    ButtonChannel->addButton(ui->radioButton_channelA, 0);
    ButtonChannel->addButton(ui->radioButton_channelB, 1);
    ButtonChannel->addButton(ui->radioButton_channelBo, 2);

    ButtonTrigger = new QButtonGroup(this);
    ButtonTrigger->addButton(ui->radioButton_noTrig, 0);
    ButtonTrigger->addButton(ui->radioButton_softwareTrig, 1);
    ButtonTrigger->addButton(ui->radioButton_externalTrig, 2);

    connect(ui->radioButton_channelA, SIGNAL(clicked()), this, SLOT(onRadioChannels()));
    connect(ui->radioButton_channelB, SIGNAL(clicked()), this, SLOT(onRadioChannels()));
    connect(ui->radioButton_channelBo, SIGNAL(clicked()), this, SLOT(onRadioChannels()));
    connect(ui->radioButton_noTrig, SIGNAL(clicked()), this, SLOT(onRadioTrigger()));
    connect(ui->radioButton_softwareTrig, SIGNAL(clicked()), this, SLOT(onRadioTrigger()));
    connect(ui->radioButton_externalTrig, SIGNAL(clicked()), this, SLOT(onRadioTrigger()));
}

void MainWindow::onRadioChannels()
{
    setupadq.stream_ch = ButtonChannel->checkedId();
}

void MainWindow::onRadioTrigger()
{
    setupadq.trig_mode = ButtonTrigger->checkedId();
}

void MainWindow::on_lineEdit_BufferNum_returnPressed()
{
    setupadq.num_buffers = ui->lineEdit_BufferNum->text().toInt();
}

void MainWindow::on_lineEdit_BufferSize_returnPressed()
{
    setupadq.size_buffers = ui->lineEdit_BufferSize->text().toInt();
}

void MainWindow::on_methodBox_activated(int index)
{
    if(index == 1)
    {
        method = true;
    }
    else
    {
        method = false;
    }

}

void MainWindow::on_pushButton_clicked() //放大按钮
{
    if(!drawLayoutCHB->isEmpty())  //图像放大
    {
        CHB = this->drawLayoutCHB->itemAt(0)->widget();
        if(CHB->isWidgetType())
        {
            CHB->setWindowFlags(Qt::Window);
            CHB->showMaximized();
        }
    }
    else
    {
        if(!CHB)
            CHB = NULL;
        else                 //再次点击取消放大
        {
            CHB->setWindowFlags(Qt::Widget);
            drawLayoutCHB->addWidget(CHB);
        }
    }

    if(!drawLayoutCHA->isEmpty())
    {
        CHA = this->drawLayoutCHA->itemAt(0)->widget();
        if(CHA->isWidgetType())
        {
            CHA->setWindowFlags(Qt::Window);
            CHA->showMaximized();
        }
    }
    else
    {
        if(!CHA)
            CHA = NULL;
        else
        {
            CHA->setWindowFlags(Qt::Widget);
            drawLayoutCHA->addWidget(CHA);
        }
    }
}

void MainWindow::Clear_Dispaly()                   // 清除数据绘图显示
{
    while(drawLayoutCHA->count())                  // 删除布局中的所有控件
    {
        QWidget *p = this->drawLayoutCHA->itemAt(0)->widget();
        p->setParent (NULL);
        this->drawLayoutCHA->removeWidget(p);
        delete p;
    }
    while(drawLayoutCHB->count())                  // 删除布局中的所有控件
    {
        QWidget *p = this->drawLayoutCHB->itemAt(0)->widget();
        p->setParent (NULL);
        this->drawLayoutCHB->removeWidget(p);
        delete p;
    }
    CHA = NULL;
    CHB = NULL;
}


//开始采集
void MainWindow::on_pushButton_CaptureStart_clicked()
{

    if(!Config_ADQ214())
        return;
    Clear_Dispaly();

    setupadq.num_samples_collect = ui->lineEdit_SampTotNum->text().toInt();  //设置采样点数
    setupadq.data_stream_target = new qint16[setupadq.num_samples_collect];
    memset(setupadq.data_stream_target, 0, setupadq.num_samples_collect);

    if(!CaptureData2Buffer())
        return;

    //    WriteData2disk();
    WriteSpecData2disk();
    Display_Data();

    qDebug() << ("Collect finished!");
    delete setupadq.data_stream_target;
    if(success == 0)
    {
        qDebug() << "Error!";
        DeleteADQControlUnit(adq_cu);
    }
}

bool MainWindow::Config_ADQ214()                   // 配置采集卡
{
    success = false;
    if (!isADQ214Connected)
    {
        QMessageBox::critical(this, QString::fromStdString("采集卡未连接！！"), QString::fromStdString("采集卡未连接"));
    }
    else
    {
        ADQ214_SetDataFormat(adq_cu, adq_num,ADQ214_DATA_FORMAT_UNPACKED_14BIT);
        // 设置TransferBuffer大小及数量

        if(ui->radioButton_customize->isChecked())
        {
            success = ADQ214_SetTransferBuffers(adq_cu, adq_num, setupadq.num_buffers, setupadq.size_buffers);
        }
        qDebug() << "num_buffer = " << setupadq.num_buffers;
        qDebug() << "size_buffer = " << setupadq.size_buffers;

        //设置数据简化方案
        if(ADQ214_SetSampleSkip(adq_cu, adq_num, setupadq.num_sample_skip) == 0)
        {
            qDebug() << "Error";
            DeleteADQControlUnit(adq_cu);
        }


        // 设置采集通道
        qDebug() << "stream_ch=" << setupadq.stream_ch;
        switch(setupadq.stream_ch) {
        case 0:
            setupadq.stream_ch = ADQ214_STREAM_ENABLED_A;
            qDebug() << "A";
            break;
        case 1:
            setupadq.stream_ch = ADQ214_STREAM_ENABLED_B;
            qDebug() << "B";
            break;
        case 2:
            setupadq.stream_ch = ADQ214_STREAM_ENABLED_BOTH;
            qDebug() << "Bo";
            break;
        }
        // 设置触发方式：无触发、软件触发、外触发

        qDebug() << "tri_mode=" << setupadq.trig_mode;
        switch(setupadq.trig_mode)
        {
        case 0:
            setupadq.stream_ch &= 0x7;
            qDebug() << "no";
            break;
        case 1:
        {
            ADQ_SetTriggerMode(adq_cu, adq_num,setupadq.trig_mode);
            setupadq.stream_ch |= 0x8;
            qDebug() << "soft";
        }
            break;
        case 2:
        {
            ADQ_SetTriggerMode(adq_cu, adq_num,setupadq.trig_mode);
            setupadq.stream_ch |= 0x8;
            qDebug() << "exter";
        }
            break;
        }

        setupadq.clock_source = 0;            //0 = Internal clock
        success = ADQ214_SetClockSource(adq_cu, adq_num, setupadq.clock_source);

        setupadq.pll_divider = 2;            //在Internal clock=0时，设置，f_clk = 800MHz/divider
        success = success && ADQ214_SetPllFreqDivider(adq_cu, adq_num, setupadq.pll_divider);
    }
    return success;
}

bool MainWindow::CaptureData2Buffer()                   // 采集数据到缓存
{
    success = ADQ214_DisarmTrigger(adq_cu, adq_num);
    success = success && ADQ214_SetStreamStatus(adq_cu, adq_num,setupadq.stream_ch);
    success = success && ADQ214_ArmTrigger(adq_cu, adq_num);


    unsigned int samples_to_collect;
    samples_to_collect = setupadq.num_samples_collect;

    int tmpval = 0;

    if (setupadq.trig_mode == 1)	    //触发模式为sofware
    {
        ADQ214_SWTrig(adq_cu, adq_num);
    }

    // ADQ214_SWTrig(adq_cu, adq_num);
    while (samples_to_collect > 0)
    {
        tmpval = tmpval + 1;
        qDebug() << "Loops:" << tmpval;
        if (setupadq.trig_mode == 1)	//If trigger mode is sofware
        {
            ADQ214_SWTrig(adq_cu, adq_num);
        }
        do
        {
            setupadq.collect_result = ADQ214_GetTransferBufferStatus(adq_cu, adq_num, &setupadq.buffers_filled);
            qDebug() << ("Filled: ") << setupadq.buffers_filled;
        } while ((setupadq.buffers_filled == 0) && (setupadq.collect_result));

        setupadq.collect_result = ADQ214_CollectDataNextPage(adq_cu, adq_num);

        int samples_in_buffer;
        if(ADQ214_GetSamplesPerPage(adq_cu, adq_num) > samples_to_collect)
        {
            samples_in_buffer = samples_to_collect;
            qDebug() << "samples_in_buffer = " << samples_in_buffer;
        }
        else
        {
            samples_in_buffer = ADQ214_GetSamplesPerPage(adq_cu, adq_num);
            qDebug() << "samples_in_buffer = " << samples_in_buffer;
        }

        if (ADQ214_GetStreamOverflow(adq_cu, adq_num))
        {
            qDebug() << ("Warning: Streaming Overflow!");
            setupadq.collect_result = 0;
        }

        if (setupadq.collect_result)
        {
            // Buffer all data in RAM before writing to disk, if streaming to disk is need a high performance
            // procedure could be implemented here.
            // Data format is set to 16 bits, so buffer size is Samples*2 bytes
            memcpy((void*)&setupadq.data_stream_target[setupadq.num_samples_collect - samples_to_collect],
                    ADQ214_GetPtrStream(adq_cu, adq_num), samples_in_buffer* sizeof(signed short));
            samples_to_collect -= samples_in_buffer;
            qDebug() << " AA= "<<samples_to_collect;
        }
        else
        {
            qDebug() << ("Collect next data page failed!");
            samples_to_collect = 0;
        }
    }

    success = success && ADQ_DisarmTrigger(adq_cu, adq_num);

    success = success && ADQ214_SetStreamStatus(adq_cu, adq_num,0);
    return success;
}

void MainWindow::WriteData2disk()                   // 将数据直接写入文件
{
    // Write to data to file after streaming to RAM, because ASCII output is too slow for realtime.
    qDebug() << "Writing stream data in RAM to disk" ;

    setupadq.stream_ch &= 0x07;
    QFile fileA("dataA.txt");
    QFile fileB("dataB.txt");
    //    QFile fileBoth("dataBoth.txt");


    switch(setupadq.stream_ch)
    {
    case ADQ214_STREAM_ENABLED_BOTH:
    {
        QTextStream out(&fileA);
        QTextStream out2(&fileB);

        unsigned int samples_to_collect;
        samples_to_collect = setupadq.num_samples_collect;
        if(fileA.open(QFile::WriteOnly)&&fileB.open(QFile::WriteOnly))
        {
            while (samples_to_collect > 0)
            {
                for (int i=0; (i<4) && (samples_to_collect>0); i++)
                {

                    out << setupadq.data_stream_target[setupadq.num_samples_collect-samples_to_collect] << endl;
                    qDebug()<<"CHA -- "<<setupadq.num_samples_collect-samples_to_collect;
                    samples_to_collect--;
                }

                for (int i=0; (i<4) && (samples_to_collect>0); i++)
                {

                    out2 << setupadq.data_stream_target[setupadq.num_samples_collect-samples_to_collect] << endl;
                    qDebug()<<"CHB -- "<<setupadq.num_samples_collect-samples_to_collect;
                    samples_to_collect--;
                }
            }
        }
        fileA.close();
        fileB.close();
        break;
    }
    case ADQ214_STREAM_ENABLED_A:
    {
        if(fileA.open(QFile::WriteOnly))
        {
            QTextStream out(&fileA);
            for (int i=0; i<setupadq.num_samples_collect; i++)
            {
                out<<setupadq.data_stream_target[i]<<endl;
            }
        }
        fileA.close();
        break;
    }
    case ADQ214_STREAM_ENABLED_B:
    {
        if(fileB.open(QFile::WriteOnly))
        {
            QTextStream out(&fileB);
            for (int i=0; i<setupadq.num_samples_collect; i++)
            {
                out<<setupadq.data_stream_target[i]<<endl;
            }
        }
        fileB.close();
        break;
    }
    default:
        break;
    }
}

void MainWindow::WriteSpecData2disk()                   // 将数据转换成功率谱，写入到文件
{
    // Write to data to file after streaming to RAM, because ASCII output is too slow for realtime.
    qDebug() << "Writing streamed Spectrum data in RAM to disk" ;

    setupadq.stream_ch &= 0x07;

    QFile Specfile("data_Spec.txt");

    if(setupadq.stream_ch == ADQ214_STREAM_ENABLED_BOTH)
    {
        QTextStream out(&Specfile);

        unsigned int samples_to_collect;
        samples_to_collect = setupadq.num_samples_collect;

        if (psd_res != nullptr)
            delete psd_res;
        int psd_datanum = samples_to_collect/4;
        psd_res = new PSD_DATA[psd_datanum];
        int i = 0, k = 0;

        for (k=0; (k<psd_datanum); k++,k++)
        {
            psd_res[BitReverseIndex[k]].pos[0] = setupadq.data_stream_target[i];
            psd_res[BitReverseIndex[k]].pos[1] = setupadq.data_stream_target[i+1];
            psd_res[BitReverseIndex[k]].pos[2] = setupadq.data_stream_target[i+4];
            psd_res[BitReverseIndex[k]].pos[3] = setupadq.data_stream_target[i+5];
            psd_res[BitReverseIndex[k+1]].pos[0] = setupadq.data_stream_target[i+2];
            psd_res[BitReverseIndex[k+1]].pos[1] = setupadq.data_stream_target[i+3];
            psd_res[BitReverseIndex[k+1]].pos[2] = setupadq.data_stream_target[i+6];
            psd_res[BitReverseIndex[k+1]].pos[3] = setupadq.data_stream_target[i+7];
            i = i + 8;
            qDebug()<<"Union.Spec["<<k<<"] = "<<psd_res[k].data64;
            qDebug()<<"Union.Spec["<<k+1<<"] = "<<psd_res[k+1].data64;
        }
        for (k=0; (k<psd_datanum); k++)
            out <<psd_res[k].data64 << endl;
    }
    Specfile.close();
}

void MainWindow::Display_Data()                   // 显示数据
{
    switch(setupadq.stream_ch)
    {
    case ADQ214_STREAM_ENABLED_BOTH:
    {
        unsigned int samples_to_collect;
        samples_to_collect = setupadq.num_samples_collect;
        rowCHA.resize((setupadq.num_samples_collect));      //设置数组大小为采集点数
        rowCHB.resize((setupadq.num_samples_collect));
        int j=0,k=0;         //用于双通道计数
        while (samples_to_collect > 0)
        {
            for (int i=0; (i<4) && (samples_to_collect>0); i++)
            {

                rowCHA[j] = setupadq.data_stream_target[setupadq.num_samples_collect-samples_to_collect];
                j++;
                samples_to_collect--;
            }

            for (int i=0; (i<4) && (samples_to_collect>0); i++)
            {

                rowCHB[k] = setupadq.data_stream_target[setupadq.num_samples_collect-samples_to_collect];
                k++;
                samples_to_collect--;
            }
        }

        if(!method)
        {
            lineChart.line(rowCHA,(setupadq.num_samples_collect)/2);  //数组传给linechart
            drawLayoutCHA->addWidget(lineChart.chartView);
            lineChart.line(rowCHB,(setupadq.num_samples_collect)/2);
            drawLayoutCHB->addWidget(lineChart.chartView);
        }
        else
        {
            barChart.chart(rowCHA,(setupadq.num_samples_collect)/2);  //数组传给linechart
            drawLayoutCHA->addWidget(barChart.chartView);
            barChart.chart(rowCHB,(setupadq.num_samples_collect)/2);  //数组传给linechart
            drawLayoutCHB->addWidget(barChart.chartView);
        }
        break;
    }
    case ADQ214_STREAM_ENABLED_A:
    {
        rowCHA.resize(setupadq.num_samples_collect);

        for (int i=0; i<setupadq.num_samples_collect; i++)
        {
            rowCHA[i] = setupadq.data_stream_target[i];
        }
        if(!method)
        {
            lineChart.line(rowCHA,setupadq.num_samples_collect);  //数组传给linechart
            drawLayoutCHA->addWidget(lineChart.chartView);
            // b.chartView->setWindowFlags(Qt::Window|Qt::WindowMinimizeButtonHint
            //               |Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
        }
        else
        {
            barChart.chart(rowCHA,setupadq.num_samples_collect);  //数组传给barChart
            drawLayoutCHA->addWidget(barChart.chartView);
        }
        break;
    }
    case ADQ214_STREAM_ENABLED_B:
    {
        rowCHB.resize(setupadq.num_samples_collect);

        for (int i=0; i<setupadq.num_samples_collect; i++)
        {
            rowCHB[i] = setupadq.data_stream_target[i];
        }
        if(!method)
        {
            lineChart.line(rowCHB,setupadq.num_samples_collect);  //数组传给linechart
            drawLayoutCHB->addWidget(lineChart.chartView);
        }
        else
        {
            barChart.chart(rowCHB,setupadq.num_samples_collect);  //数组传给barChartchart
            drawLayoutCHB->addWidget(barChart.chartView);
        }
        break;
    }
    default:
        break;
    }
}
