#include "widget_factory_02.h"

#include <QMessageBox>
#include <QtQml/QJSValue>

#include <qDebug>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
C_List_Basis::C_List_Basis() { qDebug() << "ctor C_List_Basis called"; }
C_List_Basis::~C_List_Basis() { qDebug() << "dtor C_List_Basis called"; }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void C_List_Basis::update()
{
	qDebug() << "called: C_List_Basis::update";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
C_List::C_List() { qDebug() << "ctor C_List called"; }
C_List::~C_List() { qDebug() << "dtor C_List called"; }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//void C_List::update()
//{
//	qDebug() << "called: C_List::update";
//}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void C_List::update(int i)
{
	qDebug() << "called: C_List::update, i = " << i;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Widget_factory::Widget_factory(QObject* parent)
	: QObject(parent)
	, widgets {}
	, engine {}
{}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::setup_engine()
{
	qDebug() << "\n*** instantiate a class, C_List and expose it to QJSEngine  ***";
	// Create a instance of a class
	auto a_C_List{ new C_List{} };
	// Expose the instance to QJSEngine
	QJSValue a_list_value = engine.newQObject(a_C_List);
	engine.globalObject().setProperty("C_List_obj", a_list_value);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QVBoxLayout* Widget_factory::get_layout() const
{
	return qobject_cast<QVBoxLayout*>(
		engine.globalObject().property("theLayoutObject").toQObject());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::build_layout()
{
	QVBoxLayout* layout{ new QVBoxLayout() };
	QJSValue layoutObject = engine.newQObject(layout);
	engine.globalObject().setProperty("theLayoutObject", layoutObject);

	createButton("Click this");
	createButton("Click that");

	for (QWidget* widget : widgets) {
		layout->addWidget(widget);
	}

	QPushButton* button_this = qobject_cast<QPushButton*>(widgets.at(0));
	connect(button_this, &QPushButton::clicked,
		this, &Widget_factory::onButtonClickted_this);

	QPushButton* button_that = qobject_cast<QPushButton*>(widgets.at(1));
	connect(button_that, &QPushButton::clicked,
		this, &Widget_factory::onButtonClickted_that);

	qDebug() << "Layout and buttons built.";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::createButton(const QString& text)
{
	QPushButton* button{ new QPushButton{text} };
	widgets.append(button);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::onButtonClickted_this()
{
	QMessageBox::information(nullptr, "Button Clicked", "Buton clicked this!");
	qDebug() << "Buton clicked this!";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::onButtonClickted_that()
{
	QMessageBox::information(nullptr, "Button Clicked", "Buton clicked that!");
	qDebug() << "Buton clicked that!";

	qDebug() << "\n*** exec script, calling the C++ object C_List_obj exposed to QJSEngine ***";
	QString list_func_script{ R"(
		C_List_obj.update();
	)" };

	QJSValue result = engine.evaluate(list_func_script);
	if (result.isError()) {
		qDebug() << "Uncaught exception at line" 
			<< result.property("lineNumber").toInt() 
			<< " : "
			<< result.toString();
	}

	qDebug() << "list update done";
}

