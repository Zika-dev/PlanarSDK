#pragma once

#include "AudioClip.hpp"

namespace core::audio {
	struct AudioClipInfo {

		AudioClipInfo() = default;

		AudioClipInfo(AudioClip& clip) {
			volume = clip.volume;
			loop = clip.loop;
			speed = clip.speed;
		}

		float volume = 1.0f;
		bool loop = false;
		float speed = 1.0f;

	};
}
