#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class MyFunctions : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QScriptValue the_name READ getTheName WRITE setTheName)

public:
	MyFunctions();
	~MyFunctions();

	QScriptValue getTheName() const;
	void setTheName(QScriptValue name_);

	Q_INVOKABLE QScriptValue add_the_name(const QScriptValue& a);

private:
	QString the_name_;

public:
	QScriptValue the_add_name_function_;
	QScriptValue the_exposed_obj_;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Widget_factory : public QObject
{
	Q_OBJECT

	QList<QWidget*> widgets;
	QScriptEngine engine;

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
