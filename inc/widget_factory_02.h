#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include <QtQml/QJSEngine>
#include <QtQml/QQmlEngine>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class C_List_Basis : public QObject
{
	Q_OBJECT

public:
	C_List_Basis();
	~C_List_Basis();

	Q_INVOKABLE void update();
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class C_List : public C_List_Basis
{
	Q_OBJECT

public:
	C_List();
	~C_List();

	//Q_INVOKABLE void update();
	Q_INVOKABLE void update(int i);
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
