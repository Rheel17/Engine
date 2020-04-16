/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SPLINEINTERPOLATOR_H
#define RHEELENGINE_SPLINEINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

#include <string_format.h>

namespace rheel {

template<typename Float, unsigned N>
using SplineInterpolatedType = std::conditional_t<N == 1, Float, glm::vec<N, Float, glm::defaultp>>;

template<typename Float, unsigned N = 1>
class SplineInterpolator : public Interpolator<SplineInterpolatedType<Float, N>> {
	static_assert(std::is_floating_point_v<Float>,
	        "Spline interpolation is only valid for floating point types. Use SplineIterpolator<type, N> to interpolate vectors.");

	using V = SplineInterpolatedType<Float, N>;

private:
	class Spline {

	public:
		Spline() = default;

		template<typename InputIt>
		Spline(InputIt begin, InputIt end, unsigned k, unsigned i) {
			unsigned n = 4 * k;
			std::vector<std::vector<Float>> matrix(n, std::vector<Float>(n));
			std::vector<Float> vector(n);

			unsigned index = 0;

			for (auto iter = begin; iter != end; iter++) {
				float t = iter->first;
				float powers[4] = { t * t * t, t * t, t, 1.0f };

				if (index > 0) {
					matrix[index * 2 - 1][4 * index - 4] = powers[0];
					matrix[index * 2 - 1][4 * index - 3] = powers[1];
					matrix[index * 2 - 1][4 * index - 2] = powers[2];
					matrix[index * 2 - 1][4 * index - 1] = powers[3];
					vector[index * 2 - 1] = Get_(iter, i);
				}

				if (index < k) {
					matrix[index * 2][4 * index + 0] = powers[0];
					matrix[index * 2][4 * index + 1] = powers[1];
					matrix[index * 2][4 * index + 2] = powers[2];
					matrix[index * 2][4 * index + 3] = powers[3];
					vector[index * 2] = Get_(iter, i);
				}

				// next point
				index++;
			}

			for (int j = 0; j < n; j++) {
				for (int l = 0; l < n; l++) {
					std::cout << string_format(" % 6.1f ", matrix[j][l]);
				}

				std::cout << string_format("     % 6.1f", vector[j]) << std::endl;
			}
		}

	private:
		template<typename InputIt, unsigned N_ = N, std::enable_if_t<N_ == 1, int> = 0>
		static Float Get_(InputIt iterator, unsigned i) {
			return iterator->second;
		}

		template<typename InputIt, unsigned N_ = N, std::enable_if_t<N_ != 1, int> = 0>
		static Float Get_(InputIt iterator, unsigned i) {
			return iterator->second[i];
		}

	};

public:
	SplineInterpolator() = default;

	virtual void AddPoint(float t, const V& value) override {
		Interpolator<V>::AddPoint(t, value);
		_dirty = true;
	}

	virtual void AddPoint(float t, V&& value) override {
		Interpolator<V>::AddPoint(t, value);
		_dirty = true;
	}

	virtual void RemovePoint(float t) override {
		Interpolator<V>::RemovePoint(t);
		_dirty = true;
	}

protected:
	virtual V GetValue(float t) const override {
		if (_dirty) {
			CalculateSpline_();
			_dirty = false;
		}

		return V{};
	}

private:
	mutable bool _dirty = false;
	mutable std::array<Spline, N> _splines{};

	void CalculateSpline_() const {
		for (unsigned i = 0; i < N; i++) {
			_splines[i] = Spline(this->points.begin(), this->points.end(), this->points.size() - 1, i);
		}
	}

};

}

#endif
