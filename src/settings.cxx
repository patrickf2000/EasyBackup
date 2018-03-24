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
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <iostream>

#include "settings.hh"
#include "tinyxml2.h"

using namespace tinyxml2;

QString Settings::filePath;

void Settings::init() {
#ifdef _WIN32
    filePath = QDir::homePath()+"\\AppData\\Local\\EasyBackup\\";
#else
    filePath = QDir::homePath()+"/.local/share/easybackup/";
#endif
    if (!QDir(filePath).exists()) {
        QDir().mkpath(filePath);
    }
    filePath+="backup.xml";
    if (!QFile(filePath).exists()) {
        genDefault();
    }
}

void Settings::genDefault() {
#ifdef _WIN32
    QString content = "<?xml version=\"1.0\"?>\r\n";
    content+="<backup>\r\n<settings>\r\n";
    content+="<command>robocopy</command>\r\n";
    content+="<args>/mir</args>\r\n";
    content+="<dest>"+QDir::homePath()+"\\Documents\\Backup</dest>\r\n";
    content+="</settings>\r\n</backup>\r\n";
#else
    QString content = "<?xml version=\"1.0\"?>\n";
    content+="<backup>\n<settings>\n";
    content+="<command>rsync</command>\n";
    content+="<args>-av --delete</args>\n";
    content+="<dest>"+QDir::homePath()+"/Documents/Backup</dest>\n";
    content+="</settings>\n</backup>\n";
#endif

    QFile file(filePath);
    if (file.open(QFile::ReadWrite)) {
        QTextStream writer(&file);
        writer << content;
        writer.flush();
        file.close();
    }
}

void Settings::setCommand(QString command) {
    setXmlValue("settings/command",command);
}

void Settings::setArgs(QString args) {
    setXmlValue("settings/args",args);
}

void Settings::setDest(QString path) {
    setXmlValue("settings/dest",path);
}

QString Settings::command() {
    return xmlValue("settings/command");
}

QString Settings::args() {
    return xmlValue("settings/args");
}

QString Settings::dest() {
    return xmlValue("settings/dest");
}

void Settings::setXmlValue(QString path, QString value) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(filePath.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("backup");
    if (root==nullptr) {
        return;
    }

    XMLElement *element = getElement(root,path);
    if (element==nullptr) {
        return;
    } else {
        element->SetText(value.toStdString().c_str());
    }

    doc->SaveFile(filePath.toStdString().c_str());
}

XMLElement *Settings::getElement(XMLElement *root, QString path) {
    XMLElement *element = nullptr;
    bool isFirst = true;
    QString currentItem = "";
    QString lastItem = "";
    for (int i = 0; i<path.length(); i++) {
        if (path.at(i)=='/') {
            lastItem = currentItem;
            currentItem = "";
            if (isFirst) {
                isFirst = false;
                element = root->FirstChildElement(lastItem.toStdString().c_str());
                if (element==nullptr) {
                    return nullptr;
                }
            } else {
                element = element->FirstChildElement(lastItem.toStdString().c_str());
                if (element==nullptr) {
                    return nullptr;
                }
            }
        } else {
            currentItem+=path.at(i);
        }
    }
    if (isFirst) {
        element = root->FirstChildElement(currentItem.toStdString().c_str());
    } else {
        element = element->FirstChildElement(currentItem.toStdString().c_str());
    }
    return element;
}

QString Settings::xmlValue(QString path) {
    QString value = "";

    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(filePath.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("backup");
    if (root==nullptr) {
        return "";
    }

    XMLElement *element = getElement(root,path);
    if (element==nullptr) {
        value = "Null";
    } else {
        value = QString(element->GetText());
    }

    return value;
}
