#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

namespace bf {
	template <typename T, typename N> class TBox2 {
	public:
		T start{}, size{};

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

		inline void expandPoint(T point) {
			T end = getEnd();

			if (point.x < start.x) {
				start.x = point.x;
				size.x = end.x - start.x;
			}
			else if (point.x > end.x) {
				size.x = point.x - start.x;
			}

			if (point.y < start.y) {
				start.y = point.y;
				size.y = end.y - start.y;
			}
			else if (point.y > end.y) {
				size.y = point.y - start.y;
			}
		}
	};

	typedef TBox2<glm::vec2, float> Box2;
	typedef TBox2<glm::ivec2, int> Box2i;
}