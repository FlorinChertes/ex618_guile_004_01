#include "widget_factory_05.h"

#include <QMessageBox>

#include <QScriptValueList>
#include <qDebug>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
MyFunctions::MyFunctions() { qDebug() << "ctor called"; }
MyFunctions::~MyFunctions() { qDebug() << "dtor called"; }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QScriptValue MyFunctions::getTheName() const
{
	return the_name_;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MyFunctions::setTheName(QScriptValue name_)
{
	the_name_ = name_.toString();
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
	qDebug() << "\n*** instantiate a class and expose it to QScriptEngine  ***";
	// Create a instance of a class
	auto my_funcs_ptr { new MyFunctions {} };
	// Expose the instance to QScriptEngine
	my_funcs_ptr->the_exposed_obj_ = engine.newQObject(my_funcs_ptr, QScriptEngine::ScriptOwnership);
	engine.globalObject().setProperty("myFuncs", my_funcs_ptr->the_exposed_obj_);

	{
		qDebug() << "\n*** expose to QScriptEngine the function: function(x) {return x + this.the_name} ***";
		QString script = R"(
			(function(x) {
				return x + this.the_name + "gut!<";
			})
		)";
		// Evaluate the script, loading the function into the QScriptEngine
		my_funcs_ptr->the_add_name_function_ = engine.evaluate(script);
	}

	{
		qDebug() << "\n*** a named function: this.a_named_function = function(x) {return x + this.the_name} ***";
		QString script = R"(
			this.a_named_function = (function(x) {
				return x + this.the_name + "gut!<";
			})
		)";
		// Evaluate the script, loading the function into the QScriptEngine
		my_funcs_ptr->the_named_function_ = engine.evaluate(script);
	}
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
	QScriptValue layoutObject = engine.newQObject(layout);
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

	{
		qDebug() << "\n*** script QScriptEngine using a function adding the_name of an instance variable ***";

		QString func_script{ R"(
		myFuncs.the_name = "Flori ";
		var the_sum = myFuncs.add_the_name(">Laur ");
		the_sum;
		)" };

		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result of Laur + Flori is: " << result.toString();
	}

	{
		qDebug() << "\n*** script QScriptEngine call the named function ***";

		QString func_script{ R"(
		myFuncs.the_name = "Flori ";
		var the_sum = myFuncs.call_the_name_fuction(">Laur ");
		the_sum;
		)" };

		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result named function is: " << result.toString();
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QScriptValue MyFunctions::add_the_name(const QScriptValue& a)
{
	QScriptValueList args;
	args << a;

	QScriptValue result = the_add_name_function_.call(the_exposed_obj_, args);

	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QScriptValue MyFunctions::call_the_name_fuction(const QScriptValue& a)
{
	QScriptValue result{};

	if (the_named_function_.isFunction())
	{
		QScriptValueList args;
		args << a;
		result = the_named_function_.call(the_exposed_obj_, args);
	}

	return result;
}