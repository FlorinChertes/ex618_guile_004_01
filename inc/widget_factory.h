#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include <QtQml/QJSEngine>
#include <QtQml/QQmlEngine>

QObject* qjsValueToQObject(QJSValue value);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class MyFunctions : public QObject
{
	Q_OBJECT
		
public:
	MyFunctions();
	~MyFunctions();

	Q_INVOKABLE double sum(double a, double b) {
		return a + b;
	}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Factory : public QObject
{
	Q_OBJECT

public:
	Factory() {}

	Q_INVOKABLE QObject* createField() {
		MyFunctions* object{ new MyFunctions{} };
		QQmlEngine::setObjectOwnership(object, QQmlEngine::JavaScriptOwnership);
		return object;
	}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Widget_factory : public QObject
{
	Q_OBJECT

	QList<QWidget*> widgets;
	QJSEngine engine;

	void createButton(const QString& text);

private slots:
	void onButtonClickted_this();
	void onButtonClickted_that();

public:
	Widget_factory(QObject* parent = nullptr);

	void setup_engine();
	void build_layout();

	QVBoxLayout* get_layout() const;
};
