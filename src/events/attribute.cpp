/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>

#include "events/attribute_impl.hpp"


namespace handystats { namespace events { namespace attribute {

event_message* create_set_event(
		std::string&& attr_name,
		const handystats::attribute::value_type& value,
		const handystats::attribute::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(attr_name);
	message->destination_type = event_destination_type::ATTRIBUTE;

	message->timestamp = timestamp;

	message->event_type = event_type::SET;
	message->event_data = new handystats::attribute::value_type(value);

	return message;
}

void delete_set_event(event_message* message) {
	delete static_cast<handystats::attribute::value_type*>(message->event_data);

	delete message;
}

void delete_event(event_message* message) {
	switch (message->event_type) {
		case event_type::SET:
			delete_set_event(message);
			break;
	}
}

void process_set_event(handystats::attribute& attr, const event_message& message) {
	const auto& value = *reinterpret_cast<handystats::attribute::value_type*>(message.event_data);
	attr.set(value);
}

void process_event(handystats::attribute& attr, const event_message& message) {
	switch (message.event_type) {
		case event_type::SET:
			process_set_event(attr, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::attribute
