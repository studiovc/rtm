#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2017 Nicholas Frechette & Animation Compression Library contributors
// Copyright (c) 2018 Nicholas Frechette & Realtime Math contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "rtm/math.h"
#include "rtm/scalard.h"
#include "rtm/vector4d.h"
#include "rtm/impl/compiler_utils.h"
#include "rtm/impl/memory_utils.h"
#include "rtm/impl/quat_common.h"

RTM_IMPL_FILE_PRAGMA_PUSH

namespace rtm
{
	//////////////////////////////////////////////////////////////////////////
	// Setters, getters, and casts
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned quaternion from memory.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_load(const double* input) RTM_NO_EXCEPT
	{
		return quat_set(input[0], input[1], input[2], input[3]);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned quaternion from memory.
	//////////////////////////////////////////////////////////////////////////
	RTM_DEPRECATED("Use quat_load instead, to be removed in v2.0")
	inline quatd quat_unaligned_load(const double* input) RTM_NO_EXCEPT
	{
		return quat_set(input[0], input[1], input[2], input[3]);
	}

	//////////////////////////////////////////////////////////////////////////
	// Casts a vector4 to a quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline quatd vector_to_quat(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ input.xy, input.zw };
#else
		return quatd{ input.x, input.y, input.z, input.w };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Casts a quaternion float32 variant to a float64 variant.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_cast(const quatf& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ _mm_cvtps_pd(input), _mm_cvtps_pd(_mm_shuffle_ps(input, input, _MM_SHUFFLE(3, 2, 3, 2))) };
#elif defined(RTM_NEON_INTRINSICS)
		return quatd{ double(vgetq_lane_f32(input, 0)), double(vgetq_lane_f32(input, 1)), double(vgetq_lane_f32(input, 2)), double(vgetq_lane_f32(input, 3)) };
#else
		return quatd{ double(input.x), double(input.y), double(input.z), double(input.w) };
#endif
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_get_x
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(input.xy);
#else
				return input.x;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ input.xy };
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the quaternion [x] component (real part).
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_get_x quat_get_x(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_get_x{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_get_y
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(_mm_shuffle_pd(input.xy, input.xy, 1));
#else
				return input.y;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ _mm_shuffle_pd(input.xy, input.xy, 1) };
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the quaternion [y] component (real part).
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_get_y quat_get_y(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_get_y{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_get_z
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(input.zw);
#else
				return input.z;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ input.zw };
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the quaternion [z] component (real part).
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_get_z quat_get_z(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_get_z{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_get_w
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(_mm_shuffle_pd(input.zw, input.zw, 1));
#else
				return input.w;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ _mm_shuffle_pd(input.zw, input.zw, 1) };
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the quaternion [w] component (imaginary part).
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_get_w quat_get_w(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_get_w{ input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [x] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_x(const quatd& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ _mm_move_sd(input.xy, _mm_set_sd(lane_value)), input.zw };
#else
		return quatd{ lane_value, input.y, input.z, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [x] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_x(const quatd& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return quatd{ _mm_move_sd(input.xy, lane_value.value), input.zw };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [y] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_y(const quatd& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ _mm_shuffle_pd(input.xy, _mm_set_sd(lane_value), 0), input.zw };
#else
		return quatd{ input.x, lane_value, input.z, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [y] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_y(const quatd& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return quatd{ _mm_shuffle_pd(input.xy, lane_value.value, 0), input.zw };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [z] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_z(const quatd& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ input.xy, _mm_move_sd(input.zw, _mm_set_sd(lane_value)) };
#else
		return quatd{ input.x, input.y, lane_value, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [z] component (real part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_z(const quatd& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return quatd{ input.xy, _mm_move_sd(input.zw, lane_value.value) };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [w] component (imaginary part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_w(const quatd& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ input.xy, _mm_shuffle_pd(input.zw, _mm_set_sd(lane_value), 0) };
#else
		return quatd{ input.x, input.y, input.z, lane_value };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the quaternion [w] component (imaginary part) and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_set_w(const quatd& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return quatd{ input.xy, _mm_shuffle_pd(input.zw, lane_value.value, 0) };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Writes a quaternion to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void quat_store(const quatd& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = quat_get_x(input);
		output[1] = quat_get_y(input);
		output[2] = quat_get_z(input);
		output[3] = quat_get_w(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a quaternion to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void quat_store(const quatd& input, float4d* output) RTM_NO_EXCEPT
	{
		output->x = quat_get_x(input);
		output->y = quat_get_y(input);
		output->z = quat_get_z(input);
		output->w = quat_get_w(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a quaternion to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	RTM_DEPRECATED("Use quat_store instead, to be removed in v2.0")
	inline void quat_unaligned_write(const quatd& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = quat_get_x(input);
		output[1] = quat_get_y(input);
		output[2] = quat_get_z(input);
		output[3] = quat_get_w(input);
	}



	//////////////////////////////////////////////////////////////////////////
	// Arithmetic
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Returns the quaternion conjugate.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_conjugate(const quatd& input) RTM_NO_EXCEPT
	{
		return quat_set(-quat_get_x(input), -quat_get_y(input), -quat_get_z(input), quat_get_w(input));
	}

	//////////////////////////////////////////////////////////////////////////
	// Multiplies two quaternions.
	// Note that due to floating point rounding, the result might not be perfectly normalized.
	// Multiplication order is as follow: local_to_world = quat_mul(local_to_object, object_to_world)
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_mul(const quatd& lhs, const quatd& rhs) RTM_NO_EXCEPT
	{
		double lhs_x = quat_get_x(lhs);
		double lhs_y = quat_get_y(lhs);
		double lhs_z = quat_get_z(lhs);
		double lhs_w = quat_get_w(lhs);

		double rhs_x = quat_get_x(rhs);
		double rhs_y = quat_get_y(rhs);
		double rhs_z = quat_get_z(rhs);
		double rhs_w = quat_get_w(rhs);

		double x = (rhs_w * lhs_x) + (rhs_x * lhs_w) + (rhs_y * lhs_z) - (rhs_z * lhs_y);
		double y = (rhs_w * lhs_y) - (rhs_x * lhs_z) + (rhs_y * lhs_w) + (rhs_z * lhs_x);
		double z = (rhs_w * lhs_z) + (rhs_x * lhs_y) - (rhs_y * lhs_x) + (rhs_z * lhs_w);
		double w = (rhs_w * lhs_w) - (rhs_x * lhs_x) - (rhs_y * lhs_y) - (rhs_z * lhs_z);

		return quat_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Multiplies a quaternion and a 3D vector, rotating it.
	// Multiplication order is as follow: world_position = quat_mul_vector3(local_vector, local_to_world)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d quat_mul_vector3(const vector4d& vector, const quatd& rotation) RTM_NO_EXCEPT
	{
		quatd vector_quat = quat_set_w(vector_to_quat(vector), 0.0);
		quatd inv_rotation = quat_conjugate(rotation);
		return quat_to_vector(quat_mul(quat_mul(inv_rotation, vector_quat), rotation));
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_dot
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard lhs_x = quat_get_x(lhs);
				const scalard lhs_y = quat_get_y(lhs);
				const scalard lhs_z = quat_get_z(lhs);
				const scalard lhs_w = quat_get_w(lhs);
				const scalard rhs_x = quat_get_x(rhs);
				const scalard rhs_y = quat_get_y(rhs);
				const scalard rhs_z = quat_get_z(rhs);
				const scalard rhs_w = quat_get_w(rhs);
				const scalard xx = scalar_mul(lhs_x, rhs_x);
				const scalard yy = scalar_mul(lhs_y, rhs_y);
				const scalard zz = scalar_mul(lhs_z, rhs_z);
				const scalard ww = scalar_mul(lhs_w, rhs_w);
				return scalar_cast(scalar_add(scalar_add(xx, yy), scalar_add(zz, ww)));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard lhs_x = quat_get_x(lhs);
				const scalard lhs_y = quat_get_y(lhs);
				const scalard lhs_z = quat_get_z(lhs);
				const scalard lhs_w = quat_get_w(lhs);
				const scalard rhs_x = quat_get_x(rhs);
				const scalard rhs_y = quat_get_y(rhs);
				const scalard rhs_z = quat_get_z(rhs);
				const scalard rhs_w = quat_get_w(rhs);
				const scalard xx = scalar_mul(lhs_x, rhs_x);
				const scalard yy = scalar_mul(lhs_y, rhs_y);
				const scalard zz = scalar_mul(lhs_z, rhs_z);
				const scalard ww = scalar_mul(lhs_w, rhs_w);
				return scalar_add(scalar_add(xx, yy), scalar_add(zz, ww));
			}
#endif

			quatd lhs;
			quatd rhs;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Quaternion dot product: lhs . rhs
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_dot quat_dot(const quatd& lhs, const quatd& rhs) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_dot{ lhs, rhs };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the squared length/norm of the quaternion.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_dot quat_length_squared(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_dot{ input, input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_length
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = quat_length_squared(input);
				return scalar_cast(scalar_sqrt(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = quat_length_squared(input);
				return scalar_sqrt(len_sq);
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the length/norm of the quaternion.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_length quat_length(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_length{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct quatd_quat_length_reciprocal
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = quat_length_squared(input);
				return scalar_cast(scalar_sqrt_reciprocal(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = quat_length_squared(input);
				return scalar_sqrt_reciprocal(len_sq);
			}
#endif

			quatd input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the reciprocal length/norm of the quaternion.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::quatd_quat_length_reciprocal quat_length_reciprocal(const quatd& input) RTM_NO_EXCEPT
	{
		return rtm_impl::quatd_quat_length_reciprocal{ input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns a normalized quaternion.
	// Note that if the input quaternion is invalid (pure zero or with NaN/Inf),
	// the result is undefined.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_normalize(const quatd& input) RTM_NO_EXCEPT
	{
		// TODO: Use high precision recip sqrt function and vector_mul
		double length = quat_length(input);
		//float length_recip = quat_length_reciprocal(input);
		vector4d input_vector = quat_to_vector(input);
		//return vector_to_quat(vector_mul(input_vector, length_recip));
		return vector_to_quat(vector_div(input_vector, vector_set(length)));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the linear interpolation between start and end for a given alpha value.
	// The formula used is: ((1.0 - alpha) * start) + (alpha * end).
	// Interpolation is stable and will return 'start' when 'alpha' is 0.0 and 'end' when it is 1.0.
	// This is the same instruction count when FMA is present but it might be slightly slower
	// due to the extra multiplication compared to: start + (alpha * (end - start)).
	// Note that if 'start' and 'end' are on the opposite ends of the hypersphere, 'end' is negated
	// before we interpolate. As such, when 'alpha' is 1.0, either 'end' or its negated equivalent
	// is returned. Furthermore, if 'start' and 'end' aren't exactly normalized, the result might
	// not match exactly when 'alpha' is 0.0 or 1.0 because we normalize the resulting quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_lerp(const quatd& start, const quatd& end, double alpha) RTM_NO_EXCEPT
	{
		// To ensure we take the shortest path, we apply a bias if the dot product is negative
		vector4d start_vector = quat_to_vector(start);
		vector4d end_vector = quat_to_vector(end);
		double dot = vector_dot(start_vector, end_vector);
		double bias = dot >= 0.0 ? 1.0 : -1.0;
		// ((1.0 - alpha) * start) + (alpha * (end * bias)) == (start - alpha * start) + (alpha * (end * bias))
		vector4d value = vector_mul_add(vector_mul(end_vector, bias), alpha, vector_neg_mul_sub(start_vector, alpha, start_vector));
		return quat_normalize(vector_to_quat(value));
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Returns the linear interpolation between start and end for a given alpha value.
	// The formula used is: ((1.0 - alpha) * start) + (alpha * end).
	// Interpolation is stable and will return 'start' when 'alpha' is 0.0 and 'end' when it is 1.0.
	// This is the same instruction count when FMA is present but it might be slightly slower
	// due to the extra multiplication compared to: start + (alpha * (end - start)).
	// Note that if 'start' and 'end' are on the opposite ends of the hypersphere, 'end' is negated
	// before we interpolate. As such, when 'alpha' is 1.0, either 'end' or its negated equivalent
	// is returned. Furthermore, if 'start' and 'end' aren't exactly normalized, the result might
	// not match exactly when 'alpha' is 0.0 or 1.0 because we normalize the resulting quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_lerp(const quatd& start, const quatd& end, const scalard& alpha) RTM_NO_EXCEPT
	{
		// To ensure we take the shortest path, we apply a bias if the dot product is negative
		vector4d start_vector = quat_to_vector(start);
		vector4d end_vector = quat_to_vector(end);
		double dot = vector_dot(start_vector, end_vector);
		double bias = dot >= 0.0 ? 1.0 : -1.0;
		// ((1.0 - alpha) * start) + (alpha * (end * bias)) == (start - alpha * start) + (alpha * (end * bias))
		vector4d alpha_v = vector_set(alpha);
		vector4d value = vector_mul_add(vector_mul(end_vector, bias), alpha_v, vector_neg_mul_sub(start_vector, alpha_v, start_vector));
		return quat_normalize(vector_to_quat(value));
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Returns a component wise negated quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_neg(const quatd& input) RTM_NO_EXCEPT
	{
		return vector_to_quat(vector_mul(quat_to_vector(input), -1.0));
	}



	//////////////////////////////////////////////////////////////////////////
	// Conversion to/from axis/angle/euler
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Returns the rotation axis and rotation angle that make up the input quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline void quat_to_axis_angle(const quatd& input, vector4d& out_axis, double& out_angle) RTM_NO_EXCEPT
	{
		constexpr double epsilon = 1.0E-8;
		constexpr double epsilon_squared = epsilon * epsilon;

		const scalard input_w = quat_get_w(input);
		out_angle = scalar_cast(scalar_acos(input_w)) * 2.0;

		const double scale_sq = scalar_max(1.0 - quat_get_w(input) * quat_get_w(input), 0.0);
		out_axis = scale_sq >= epsilon_squared ? vector_mul(quat_to_vector(input), vector_set(scalar_sqrt_reciprocal(scale_sq))) : vector_set(1.0, 0.0, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the rotation axis part of the input quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d quat_get_axis(const quatd& input) RTM_NO_EXCEPT
	{
		constexpr double epsilon = 1.0E-8;
		constexpr double epsilon_squared = epsilon * epsilon;

		const double scale_sq = scalar_max(1.0 - quat_get_w(input) * quat_get_w(input), 0.0);
		return scale_sq >= epsilon_squared ? vector_mul(quat_to_vector(input), vector_set(scalar_sqrt_reciprocal(scale_sq))) : vector_set(1.0, 0.0, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the rotation angle part of the input quaternion.
	//////////////////////////////////////////////////////////////////////////
	inline double quat_get_angle(const quatd& input) RTM_NO_EXCEPT
	{
		const scalard input_w = quat_get_w(input);
		return scalar_cast(scalar_acos(input_w)) * 2.0;
	}

	//////////////////////////////////////////////////////////////////////////
	// Creates a quaternion from a rotation axis and a rotation angle.
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_from_axis_angle(const vector4d& axis, double angle) RTM_NO_EXCEPT
	{
		vector4d sincos_ = scalar_sincos(0.5 * angle);
		vector4d sin_ = vector_dup_x(sincos_);
		scalard cos_ = vector_get_y(sincos_);

		return vector_to_quat(vector_set_w(vector_mul(sin_, axis), cos_));
	}

	//////////////////////////////////////////////////////////////////////////
	// Creates a quaternion from Euler Pitch/Yaw/Roll angles.
	// Pitch is around the Y axis (right)
	// Yaw is around the Z axis (up)
	// Roll is around the X axis (forward)
	//////////////////////////////////////////////////////////////////////////
	inline quatd quat_from_euler(double pitch, double yaw, double roll) RTM_NO_EXCEPT
	{
		double sp;
		double sy;
		double sr;
		double cp;
		double cy;
		double cr;

		scalar_sincos(pitch * 0.5, sp, cp);
		scalar_sincos(yaw * 0.5, sy, cy);
		scalar_sincos(roll * 0.5, sr, cr);

		return quat_set(cr * sp * sy - sr * cp * cy,
			-cr * sp * cy - sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy);
	}



	//////////////////////////////////////////////////////////////////////////
	// Comparisons and masking
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Returns true if the input quaternion does not contain any NaN or Inf, otherwise false.
	//////////////////////////////////////////////////////////////////////////
	inline bool quat_is_finite(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));

		const __m128d infinity = _mm_set1_pd(std::numeric_limits<double>::infinity());
		__m128d is_infinity_xy = _mm_cmpeq_pd(abs_input_xy, infinity);
		__m128d is_infinity_zw = _mm_cmpeq_pd(abs_input_zw, infinity);

		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		__m128d is_not_finite_xy = _mm_or_pd(is_infinity_xy, is_nan_xy);
		__m128d is_not_finite_zw = _mm_or_pd(is_infinity_zw, is_nan_zw);
		__m128d is_not_finite = _mm_or_pd(is_not_finite_xy, is_not_finite_zw);
		return _mm_movemask_pd(is_not_finite) == 0;
#else
		return scalar_is_finite(quat_get_x(input)) && scalar_is_finite(quat_get_y(input)) && scalar_is_finite(quat_get_z(input)) && scalar_is_finite(quat_get_w(input));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if the input quaternion is normalized, otherwise false.
	//////////////////////////////////////////////////////////////////////////
	inline bool quat_is_normalized(const quatd& input, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		double length_squared = quat_length_squared(input);
		return scalar_abs(length_squared - 1.0) < threshold;
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if the two quaternions are nearly equal component wise, otherwise false.
	//////////////////////////////////////////////////////////////////////////
	inline bool quat_near_equal(const quatd& lhs, const quatd& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_all_near_equal(quat_to_vector(lhs), quat_to_vector(rhs), threshold);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if the input quaternion is nearly equal to the identity quaternion
	// by comparing its rotation angle.
	//////////////////////////////////////////////////////////////////////////
	inline bool quat_near_identity(const quatd& input, double threshold_angle = 0.00284714461) RTM_NO_EXCEPT
	{
		// See the quatf version of quat_near_identity for details.
		const scalard input_w = quat_get_w(input);
		const double positive_w_angle = scalar_acos(scalar_cast(scalar_abs(input_w))) * 2.0;
		return positive_w_angle < threshold_angle;
	}
}

RTM_IMPL_FILE_PRAGMA_POP
