#pragma once

namespace wing_ding {
	enum class state {
		pre,
		during,
		post
	};

	class state_manager {
	public:
		state_manager() = default;
		~state_manager() = default;

		void update();
		void draw();

		[[nodiscard]] state get_state();
		void change_state(state new_state);

	private:
		state m_state = state::pre;
	};
}