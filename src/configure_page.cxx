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
#include <QFileDialog>
#include <QDir>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPixmap>

#include "configure_page.hh"
#include "settings.hh"
#include "backup.hh"
#include "lang.hh"

ConfigurePage::ConfigurePage()
    : layout(new QVBoxLayout),
      toolbar(new QToolBar),
      add(new QToolButton),
      remove(new QToolButton),
      listWidget(new QListWidget),
      destWidget(new QWidget),
      destLayout(new QHBoxLayout),
      destLabel(new QLabel(t("Destination"))),
      destLocation(new QLineEdit),
      destBrowse(new QPushButton(t("Browse"))),
      save(new QPushButton(t("Save Settings")))
{
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    add->setIcon(QIcon::fromTheme("folder-new",QPixmap(":/icons/folder-new.png")));
    remove->setIcon(QIcon::fromTheme("edit-delete",QPixmap(":/icons/edit-delete.png")));

    toolbar->addWidget(add);
    toolbar->addWidget(remove);

    destWidget->setLayout(destLayout);
    destLayout->addWidget(destLabel);
    destLayout->addWidget(destLocation);
    destLayout->addWidget(destBrowse);

    layout->addWidget(toolbar);
    layout->addWidget(listWidget);
    layout->addWidget(destWidget);
    layout->addWidget(save);

    connect(add,&QToolButton::clicked,this,&ConfigurePage::onAddClicked);
    connect(remove,&QToolButton::clicked,this,&ConfigurePage::onRemoveClicked);
    connect(destBrowse,&QPushButton::clicked,this,&ConfigurePage::onDestBrowseClicked);
    connect(save,&QPushButton::clicked,this,&ConfigurePage::onSaveClicked);

    this->load();
}

ConfigurePage::~ConfigurePage() {
    delete layout;
    delete toolbar;
    delete listWidget;
    delete destWidget;
    delete save;
}

void ConfigurePage::load() {
    destLocation->setText(Settings::dest());

    QStringList items = Backup::backupList();
    listWidget->clear();
    listWidget->addItems(items);
}

QString ConfigurePage::chooseFolder() {
    QString path = "";
    QFileDialog dialog;
    dialog.setWindowTitle(t("Choose Folder"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setDirectory(QDir::homePath());
    if (dialog.exec()) {
        if (dialog.selectedFiles().size()==0) {
            return "";
        }
        path = dialog.selectedFiles().at(0);
    }
    return path;
}

void ConfigurePage::onAddClicked() {
    QString chosen = chooseFolder();
    if (chosen!="") {
        listWidget->addItem(chosen);
    }
}

void ConfigurePage::onRemoveClicked() {
    QListWidgetItem *item = listWidget->currentItem();
    if (item==nullptr) {
        return;
    }
    delete item;
}

void ConfigurePage::onDestBrowseClicked() {
    QString chosen = chooseFolder();
    if (chosen!="") {
        destLocation->setText(chosen);
    }
}

void ConfigurePage::onSaveClicked() {
    Settings::setDest(destLocation->text());

    QStringList items;
    for (int i = 0; i<listWidget->count(); i++) {
        items.push_back(listWidget->item(i)->text());
    }
    Backup::setBackupList(items);

    QMessageBox msg;
    msg.setWindowTitle(t("Information"));
    msg.setText(t("settings_saved"));
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}
