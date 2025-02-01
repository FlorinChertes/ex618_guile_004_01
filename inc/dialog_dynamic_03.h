#pragma once

#include <QDialog>
#include <QFrame>
#include <QVBoxlayout>

#include <memory>

class Dynamic_dialog_03 : public QDialog 
{
	Q_OBJECT

	QFrame frame;
	QVBoxLayout layout;

public:
	Dynamic_dialog_03();
};

std::unique_ptr<Dynamic_dialog_03> start_dynamic_dialog_03();
