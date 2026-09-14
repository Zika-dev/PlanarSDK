
namespace core {
	struct Timer {
		float elapsed{};
		float goal{};

		bool finished() {
			return elapsed >= goal;
		}

		bool tick(float delta) {
			if (!started) return false;

			elapsed += delta;

			if (elapsed > goal) elapsed = goal;

			if (finished()) {
				if (extraFrame) {
					extraFrame = false;
					return true;
				}

				return false;
			}

			return true;
		}

		float progress() {
			return elapsed / goal;
		}

		void start() {
			started = true;
		}

		void stop() {
			started = false;
		}

		void reset() {
			elapsed = 0.0f;
			stop();
			extraFrame = true;
		}

		bool running() {
			return started;
		}

	private:

		bool started = false;
		bool extraFrame = true; // For returning finished one update later than when actually finished (so that progress is 100% for 1 frame)
	};

}