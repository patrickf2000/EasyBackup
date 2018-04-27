// Copyright 2018 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//	list of conditions and the following disclaimer in the documentation and/or
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may
//	be used to endorse or promote products derived from this software
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <QScrollBar>
#include <QDir>
#include <QMessageBox>

#include "backup_page.hh"
#include "settings.hh"
#include "backup.hh"

BackupPage::BackupPage()
    : layout(new QVBoxLayout),
      begin(new QPushButton("Begin Backup")),
      output(new QTextEdit)
{
    this->setLayout(layout);

    output->setStyleSheet("background-color:black;color: white;");
    output->setReadOnly(true);
    output->setText("Click \"Begin Backup\" to begin.\n"
                    "Or, click the \"Configure\" tab to adjust settings.\n");

    layout->addWidget(begin);
    layout->addWidget(output);

    proc = new QProcess;
#ifdef _WIN32
    QString path = QDir::homePath() + "\\AppData\\Local\\EasyBackup\\backup.bat";
    proc->setProgram(path);
#else
    proc->setProgram(qgetenv("SHELL"));
    proc->setArguments(QStringList() << "/tmp/backup.sh");
#endif

    connect(begin,&QPushButton::clicked,this,&BackupPage::onBeginClicked);
    connect(proc,&QProcess::readyReadStandardOutput,this,&BackupPage::onOutputReady);
    connect(proc,SIGNAL(finished(int)),this,SLOT(onDone()));
}

BackupPage::~BackupPage() {
    delete layout;
    delete begin;
    delete output;
}

void BackupPage::onBeginClicked() {
    QString txt = "Backing up with the following configuration:";
    txt+="Command: "+Settings::command()+"\n";
    txt+="Arguments: "+Settings::args()+"\n";
    txt+="Destination: "+Settings::dest()+"\n\n";
    output->setPlainText(txt);

    Backup::genScript();

    proc->start();
}

void BackupPage::onOutputReady() {
    QString oldText = output->toPlainText();
    oldText += proc->readAllStandardOutput();
    output->setPlainText(oldText);
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

void BackupPage::onDone() {
    QString oldText = output->toPlainText();
    oldText += "\nThe backup has been completed.\n";
    output->setPlainText(oldText);
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());

    QMessageBox msg;
    msg.setWindowTitle("Backup Done");
    msg.setText("The backup has been completed.");
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}
