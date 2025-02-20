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
	{
		qDebug() << "\n*** instantiate a class and expose it to QScriptEngine  ***";
		// Create a instance of a class
		auto my_funcs_ptr { new MyFunctions {} };
		// Expose the instance to QScriptEngine
		my_funcs_ptr->the_exposed_obj_ = engine.newQObject(my_funcs_ptr, QScriptEngine::ScriptOwnership);
		engine.globalObject().setProperty("myFuncs", my_funcs_ptr->the_exposed_obj_);

		{
			qDebug() << "\n*** expose to QScriptEngine the named function: function(x) {return x + 5} ***";
			QString script = R"(
				(function(x) {
					return x + 5;
				})
			)";
			// Evaluate the script, loading the function into the QScriptEngine
			my_funcs_ptr->the_add_function_ = engine.evaluate(script);
		}

		{
			qDebug() << "\n*** expose to QScriptEngine the named function: function(x) {return x + the_name} ***";
			QString script = R"(
				(function(x) {
					return x + this.the_name + "gut!<";
				})
			)";
			// Evaluate the script, loading the function into the QScriptEngine
			my_funcs_ptr->the_add_name_function_ = engine.evaluate(script);
		}

		{
			// Evaluate a script that assigns the function to myFuncs.add_the_names
			QString script = R"(
				myFuncs.add_the_names = function(x) {
					return x + this.the_name + "gut!<";
					};
			)";
			engine.evaluate(script);
		}
	}

	{
		qDebug() << "\n*** expose to QScriptEngine the named function: addFive(x) {return x + 5} ***";
		QString script = R"(
			function addFive(x) {
				return x + 5;
			}
		)";
		// Evaluate the script, loading the function into the QScriptEngine
		engine.evaluate(script);
	}

	{
		qDebug() << "\n*** expose to QScriptEngine the named function: function(x) {return x + 5} ***";
		QString script = R"(
			(function(x) {
				return x + 5;
			})
		)";
		// Evaluate the script, loading the function into the QScriptEngine
		the_function_ = engine.evaluate(script);
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
		qDebug() << "\n*** exec script, calling the C++ object exposed to QScriptEngine ***";
		QString func_script{ R"(
		var the_sum = myFuncs.sum(1, 2);
		the_sum;
		)" };
		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result of 1 + 2 is: " << result.toNumber();
	}

	{
		qDebug() << "\n*** exec script, function calling the C++ object exposed to QScriptEngine ***";
		QString func_script{ R"(
		var the_sum = myFuncs.sum(1, 2);
		the_sum;
		)" };
		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result of 1 + 2 is: " << result.toNumber();
	}

	{
		qDebug() << "\n*** script QScriptEngine ***";

		// Prepare a function call to 'addFive' with an argument
		QScriptValue func_add_five = engine.globalObject().property("addFive");

		// Prepare arguments for the call (just a single integer 5 here)
		QScriptValueList args;
		args << 5;

		// Call the function (the first parameter is 'thisObject',
			// which can be QScriptValue() if not needed)
		QScriptValue result = func_add_five.call(QScriptValue(), args);

		// Output the result
		qDebug() << "\nResult of 5 + 5 is: " << result.toNumber();
	}

	{
		qDebug() << "\n*** script QScriptEngine using instance variable ***";

		// Prepare arguments for the call (just a single integer 5 here)
		QScriptValueList args;
		args << 5;

		// Call the function (the first parameter is 'thisObject',
			// which can be QScriptValue() if not needed)
		QScriptValue result = the_function_.call(QScriptValue(), args);

		// Output the result
		qDebug() << "\nResult of 5 + 5 is: " << result.toNumber();
	}

	{
		qDebug() << "\n*** script QScriptEngine using a function of an instance variable ***";

		QString func_script{ R"(
		var the_sum = myFuncs.add_five(6);
		the_sum;
		)" };

		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result of 6 + 5 is: " << result.toNumber();
	}

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
		qDebug() << "\n*** script QScriptEngine using a function 'add_the_names', added on te fly to an instance variable ***";

		QString func_script{ R"(
		myFuncs.the_name = "Flori ";
		var the_sum = myFuncs.add_the_names(">Laur ");
		the_sum;
		)" };

		QScriptValue result = engine.evaluate(func_script);
		qDebug() << "Result of the on the fly added function: 'add_the_names' is: " << result.toString();
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
double MyFunctions::add_five(double a)
{
	QScriptValueList args;
	args << a;

	// Call the function (the first parameter is 'thisObject',
		// which can be QScriptValue() if not needed)
	QScriptValue result = the_add_function_.call(QScriptValue(), args);

	// Output the result
	qDebug() << "\nResult of a + 5 is: " << result.toNumber();

	return result.toNumber();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
QScriptValue MyFunctions::add_the_name(const QScriptValue& a)
{
	QScriptValueList args;
	args << a;

	// Call the function (the first parameter is 'thisObject',
		// which can be QScriptValue() if not needed)

	// if first argument of the next call is: the_exposed_obj_,
	// then in the: the_add_name_function_ use: this.the_name;
	QScriptValue result = the_add_name_function_.call(the_exposed_obj_, args);

	// if first argument of the next call is: QScriptValue(),
	// then in the: the_add_name_function_ use: myFuncs.the_name;
	//QScriptValue result = the_add_name_function_.call(QScriptValue(), args);

	// Output the result
	qDebug() << "\nResult of a + the_name is: " << result.toString();

	return result;
}
