/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SPLINEINTERPOLATOR_H
#define RHEELENGINE_SPLINEINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

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

		/*
		 * begin, end: point iterators
		 * k: number of functions
		 * i: vector element index (i=0: x, i=1: y, etc.)
		 */
		template<typename InputIt>
		Spline(InputIt begin, InputIt end, unsigned k, unsigned i) {
			unsigned n = 4 * k;
			std::vector<std::vector<Float>> matrix(n, std::vector<Float>(n + 1));

			unsigned index = 0;

			for (auto iter = begin; iter != end; iter++) {
				float t = iter->first;
				_t_values.push_back(t);

				float powers[4] = { 1.0f, t, t * t, t * t * t };

				// function values should equal the input at the correct times
				if (index > 0) {
					matrix[index * 2 - 1][4 * index - 4] = powers[3];
					matrix[index * 2 - 1][4 * index - 3] = powers[2];
					matrix[index * 2 - 1][4 * index - 2] = powers[1];
					matrix[index * 2 - 1][4 * index - 1] = powers[0];
					matrix[index * 2 - 1][n] = Get_(iter, i);
				}

				if (index < k) {
					matrix[index * 2][4 * index + 0] = powers[3];
					matrix[index * 2][4 * index + 1] = powers[2];
					matrix[index * 2][4 * index + 2] = powers[1];
					matrix[index * 2][4 * index + 3] = powers[0];
					matrix[index * 2][n] = Get_(iter, i);
				}

				if (index > 0 && index < k) {
					// first derivatives should match
					matrix[2 * k + index - 1][4 * index - 4 + 0] = powers[2] * 3.0f;
					matrix[2 * k + index - 1][4 * index - 4 + 1] = powers[1] * 2.0f;
					matrix[2 * k + index - 1][4 * index - 4 + 2] = powers[0] * 1.0f;

					matrix[2 * k + index - 1][4 * index + 0] = powers[2] * -3.0f;
					matrix[2 * k + index - 1][4 * index + 1] = powers[1] * -2.0f;
					matrix[2 * k + index - 1][4 * index + 2] = powers[0] * -1.0f;

					// second derivatives should match
					matrix[2 * k + (k - 1) + index - 1][4 * index - 4 + 0] = powers[1] * 6.0f;
					matrix[2 * k + (k - 1) + index - 1][4 * index - 4 + 1] = powers[0] * 2.0f;

					matrix[2 * k + (k - 1) + index - 1][4 * index + 0] = powers[1] * -6.0f;
					matrix[2 * k + (k - 1) + index - 1][4 * index + 1] = powers[0] * -2.0f;
				}

				// boundary conditions: natural
				if (index == 0) {
					matrix[n - 2][0] = 6.0 * t;
					matrix[n - 2][1] = 2.0;
				}

				if (index == k) {
					matrix[n - 1][n - 4] = 6.0 * t;
					matrix[n - 1][n - 3] = 2.0;
				}

				// next point
				index++;
			}

			_factors = GaussianElimination_(matrix, n);
		}

		Float operator()(float t) const {
			unsigned index = std::upper_bound(_t_values.begin(), _t_values.end(), t) - _t_values.begin() - 1;
			return _factors[4 * index + 0] * t * t * t +
					_factors[4 * index + 1] * t * t +
					_factors[4 * index + 2] * t +
					_factors[4 * index + 3];
		}

	private:
		std::vector<Float> _factors;
		std::vector<float> _t_values;

	private:
		template<typename InputIt, unsigned N_ = N, std::enable_if_t<N_ == 1, int> = 0>
		static Float Get_(InputIt iterator, unsigned i) {
			return iterator->second;
		}

		template<typename InputIt, unsigned N_ = N, std::enable_if_t<N_ != 1, int> = 0>
		static Float Get_(InputIt iterator, unsigned i) {
			return iterator->second[i];
		}

		static std::vector<Float> GaussianElimination_(std::vector<std::vector<Float>>& augmentedMatrix, unsigned n) {
			// Source:
			// https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/

			for (int i = 0; i < n; i++) {
				// Search for maximum in this column
				Float maxEl = abs(augmentedMatrix[i][i]);
				unsigned maxRow = i;

				for (unsigned k = i + 1; k < n; k++) {
					if (abs(augmentedMatrix[k][i]) > maxEl) {
						maxEl = abs(augmentedMatrix[k][i]);
						maxRow = k;
					}
				}

				// Swap maximum row with current row (column by column)
				for (unsigned k = i; k < n + 1; k++) {
					Float tmp = augmentedMatrix[maxRow][k];
					augmentedMatrix[maxRow][k] = augmentedMatrix[i][k];
					augmentedMatrix[i][k] = tmp;
				}

				// Make all rows below this one 0 in current column
				for (unsigned k = i + 1; k < n; k++) {
					Float c = -augmentedMatrix[k][i] / augmentedMatrix[i][i];

					for (unsigned j = i; j < n + 1; j++) {
						if (i == j) {
							augmentedMatrix[k][j] = 0;
						} else {
							augmentedMatrix[k][j] += c * augmentedMatrix[i][j];
						}
					}
				}
			}

			// Solve equation Ax=b for an upper triangular matrix A
			std::vector<Float> x(n);
			for (int i = n - 1; i >= 0; i--) {
				x[i] = augmentedMatrix[i][n] / augmentedMatrix[i][i];

				for (int k = i - 1; k >= 0; k--) {
					augmentedMatrix[k][n] -= augmentedMatrix[k][i] * x[i];
				}
			}

			return x;
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

		return GetValue_(t);
	}

private:
	mutable bool _dirty = false;
	mutable std::array<Spline, N> _splines{};

	void CalculateSpline_() const {
		for (unsigned i = 0; i < N; i++) {
			_splines[i] = Spline(this->points.begin(), this->points.end(), this->points.size() - 1, i);
		}
	}

	// GetValue for floats
	template<unsigned N_ = N, std::enable_if_t<N_ == 1, int> = 0>
	V GetValue_(float t) const {
		return _splines[0](t);
	}

	// GetValue for vectors
	template<unsigned N_ = N, std::enable_if_t<N_ != 1, int> = 0>
	V GetValue_(float t) const {
		V v{};

		for (unsigned i = 0; i < N; i++) {
			v[i] = _splines[i](t);
		}
	}

};

}

#endif
