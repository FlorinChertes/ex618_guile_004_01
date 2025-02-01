#include "widget_factory_03.h"

#include "dialog_dynamic_03.h"

Dynamic_dialog_03::Dynamic_dialog_03()
{
	frame.setMinimumSize(200, 200);
	frame.setLayout(&layout);

	auto factory = new Widget_factory(this);
	factory->setup_engine();
	factory->build_layout();
	
	layout.addLayout(factory->get_layout());
	frame.show();
}

std::unique_ptr<Dynamic_dialog_03> start_dynamic_dialog_03()
{
	return std::make_unique<Dynamic_dialog_03>();
}