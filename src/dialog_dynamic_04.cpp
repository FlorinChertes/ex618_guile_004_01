#include "widget_factory_04.h"

#include "dialog_dynamic_04.h"

Dynamic_dialog::Dynamic_dialog()
{
	frame.setMinimumSize(200, 200);
	frame.setLayout(&layout);

	auto factory = new Widget_factory(this);
	factory->setup_engine();
	factory->build_layout();

	layout.addLayout(factory->get_layout());
	frame.show();
}

std::unique_ptr<Dynamic_dialog> start_dynamic_dialog()
{
	return std::make_unique<Dynamic_dialog>();
}