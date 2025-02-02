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

	{
		qDebug() << "\n*** start the prototype approach, instantiate a class, \
		MyResource and expose it to QJSEngine  ***";
		// 1. Create a C++ object
		QObject* res = new MyResource();

		// 2. Wrap the C++ object as a QJSValue
		QJSValue jsRes = engine.newQObject(res);

		// 3. Create a prototype object
		QJSValue proto = engine.newObject();

		// 4. Define a JavaScript function for `toString()` that calls `myToString()`
		QJSValue overrideToString = engine.evaluate(
			"(function() { "
			"    // 'this' is the JS object wrapping MyResource."
			"    // We'll call the Q_INVOKABLE 'myToString()' from C++."
			"    return this.myToString(); "
			"})"
		);

		// 5. Assign that function to the prototype's "toString" property
		proto.setProperty("toString", overrideToString);

		// 6. Attach this new prototype to your wrapped object
		jsRes.setPrototype(proto);

		// 7. Expose this object to global JS scope under a convenient name
		engine.globalObject().setProperty("myObj", jsRes);

		qDebug() << "\n*** end the prototype approach, instantiate a class, \
		MyResource and expose it to QJSEngine  ***";
	}

	{

		qDebug() << "\n*** start the wrapper approach  ***";

		// 1. Create your QObject and wrap it via newQObject
		QObject* res = new MyResource();

		// 1.1. Wrap the C++ object as a QJSValue
		QJSValue cxxResourceValue = engine.newQObject(res);

		QString wrapperJs = R"(
		(function(cxxObj) {
			return {
				// Store the QJSValue referencing your QObject
				_cxx: cxxObj,

				// Provide your custom toString
				toString: function() {
					// Forward to the real C++ method: myToString
					return this._cxx.myToString();
				},

				// You might also forward any other methods you need...
				callOtherMethod: function() {
					return this._cxx.someOtherInvokable();
				},

				callMoreMethod: function() {
					var input_text = "before hello: ";
					return this._cxx.someMoreInvokable(input_text);
				}
			};
		})
		)";

		QJSValue wrapperFn = engine.evaluate(wrapperJs);
		// Now call that function in JS, passing `cxxResourceValue`
		QJSValue wrappedObject = wrapperFn.call(QJSValueList{ cxxResourceValue });

		// Make it accessible as "myObjWrapp"
		engine.globalObject().setProperty("myObjWrapp", wrappedObject);

		qDebug() << "\n*** end the the wrapper approach  ***";
	}

	qDebug() << "\n*** end setup engine  ***";
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
		C_List_obj.update(42);
	)" };

	QJSValue result = engine.evaluate(list_func_script);
	if (result.isError()) {
		qDebug() << "Uncaught exception at line" 
			<< result.property("lineNumber").toInt() 
			<< " : "
			<< result.toString();
	}

	qDebug() << "list update done";

	{
		// 8. Evaluate a JavaScript snippet that uses `myObj.toString()`

		// this line does not work with prototype solution
		// by calling implicit the special toString under the new name: myToString
		qDebug() << "call implicit of myToString unsing prototype";
		QJSValue result_ = engine.evaluate("myObj.toString() + \" some text\";");

		qDebug() << "after evaluate using prototype";

		// 9. Check the result in C++
		qDebug() << "Result of myObj.toString() in JS is:" << result_.toString();
		// This should print:  "Hello from MyResource::myToString()"


		// this line works with prototype solution,
		// by calling explicit the special toString under the new name: myToString
		qDebug() << "call explicit of myToString unsing only Q_INVOKABLE";
		result_ = engine.evaluate("myObj.myToString() + \" some text\";");

		qDebug() << "after evaluate using Q_INVOKABLE";

		// 9. Check the result in C++
		qDebug() << "Result of myObj.toString() in JS is:" << result_.toString();
		// This should print:  "Hello from MyResource::myToString()"

		qDebug() << "toString done";
	}

	{
		// this line works with wrapp solution,
		// by calling implicit the special toString under the new name: myToString
		qDebug() << "call implicit of myToString unsing Wrapper";
		QJSValue result_ = engine.evaluate("myObjWrapp + \" some text\";");
		qDebug() << "after evaluate using Wrapp";
		// 9. Check the result in C++
		qDebug() << "Result of Wrapp in JS is:" << result_.toString();
		// This should print:  "Hello from MyResource::myToString()"

		result_ = engine.evaluate("myObjWrapp.callOtherMethod() + \" some more text\";");
		qDebug() << "Result of Wrapp in JS is:" << result_.toString();

		result_ = engine.evaluate("myObjWrapp.callMoreMethod() + \" some more more text\";");
		qDebug() << "Result of Wrapp in JS is:" << result_.toString();


		qDebug() << "toString Wrapp done";
	}

}

