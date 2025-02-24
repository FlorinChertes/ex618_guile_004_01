#include "widget_factory_04.h"

#include <QMessageBox>

#include <qDebug>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
MyFunctions::MyFunctions() { qDebug() << "ctor called"; }
MyFunctions::~MyFunctions() { qDebug() << "dtor called"; }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QJSValue MyFunctions::getTheName() const
{
	return the_name_;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MyFunctions::setTheName(QJSValue name_)
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
	my_funcs_ptr->the_exposed_obj_ = engine.newQObject(my_funcs_ptr);
	engine.globalObject().setProperty("myFuncs", my_funcs_ptr->the_exposed_obj_);

	{
		qDebug() << "\n*** expose to QScriptEngine the named function: function(x) {return x + this.the_name} ***";
		QString script = R"(
			(function(x) {
				return x + this.the_name + "gut!<";
			})
		)";
		// Evaluate the script, loading the function into the QScriptEngine
		my_funcs_ptr->the_add_name_function_ = engine.evaluate(script);
		if (my_funcs_ptr->the_add_name_function_.isError()) {
			qWarning() << "Error evaluating function script:" << engine.evaluate("Error").toString();
		}
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

	{
		qDebug() << "\n*** script QScriptEngine using a function adding the_name of an instance variable ***";

		QString func_script{ R"(
		myFuncs.the_name = "Flori ";
		var the_sum = myFuncs.add_the_name(">Laur ");
		the_sum;
		)" };

		QJSValue result = engine.evaluate(func_script);
		if (result.isError()) {
			qWarning() << "Error evaluating func_script:" << engine.evaluate("Error").toString();
		}
		else
		{
			qDebug() << "Result of Laur + Flori is: " << result.toString();
		}
	}

	{
		qDebug() << "\n*** script QJSEngine call the named function ***";

		QString func_script{ R"(
		myFuncs.the_name = "Flori ";
		var the_sum = myFuncs.call_the_name_fuction(">Laur ");
		the_sum;
		)" };

		QJSValue result = engine.evaluate(func_script);
		qDebug() << "Result named function is: " << result.toString();
	}

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QJSValue MyFunctions::add_the_name(const QJSValue& a)
{
	// We want to call the JS function with `this = the_exposed_obj_`
	// plus the single argument `a`.
	QJSValueList args;
	args << a;

	QJSValue result = the_add_name_function_.callWithInstance(the_exposed_obj_, args);
	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QJSValue MyFunctions::call_the_name_fuction(const QJSValue& a)
{
	QJSValue result{};

	if (the_named_function_.isCallable())
	{
		QJSValueList args;
		args << a;
		result = the_named_function_.callWithInstance(the_exposed_obj_, args);
	}

	return result;
}
