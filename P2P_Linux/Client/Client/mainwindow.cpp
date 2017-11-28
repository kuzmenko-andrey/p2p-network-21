#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client_func.h"
#include "filesyst_func.h"
#include <vector>
#include <string>
#include <QString>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <functional>

std::string serverIP;
int serverPort;
std::string currIP;

std::vector<std::string> client_files;
std::vector<std::string> all_files;

boost::asio::io_service service;


//TalkToServer tk("127.0.0.1",9090,service);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() //подключение к серверу
{
    currIP=ui->lineEdit_2->text().toStdString();
    getFileList(client_files);
    std::vector<std::string> ipport;
    split(ipport,ui->lineEdit->text().toStdString());
    std::string ip=ipport[0];
    int port= atoi(ipport[1].c_str());
    TalkToServer talk(ip,port,service);
        if(talk.connect())
        {
            ui->label->setText("");
            talk.sendFileList(client_files,all_files,currIP);
            ui->textEdit->clear();
            for(int i=0;i<all_files.size();i++)
            {
                ui->groupBox->show();
                ui->groupBox_2->hide();
                if(all_files[i]!=":NoNewFiles")
                ui->textEdit->append(all_files[i].c_str());
            }
            serverIP=ip;
            serverPort=port;

            std::thread thr(listenLoop,std::ref(service));
            thr.detach();
        }
        else
            ui->label->setText("Server is not avaiable. Try again");
}

void MainWindow::on_pushButton_2_clicked() //обновить список файлов
{
    boost::system::error_code ec;
    getFileList(client_files);
    TalkToServer talk(serverIP,serverPort,service);
    if(talk.connect())
    {
        talk.sendFileList(client_files,all_files,currIP);
        ui->textEdit->clear();
        for(int i=0;i<all_files.size();i++)
        ui->textEdit->append(all_files[i].c_str());
    }
    else
        ui->label->setText("Server is not avaiable. Try again");
}

void MainWindow::on_pushButton_3_clicked() //скачивает указанный файл
{
    boost::system::error_code ec;
    TalkToServer acceptor(serverIP,serverPort,service);
    boost::asio::streambuf sb;
    std::ostream os(&sb);
    os<<"Send"<<"\n";
    if(ui->lineEdit_3->text().toStdString()!="")
        os<<ui->lineEdit_3->text().toStdString()<<"\n";
    else
    {
        ui->label->setText("Enter file name, please.");
        return;
    }
    os<<currIP;
    tcp::socket sock(service);
    tcp::endpoint ep(boost::asio::ip::address::from_string(serverIP), serverPort);
    sock.connect(ep,ec);
    if(ec)
    {
        std::cerr << "connection to server failed\n";
        ui->label->setText("Server is not avaiable. Try again");
        return;
    }
    boost::asio::write(sock,sb,ec);
    if(ec)
    {
        std::cerr << "writting to server failed\n";
    }
    sock.shutdown(tcp::socket::shutdown_send);
    sock.close();
    std::cout<<"all ok, trying to accept file\n";
    ui->label->setText("The file is downloading now. Please wait.");
    acceptor.acceptFile();
    ui->label->setText("Successfully downloaded.");
    ui->lineEdit_3->clear();
    on_pushButton_2_clicked();
}

void MainWindow::on_pushButton_4_clicked() //отключить данного клиента
{
    boost::system::error_code ec;
    boost::asio::streambuf sb;
    std::ostream os(&sb);
    os<<"Disconnect"<<"\n";
    os<<currIP<<"\n";
    tcp::socket sock(service);
    tcp::endpoint ep(boost::asio::ip::address::from_string(serverIP), serverPort);
    sock.connect(ep,ec);
    if(ec)
    {
        std::cerr << "connection to server failed\n";
        ui->label->setText("Server is not avaiable. Try again");
        return;
    }
    boost::asio::write(sock,sb,ec);
    if(ec)
    {
        std::cerr << "writting to server failed\n";
    }
    sock.shutdown(tcp::socket::shutdown_send);
    sock.close();
    ui->groupBox->hide();
    ui->groupBox_2->show();
  //  tk.doLoop();

}



