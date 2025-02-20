#include "widget_factory.h"

#include <QMessageBox>
#include <QtQml/QJSValue>

#include <qDebug>

MyFunctions::MyFunctions() { qDebug() << "ctor called"; }
MyFunctions::~MyFunctions() { qDebug() << "dtor called"; }

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
	qDebug() << "\n*** expose to QJSEngine the named function: addFive(x) {return x + 5} ***";
	QString script{ "function addFive(x) {return x + 5}" };
	// Evaluate the script, loading the function into the QJSEngine
	engine.evaluate(script);

	qDebug() << "\n*** instantiate a class and expose it to QJSEngine  ***";
	// Create a instance of a class
	auto myFuncs{ new MyFunctions {} };
	// Expose the instance to QJSEngine
	QJSValue value = engine.newQObject(myFuncs);
	engine.globalObject().setProperty("myFuncs", value);
	// Instruct QJSEngine that JavaScript should own (and delete) myFuncs
	// for Qt6
	//QJSEngine::setObjectOwnership(myFuncs, QJSEngine::JavaScriptOwnership);
	// for Qt5, only if needed!
	//QQmlEngine::setObjectOwnership(myFuncs, QQmlEngine::JavaScriptOwnership);

	qDebug() << "\n*** instantiate a factory and expose it to QJSEngine  ***";
	Factory* factory{ new Factory{} };
	QJSValue factoryValue = engine.newQObject(factory);
	engine.globalObject().setProperty("factory", factoryValue);
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

	// Prepare a function call to 'addFive' with an argument
	QJSValue func_add_five = engine.globalObject().property("addFive");
	// Call this function with an argument
	QJSValue result = func_add_five.call(QJSValueList() << 5);
	// Output the result
	qDebug() << "\nResult of 5 + 5 is: " << result.toNumber();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Widget_factory::onButtonClickted_that()
{
	QMessageBox::information(nullptr, "Button Clicked", "Buton clicked that!");
	qDebug() << "Buton clicked that!";

	qDebug() << "\n*** exec script, calling the C++ object exposed to QJSEngine ***";
	QString func_script{ R"(
		var the_sum = myFuncs.sum(1, 2);
		the_sum;
	)" };
	QJSValue result = engine.evaluate(func_script);
	qDebug() << "Result of 1 + 2 is: " << result.toNumber();

	qDebug() << "\n*** exec script, calling the C++ object exposed to QJSEngine ***";
	func_script =  R"(
		var field = factory.createField();
		the_sum = field.sum(1 ,2)
		the_sum;
	)" ;
	result = engine.evaluate(func_script);
	qDebug() << "Result of 1 + 2 is: " << result.toNumber();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QObject* qjsValueToQObject(QJSValue value)
{
	if (value.isQObject()) {
		QVariant variant = value.toVariant();
		QObject* obj = variant.value<QObject*>();
		return obj;
	}

	return nullptr;
}

