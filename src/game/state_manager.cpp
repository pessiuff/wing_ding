#include "state_manager.h"

namespace wing_ding {
	void state_manager::update() {
		switch (m_state) {
		case state::pre:
			break;
		case state::during:
			break;
		case state::post:
			break;
		}
	}
	void state_manager::draw() {
		switch (m_state) {
		case state::pre:
			break;
		case state::during:
			break;
		case state::post:
			break;
		}
	}

	wing_ding::state state_manager::get_state() {
		return m_state;
	}

	void state_manager::change_state(state new_state) {
		m_state = new_state;
	}
}