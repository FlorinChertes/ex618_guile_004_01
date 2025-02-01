#pragma once

#include <QDialog>
#include <QFrame>
#include <QVBoxlayout>

#include <memory>

class Dynamic_dialog : public QDialog 
{
	Q_OBJECT

	QFrame frame;
	QVBoxLayout layout;

public:
	Dynamic_dialog();
};

std::unique_ptr<Dynamic_dialog> start_dynamic_dialog_01();
