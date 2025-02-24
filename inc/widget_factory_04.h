#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include <QtQml/QJSEngine>
#include <QtQml/QJSValue>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class MyFunctions : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QJSValue the_name READ getTheName WRITE setTheName)

public:
	MyFunctions();
	~MyFunctions();

	QJSValue getTheName() const;
	void setTheName(QJSValue name_);

	Q_INVOKABLE QJSValue add_the_name(const QJSValue& a);
	Q_INVOKABLE QJSValue call_the_name_fuction(const QJSValue& a);

private:
	QString the_name_;

public:
	QJSValue the_add_name_function_;
	QJSValue the_named_function_;

	QJSValue the_exposed_obj_;
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
