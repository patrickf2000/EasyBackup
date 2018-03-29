#pragma once

#include <QString>

#include <trans/trans.hh>

QString _(std::string str) {
	return QString::fromStdString(trans(str));
}
