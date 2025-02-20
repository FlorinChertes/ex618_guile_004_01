#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include <QtQml/QJSEngine>
#include <QtQml/QQmlEngine>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class MyResource : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QJSValue texts READ getTexts)

public:
	// Constructor, etc.
	explicit MyResource(QObject* parent
	, QJSEngine& engine)
		: QObject(parent)
		, engine_{engine}
	{}
	// accessors for composition 'texts'
	QJSValue getTexts() const;

	// We'll use this to demonstrate a custom string conversion:
	Q_INVOKABLE QString myToString() {
		return QString("Hello from MyResource::myToString()");
	}

	Q_INVOKABLE QString someOtherInvokable() {
		return QString("Hello from MyResource::someOtherInvokable()");
	}
	Q_INVOKABLE QString someMoreInvokable(const QJSValue& input) {
		return input.toString() + QString("Hello from MyResource::someMoreInvokable()");
	}

	QJSEngine& engine_;
};


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
