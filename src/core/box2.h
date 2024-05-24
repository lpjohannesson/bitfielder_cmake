#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

namespace bf {
	template <typename T, typename N> class TBox2 {
	public:
		T start{}, size{};

		inline T getEnd() const {
			return start + size;
		}
		inline N getArea() const {
			return size.x * size.y; 
		}

		inline TBox2<T, N> section(const TBox2<T, N> section) const {
			T end = getEnd();

			T sectionStart = glm::lerp(start, end, section.start);
			T sectionEnd = glm::lerp(start, end, section.getEnd());

			return { sectionStart, sectionEnd - sectionStart };
		}

		static inline bool overlaps(TBox2<T, N> box1, TBox2<T, N> box2) {
			glm::vec2
				start1 = box1.start,
				end1 = box1.getEnd(),
				start2 = box2.start,
				end2 = box2.getEnd();

			return
				start1.x < end2.x &&
				end1.x > start2.x &&
				start1.y < end2.y &&
				end1.y > start2.y;
		}
	};

	typedef TBox2<glm::vec2, float> Box2;
	typedef TBox2<glm::ivec2, int> Box2i;
}