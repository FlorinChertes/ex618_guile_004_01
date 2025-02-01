#include "widget_factory_02.h"

#include "dialog_dynamic_02.h"

Dynamic_dialog_02::Dynamic_dialog_02()
{
	frame.setMinimumSize(200, 200);
	frame.setLayout(&layout);

	auto factory = new Widget_factory(this);
	factory->setup_engine();
	factory->build_layout();
	
	layout.addLayout(factory->get_layout());
	frame.show();
}

std::unique_ptr<Dynamic_dialog_02> start_dynamic_dialog_02()
{
	return std::make_unique<Dynamic_dialog_02>();
}