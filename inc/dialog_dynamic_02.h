#pragma once

#include <QDialog>
#include <QFrame>
#include <QVBoxlayout>

#include <memory>

class Dynamic_dialog_02 : public QDialog 
{
	Q_OBJECT

	QFrame frame;
	QVBoxLayout layout;

public:
	Dynamic_dialog_02();
};

std::unique_ptr<Dynamic_dialog_02> start_dynamic_dialog_02();
